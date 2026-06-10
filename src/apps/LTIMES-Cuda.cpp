//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

using namespace ltimes_idx;

//
// Define thread block shape for CUDA execution
//
#define m_block_sz (block_size)
#define g_block_sz (1)
#define z_block_sz (1)

#define LTIMES_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA \
  m_block_sz, g_block_sz, z_block_sz

#define LTIMES_THREADS_PER_BLOCK_CUDA \
  dim3 nthreads_per_block(LTIMES_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA); \
  static_assert(m_block_sz*g_block_sz*z_block_sz == block_size, "Invalid block_size");

#define LTIMES_NBLOCKS_CUDA \
  dim3 nblocks(static_cast<size_t>(*num_z), \
               static_cast<size_t>(*num_g), \
               1);


template < size_t block_size >
__launch_bounds__(block_size)
__global__ void ltimes(PHI_VIEW phi, ELL_VIEW ell, PSI_VIEW psi,
                       ID num_d, IM num_m, IG num_g, IZ num_z)
{
   IM m(threadIdx.x);
   IG g(blockIdx.y);
   IZ z(blockIdx.x);

   if (m < num_m && g < num_g && z < num_z) {
     for (ID d(0); d < num_d; ++d ) {
       LTIMES_BODY;
     }
   }
}

template < size_t block_size, typename Lambda >
__launch_bounds__(block_size)
__global__ void ltimes_lam(IM num_m, IG num_g, IZ num_z,
                           Lambda body)
{
   IM m(threadIdx.x);
   IG g(blockIdx.y);
   IZ z(blockIdx.x);

   if (m < num_m && g < num_g && z < num_z) {
     body(z, g, m);
   }
}


template < size_t block_size, size_t tune_idx >
void LTIMES::runCudaVariantImpl(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  LTIMES_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      LTIMES_THREADS_PER_BLOCK_CUDA;
      LTIMES_NBLOCKS_CUDA;
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel(
        (ltimes<block_size>),
        nblocks, nthreads_per_block,
        shmem, res.get_stream(),
        phi, ell, psi,
        num_d, num_m, num_g, num_z );

    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      auto ltimes_lambda = [=] __device__ (IZ z, IG g, IM m) {
        for (ID d(0); d < num_d; ++d ) {
          LTIMES_BODY;
        }
      };

      LTIMES_THREADS_PER_BLOCK_CUDA;
      LTIMES_NBLOCKS_CUDA;
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel(
        (ltimes_lam<block_size, decltype(ltimes_lambda)>),
        nblocks, nthreads_per_block,
        shmem, res.get_stream(),
        num_m, num_g, num_z,
        ltimes_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    if constexpr (tune_idx == 0) {

      using EXEC_POL =
        RAJA::KernelPolicy<
          RAJA::statement::CudaKernelAsync<
            RAJA::statement::For<1, RAJA::cuda_block_x_loop, // z
              RAJA::statement::For<2, RAJA::cuda_block_y_loop, // g
                RAJA::statement::For<3, RAJA::cuda_thread_x_loop, // m
                  RAJA::statement::For<0, RAJA::seq_exec,           //d
                    RAJA::statement::Lambda<0>
                  >
                >
              >
            >
          >
        >;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::kernel_resource<EXEC_POL>(
          RAJA::make_tuple(IDRange(0, *num_d),
                           IZRange(0, *num_z),
                           IGRange(0, *num_g),
                           IMRange(0, *num_m)),
          res,
          [=] __device__ (ID d, IZ z, IG g, IM m) {
            LTIMES_BODY;
          }
        );

      }
      stopTimer();

    } else if constexpr (tune_idx == 1) {

      constexpr bool async = true;

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, block_size>>;

      using z_policy = RAJA::LoopPolicy<RAJA::cuda_block_x_loop>;

      using g_policy = RAJA::LoopPolicy<RAJA::cuda_block_y_loop>;

      using m_policy = RAJA::LoopPolicy<RAJA::cuda_thread_x_loop>;

      using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(RAJA::Teams(*num_z, *num_g, 1),
                               RAJA::Threads(block_size, 1, 1)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<z_policy>(ctx, IZRange(0, *num_z),
                [&](IZ z) {
                  RAJA::loop<g_policy>(ctx, IGRange(0, *num_g),
                    [&](IG g) {
                      RAJA::loop<m_policy>(ctx, IMRange(0, *num_m),
                        [&](IM m) {
                          RAJA::loop<d_policy>(ctx, IDRange(0, *num_d),
                            [&](ID d) {
                              LTIMES_BODY
                            }
                          ); // RAJA::loop<d_policy>
                        }
                      ); // RAJA::loop<m_policy>
                    }
                  ); // RAJA::loop<g_policy>
                }
              ); // RAJA::loop<z_policy>

            } // outer lambda (ctx)
        );    // RAJA::launch

      } // loop over kernel reps
      stopTimer();
    }

  } else {
     getCout() << "\n LTIMES : Unknown Cuda variant id = " << vid << std::endl;
  }
}


void LTIMES::defineCudaVariantTunings()
{

  for (VariantID vid : {Base_CUDA, Lambda_CUDA, RAJA_CUDA}) {

    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        if (vid == RAJA_CUDA) {
          addVariantTuning<&LTIMES::runCudaVariantImpl<block_size, 0>>(
              vid, "kernel_"+std::to_string(block_size));
          addVariantTuning<&LTIMES::runCudaVariantImpl<block_size, 1>>(
              vid, "launch_"+std::to_string(block_size));
        } else {
          addVariantTuning<&LTIMES::runCudaVariantImpl<block_size, 0>>(
              vid, "block_"+std::to_string(block_size));
        }

      }

    });

  }

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
