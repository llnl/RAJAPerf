//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_JACOBI_2D.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

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

#define JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP \
  j_block_sz, i_block_sz

#define JACOBI_2D_THREADS_PER_BLOCK_HIP \
  dim3 nthreads_per_block(JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP, 1);

#define JACOBI_2D_NBLOCKS_HIP \
  dim3 nblocks(static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(N-2, j_block_sz)), \
               static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(N-2, i_block_sz)), \
               static_cast<size_t>(1));


template < size_t j_block_size, size_t i_block_size >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_jacobi_2D_1(Real_ptr A, Real_ptr B, Index_type N)
{
  Index_type i = 1 + blockIdx.y * i_block_size + threadIdx.y;
  Index_type j = 1 + blockIdx.x * j_block_size + threadIdx.x;

  if ( i < N-1 && j < N-1 ) {
    POLYBENCH_JACOBI_2D_BODY1;
  }
}

template < size_t j_block_size, size_t i_block_size >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_jacobi_2D_2(Real_ptr A, Real_ptr B, Index_type N)
{
  Index_type i = 1 + blockIdx.y * i_block_size + threadIdx.y;
  Index_type j = 1 + blockIdx.x * j_block_size + threadIdx.x;

  if ( i < N-1 && j < N-1 ) {
    POLYBENCH_JACOBI_2D_BODY2;
  }
}

template < size_t j_block_size, size_t i_block_size, typename Lambda >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_jacobi_2D_lam(Index_type N, Lambda body)
{
  Index_type i = 1 + blockIdx.y * i_block_size + threadIdx.y;
  Index_type j = 1 + blockIdx.x * j_block_size + threadIdx.x;

  if ( i < N-1 && j < N-1 ) {
    body(i, j);
  }
}





template < size_t j_block_size, size_t i_block_size >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_jacobi_2D_reorder_1(Real_ptr A, Real_ptr B, Index_type N)
{
  Index_type i = 1 + (gridDim.z*blockIdx.x + blockIdx.z) * i_block_size + threadIdx.y;
  Index_type j = 1 + blockIdx.y * j_block_size + threadIdx.x;

  if ( i < N-1 && j < N-1 ) {
    POLYBENCH_JACOBI_2D_BODY1;
  }
}

template < size_t j_block_size, size_t i_block_size >
__launch_bounds__(j_block_size*i_block_size)
__global__ void poly_jacobi_2D_reorder_2(Real_ptr A, Real_ptr B, Index_type N)
{
  Index_type i = 1 + (gridDim.z*blockIdx.x + blockIdx.z) * i_block_size + threadIdx.y;
  Index_type j = 1 + blockIdx.y * j_block_size + threadIdx.x;

  if ( i < N-1 && j < N-1 ) {
    POLYBENCH_JACOBI_2D_BODY2;
  }
}


template < size_t block_size, size_t reorder_num >
void POLYBENCH_JACOBI_2D::runHipVariantReorder(VariantID vid)
{
  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  POLYBENCH_JACOBI_2D_DATA_SETUP;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      for (Index_type t = 0; t < tsteps; ++t) {

        dim3 nthreads_per_block(JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP, 1);
        int blocks_i = static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(N-2, i_block_sz));
        dim3 nblocks(reorder_num,
                     static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(N-2, j_block_sz)),
                     static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(blocks_i, reorder_num)));
        constexpr size_t shmem = 0;

        RPlaunchHipKernel(
          (poly_jacobi_2D_reorder_1<j_block_sz, i_block_sz>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          A, B, N );

        RPlaunchHipKernel(
          (poly_jacobi_2D_reorder_2<j_block_sz, i_block_sz>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          A, B, N );

      }

    }
    stopTimer();

  } else {
      getCout() << "\n  POLYBENCH_JACOBI_2D : Unknown Hip variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void POLYBENCH_JACOBI_2D::runHipVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  POLYBENCH_JACOBI_2D_DATA_SETUP;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      for (Index_type t = 0; t < tsteps; ++t) {

        JACOBI_2D_THREADS_PER_BLOCK_HIP;
        JACOBI_2D_NBLOCKS_HIP;
        constexpr size_t shmem = 0;

        RPlaunchHipKernel(
          (poly_jacobi_2D_1<JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          A, B, N );

        RPlaunchHipKernel(
          (poly_jacobi_2D_2<JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          A, B, N );

      }

    }
    stopTimer();

  } else if ( vid == Lambda_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      for (Index_type t = 0; t < tsteps; ++t) {

        JACOBI_2D_THREADS_PER_BLOCK_HIP;
        JACOBI_2D_NBLOCKS_HIP;
        constexpr size_t shmem = 0;

        auto poly_jacobi_2D_1_lambda = [=] __device__ (Index_type i,
                                                       Index_type j) {
          POLYBENCH_JACOBI_2D_BODY1;
        };

        RPlaunchHipKernel(
          (poly_jacobi_2D_lam<JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP,
                              decltype(poly_jacobi_2D_1_lambda)>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          N, poly_jacobi_2D_1_lambda );

        auto poly_jacobi_2D_2_lambda = [=] __device__ (Index_type i,
                                                       Index_type j) {
          POLYBENCH_JACOBI_2D_BODY2;
        };

        RPlaunchHipKernel(
          (poly_jacobi_2D_lam<JACOBI_2D_THREADS_PER_BLOCK_TEMPLATE_PARAMS_HIP,
                              decltype(poly_jacobi_2D_2_lambda)>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          N, poly_jacobi_2D_2_lambda );

      }

    }
    stopTimer();

  } else if (vid == RAJA_HIP) {

    POLYBENCH_JACOBI_2D_VIEWS_RAJA;

    using EXEC_POL =
      RAJA::KernelPolicy<
        RAJA::statement::HipKernelFixedAsync<i_block_sz * j_block_sz,
          RAJA::statement::For<0, RAJA::hip_global_size_y_direct<i_block_sz>,   // i
            RAJA::statement::For<1, RAJA::hip_global_size_x_direct<j_block_sz>, // j
              RAJA::statement::Lambda<0>
            >
          >
        >
      >;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      for (Index_type t = 0; t < tsteps; ++t) {

        RAJA::kernel_resource<EXEC_POL>(
          RAJA::make_tuple(RAJA::RangeSegment{1, N-1},
                           RAJA::RangeSegment{1, N-1}),
          res,
          [=] __device__ (Index_type i, Index_type j) {
            POLYBENCH_JACOBI_2D_BODY1_RAJA;
          }
        );

        RAJA::kernel_resource<EXEC_POL>(
          RAJA::make_tuple(RAJA::RangeSegment{1, N-1},
                           RAJA::RangeSegment{1, N-1}),
          res,
          [=] __device__ (Index_type i, Index_type j) {
            POLYBENCH_JACOBI_2D_BODY2_RAJA;
          }
        );

      }

    }
    stopTimer();

  } else {
      getCout() << "\n  POLYBENCH_JACOBI_2D : Unknown Hip variant id = " << vid << std::endl;
  }
}


void POLYBENCH_JACOBI_2D::runHipVariant(VariantID vid, size_t tune_idx)
{
  size_t t = 0;

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (tune_idx == t) {
        setBlockSize(block_size);
        runHipVariantImpl<block_size>(vid);

      }

      t += 1;

    }

  });

  if (vid == Base_HIP) {

    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 1>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 2>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 3>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 4>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 6>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantReorder<block_size, 8>(vid);

        }

        t += 1;

      }

    });

  }
}

void POLYBENCH_JACOBI_2D::setHipTuningDefinitions(VariantID vid)
{
  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      addVariantTuningName(vid, "block_"+std::to_string(block_size));

    }

  });

  if (vid == Base_HIP) {

    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        addVariantTuningName(vid, "reorder1_"+std::to_string(block_size));
        addVariantTuningName(vid, "reorder2_"+std::to_string(block_size));
        addVariantTuningName(vid, "reorder3_"+std::to_string(block_size));
        addVariantTuningName(vid, "reorder4_"+std::to_string(block_size));
        addVariantTuningName(vid, "reorder6_"+std::to_string(block_size));
        addVariantTuningName(vid, "reorder8_"+std::to_string(block_size));

      }

    });

  }
}

} // end namespace polybench
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP

