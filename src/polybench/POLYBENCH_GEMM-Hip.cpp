//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GEMM.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <cstdlib>
#include <iostream>

namespace rajaperf
{
namespace polybench
{

//
// Define thread block shape for Hip execution
//
#define j_block_sz (32)
#define i_block_sz (block_size / j_block_sz)

#define POLY_GEMM_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP \
  j_block_sz, i_block_sz

#define POLY_GEMM_THREADS_PER_BLOCK_HIP \
  dim3 nthreads_per_block(POLY_GEMM_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP, 1);

#define POLY_GEMM_NBLOCKS_HIP \
  const Index_type num_tiles_m = RAJA_DIVIDE_CEILING_INT(ni, i_block_sz); \
  const Index_type num_tiles_n = RAJA_DIVIDE_CEILING_INT(nj, j_block_sz); \
  dim3 nblocks(static_cast<size_t>(num_tiles_m * num_tiles_n), \
               static_cast<size_t>(1), \
               static_cast<size_t>(1));


#define POLYBENCH_GEMM_BODY3_LOAD_LOOKAHEAD2 \
  Index_type k = 0; \
  for ( ; k + 1 < nk; k += 2 ) { \
    Real_type a0 = A[i * nk + k + 0]; \
    Real_type b0 = B[(k + 0) * nj + j]; \
    Real_type a1 = A[i * nk + k + 1]; \
    Real_type b1 = B[(k + 1) * nj + j]; \
    dot += alpha * a0 * b0; \
    dot += alpha * a1 * b1; \
  } \
  for ( ; k < nk; ++k ) { \
    POLYBENCH_GEMM_BODY3; \
  }


static Index_type getPositiveEnvValue(const char* name, Index_type default_value)
{
  const char* value = std::getenv(name);
  if (value == nullptr || *value == '\0') {
    return default_value;
  }

  char* end = nullptr;
  long parsed = std::strtol(value, &end, 10);
  return (end != value && parsed > 0) ? static_cast<Index_type>(parsed)
                                      : default_value;
}

static Index_type getEnabledEnvValue(const char* name, Index_type default_value)
{
  const char* value = std::getenv(name);
  if (value == nullptr || *value == '\0') {
    return default_value;
  }

  char* end = nullptr;
  long parsed = std::strtol(value, &end, 10);
  return (end != value) ? static_cast<Index_type>(parsed != 0)
                        : default_value;
}

static __device__ inline Index_type remap_xcd(Index_type physical_pid,
                                              Index_type total_tiles,
                                              Index_type num_xcds)
{
  if (num_xcds <= 1) {
    return physical_pid;
  }

  Index_type xcd = physical_pid % num_xcds;
  Index_type local = physical_pid / num_xcds;
  Index_type tiles_per_xcd = total_tiles / num_xcds;
  Index_type extra_tiles = total_tiles % num_xcds;

  Index_type start = xcd * tiles_per_xcd +
                     ((xcd < extra_tiles) ? xcd : extra_tiles);

  return start + local;
}

static __device__ inline void get_xcd_swizzled_gemm_tile(
    Index_type physical_pid,
    Index_type num_tiles_m,
    Index_type num_tiles_n,
    Index_type group_m,
    Index_type num_xcds,
    Index_type enable_xcd_swizzle,
    Index_type& block_m,
    Index_type& block_n)
{
  if (enable_xcd_swizzle == 0) {
    block_m = physical_pid / num_tiles_n;
    block_n = physical_pid % num_tiles_n;
    return;
  }

  Index_type total_tiles = num_tiles_m * num_tiles_n;
  Index_type pid = remap_xcd(physical_pid, total_tiles, num_xcds);
  Index_type actual_group_m = (group_m > 0) ? group_m : 1;

  Index_type group_span = actual_group_m * num_tiles_n;
  Index_type group_id = pid / group_span;
  Index_type first_m = group_id * actual_group_m;
  Index_type remaining_m = num_tiles_m - first_m;
  Index_type actual_gm = (remaining_m < actual_group_m) ?
                         remaining_m : actual_group_m;
  Index_type local = pid % group_span;

  block_m = first_m + local % actual_gm;
  block_n = local / actual_gm;
}

template < size_t j_block_size, size_t i_block_size >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_gemm(Real_ptr C, Real_ptr A, Real_ptr B,
                          Real_type alpha, Real_type beta,
                          Index_type ni, Index_type nj, Index_type nk,
                          Index_type group_m, Index_type num_xcds,
                          Index_type enable_xcd_swizzle)
{
  Index_type num_tiles_m = RAJA_DIVIDE_CEILING_INT(ni, i_block_size);
  Index_type num_tiles_n = RAJA_DIVIDE_CEILING_INT(nj, j_block_size);
  Index_type block_m = 0;
  Index_type block_n = 0;
  get_xcd_swizzled_gemm_tile(blockIdx.x, num_tiles_m, num_tiles_n,
                             group_m, num_xcds, enable_xcd_swizzle,
                             block_m, block_n);

  Index_type i = block_m * i_block_size + threadIdx.y;
  Index_type j = block_n * j_block_size + threadIdx.x;

  if ( i < ni && j < nj ) {
    POLYBENCH_GEMM_BODY1;
    POLYBENCH_GEMM_BODY2;
    POLYBENCH_GEMM_BODY3_LOAD_LOOKAHEAD2;
    POLYBENCH_GEMM_BODY4;
  }
}

template < size_t j_block_size, size_t i_block_size, typename Lambda >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_gemm_lam(Index_type ni, Index_type nj,
                              Index_type group_m, Index_type num_xcds,
                              Index_type enable_xcd_swizzle,
                              Lambda body)
{
  Index_type num_tiles_m = RAJA_DIVIDE_CEILING_INT(ni, i_block_size);
  Index_type num_tiles_n = RAJA_DIVIDE_CEILING_INT(nj, j_block_size);
  Index_type block_m = 0;
  Index_type block_n = 0;
  get_xcd_swizzled_gemm_tile(blockIdx.x, num_tiles_m, num_tiles_n,
                             group_m, num_xcds, enable_xcd_swizzle,
                             block_m, block_n);

  Index_type i = block_m * i_block_size + threadIdx.y;
  Index_type j = block_n * j_block_size + threadIdx.x;

  if ( i < ni && j < nj ) {
    body(i, j);
  }
}


template < size_t block_size >
void POLYBENCH_GEMM::runHipVariantImpl(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  POLYBENCH_GEMM_DATA_SETUP;

  const Index_type group_m = 8;
  const Index_type num_xcds =
    getPositiveEnvValue("RAJAPERF_GEMM_XCDS", 6);
  const Index_type enable_xcd_swizzle =
    getEnabledEnvValue("RAJAPERF_GEMM_XCD_SWIZZLE", 1);

  if ( vid == Base_HIP ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      POLY_GEMM_THREADS_PER_BLOCK_HIP;
      POLY_GEMM_NBLOCKS_HIP;
      constexpr size_t shmem = 0;

      RPlaunchHipKernel(
          (poly_gemm<POLY_GEMM_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          C, A, B,
          alpha, beta,
          ni, nj, nk,
          group_m, num_xcds, enable_xcd_swizzle );

    }
    stopTimer();

  } else if ( vid == Lambda_HIP ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      POLY_GEMM_THREADS_PER_BLOCK_HIP;
      POLY_GEMM_NBLOCKS_HIP;
      constexpr size_t shmem = 0;

      auto poly_gemm_lambda = [=] __device__ (Index_type i, Index_type j) {
        POLYBENCH_GEMM_BODY1;
        POLYBENCH_GEMM_BODY2;
        POLYBENCH_GEMM_BODY3_LOAD_LOOKAHEAD2;
        POLYBENCH_GEMM_BODY4;
      };

      RPlaunchHipKernel(
       (poly_gemm_lam<POLY_GEMM_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP,
                      decltype(poly_gemm_lambda)>),
       nblocks, nthreads_per_block,
       shmem, res.get_stream(),
       ni, nj, group_m, num_xcds, enable_xcd_swizzle,
       poly_gemm_lambda );

    }
    stopTimer();

  } else if (vid == RAJA_HIP) {

    POLYBENCH_GEMM_VIEWS_RAJA;

    using EXEC_POL =
      RAJA::KernelPolicy<
        RAJA::statement::HipKernelFixedAsync<i_block_sz * j_block_sz,
          RAJA::statement::For<0, RAJA::hip_global_size_y_direct<i_block_sz>,   // i
            RAJA::statement::For<1, RAJA::hip_global_size_x_direct<j_block_sz>, // j
              RAJA::statement::Lambda<0, RAJA::Params<0>>,
              RAJA::statement::Lambda<1, RAJA::Segs<0,1>>,
              RAJA::statement::For<2, RAJA::seq_exec,           // k
                RAJA::statement::Lambda<2, RAJA::Segs<0,1,2>, RAJA::Params<0>>
              >,
              RAJA::statement::Lambda<3, RAJA::Segs<0,1>, RAJA::Params<0>>
            >
          >
        >
      >;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::kernel_param_resource<EXEC_POL>(

          RAJA::make_tuple( RAJA::RangeSegment{0, ni},
                            RAJA::RangeSegment{0, nj},
                            RAJA::RangeSegment{0, nk} ),
          RAJA::tuple<Real_type>{0.0},  // variable for dot
          res,

          [=] __device__ (Real_type& dot) {
            POLYBENCH_GEMM_BODY1_RAJA;
          },
          [=] __device__ (Index_type i, Index_type j) {
            POLYBENCH_GEMM_BODY2_RAJA;
          },
          [=] __device__ (Index_type i, Index_type j, Index_type k,
                          Real_type& dot) {
            POLYBENCH_GEMM_BODY3_RAJA;
          },
          [=] __device__ (Index_type i, Index_type j,
                          Real_type& dot) {
            POLYBENCH_GEMM_BODY4_RAJA;
          }
        );

      }
      stopTimer();

  } else {
      getCout() << "\n  POLYBENCH_GEMM : Unknown Hip variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(POLYBENCH_GEMM, Hip, Base_HIP, Lambda_HIP, RAJA_HIP)

} // end namespace polybench
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP
