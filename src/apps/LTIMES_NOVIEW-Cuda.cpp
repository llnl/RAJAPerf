//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES_NOVIEW.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

//
// Define thread block shape for CUDA execution
//
#define block_moments_m_block_sz (block_size)
#define block_moments_g_block_sz (1)
#define block_moments_z_block_sz (1)

#define LTIMES_NOVIEW_BLOCK_MOMENTS_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA \
  block_moments_m_block_sz, block_moments_g_block_sz, block_moments_z_block_sz

#define LTIMES_NOVIEW_BLOCK_MOMENTS_THREADS_PER_BLOCK_CUDA \
  dim3 nthreads_per_block(LTIMES_NOVIEW_BLOCK_MOMENTS_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA); \
  static_assert(block_moments_m_block_sz*block_moments_g_block_sz*block_moments_z_block_sz == block_size, "Invalid block_size");

#define LTIMES_NOVIEW_BLOCK_MOMENTS_NBLOCKS_CUDA \
  dim3 nblocks(static_cast<size_t>(num_z), \
               static_cast<size_t>(num_g), \
               1);

#define factorized_m_block_sz (32)
#define factorized_g_block_sz (integer::greater_of_squarest_factor_pair(block_size/factorized_m_block_sz))
#define factorized_z_block_sz (integer::lesser_of_squarest_factor_pair(block_size/factorized_m_block_sz))

#define LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA \
  factorized_m_block_sz, factorized_g_block_sz, factorized_z_block_sz

#define LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_CUDA \
  dim3 nthreads_per_block(LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA); \
  static_assert(factorized_m_block_sz*factorized_g_block_sz*factorized_z_block_sz == block_size, "Invalid block_size");

#define LTIMES_NOVIEW_FACTORIZED_NBLOCKS_CUDA \
  dim3 nblocks(static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_m, factorized_m_block_sz)), \
               static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_g, factorized_g_block_sz)), \
               static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_z, factorized_z_block_sz)));


template < size_t block_size >
__launch_bounds__(block_size)
__global__ void ltimes_noview_block_moments(Real_ptr phidat, Real_ptr elldat, Real_ptr psidat,
                              Index_type num_d,
                              Index_type num_m, Index_type num_g, Index_type num_z)
{
   Index_type g = blockIdx.y;
   Index_type z = blockIdx.x;

   if (g < num_g && z < num_z) {
     for (Index_type m = threadIdx.x; m < num_m; m += block_size) {
       for (Index_type d = 0; d < num_d; ++d ) {
         LTIMES_NOVIEW_BODY;
       }
     }
   }
}

template < size_t m_block_size, size_t g_block_size, size_t z_block_size >
__launch_bounds__(m_block_size*g_block_size*z_block_size)
__global__ void ltimes_noview_factorized(Real_ptr phidat, Real_ptr elldat, Real_ptr psidat,
                                  Index_type num_d,
                                  Index_type num_m, Index_type num_g, Index_type num_z)
{
   Index_type m = blockIdx.x * m_block_size + threadIdx.x;
   Index_type g = blockIdx.y * g_block_size + threadIdx.y;
   Index_type z = blockIdx.z * z_block_size + threadIdx.z;

   if (m < num_m && g < num_g && z < num_z) {
     for (Index_type d = 0; d < num_d; ++d ) {
       LTIMES_NOVIEW_BODY;
     }
   }
}

template < size_t block_size, typename Lambda >
__launch_bounds__(block_size)
__global__ void ltimes_noview_lam_block_moments(Index_type num_m, Index_type num_g, Index_type num_z,
                                  Lambda body)
{
   Index_type g = blockIdx.y;
   Index_type z = blockIdx.x;

   if (g < num_g && z < num_z) {
     for (Index_type m = threadIdx.x; m < num_m; m += block_size) {
       body(z, g, m);
     }
   }
}

template < size_t m_block_size, size_t g_block_size, size_t z_block_size, typename Lambda >
__launch_bounds__(m_block_size*g_block_size*z_block_size)
__global__ void ltimes_noview_lam_factorized(Index_type num_m, Index_type num_g, Index_type num_z,
                                      Lambda body)
{
   Index_type m = blockIdx.x * m_block_size + threadIdx.x;
   Index_type g = blockIdx.y * g_block_size + threadIdx.y;
   Index_type z = blockIdx.z * z_block_size + threadIdx.z;

   if (m < num_m && g < num_g && z < num_z) {
     body(z, g, m);
   }
}


template < size_t block_size, size_t tune_idx >
void LTIMES_NOVIEW::runCudaVariantImpl(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  LTIMES_NOVIEW_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      constexpr size_t shmem = 0;

      if constexpr (tune_idx == 1) {
        LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_CUDA;
        LTIMES_NOVIEW_FACTORIZED_NBLOCKS_CUDA;

        RPlaunchCudaKernel(
          (ltimes_noview_factorized<LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          phidat, elldat, psidat,
          num_d, num_m, num_g, num_z );
      } else {
        LTIMES_NOVIEW_BLOCK_MOMENTS_THREADS_PER_BLOCK_CUDA;
        LTIMES_NOVIEW_BLOCK_MOMENTS_NBLOCKS_CUDA;

        RPlaunchCudaKernel(
          (ltimes_noview_block_moments<block_size>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          phidat, elldat, psidat,
          num_d, num_m, num_g, num_z );
      }

    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      auto ltimes_noview_lambda = 
        [=] __device__ (Index_type z, Index_type g, Index_type m) {
          for (Index_type d = 0; d < num_d; ++d ) {
            LTIMES_NOVIEW_BODY;
          }
        }; 

      constexpr size_t shmem = 0;

      if constexpr (tune_idx == 1) {
        LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_CUDA;
        LTIMES_NOVIEW_FACTORIZED_NBLOCKS_CUDA;

        RPlaunchCudaKernel(
          (ltimes_noview_lam_factorized<LTIMES_NOVIEW_FACTORIZED_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA,
                                 decltype(ltimes_noview_lambda)>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          num_m, num_g, num_z,
          ltimes_noview_lambda );
      } else {
        LTIMES_NOVIEW_BLOCK_MOMENTS_THREADS_PER_BLOCK_CUDA;
        LTIMES_NOVIEW_BLOCK_MOMENTS_NBLOCKS_CUDA;

        RPlaunchCudaKernel(
          (ltimes_noview_lam_block_moments<block_size, decltype(ltimes_noview_lambda)>),
          nblocks, nthreads_per_block,
          shmem, res.get_stream(),
          num_m, num_g, num_z,
          ltimes_noview_lambda );
      }

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
          RAJA::make_tuple(RAJA::RangeSegment(0, num_d),
                           RAJA::RangeSegment(0, num_z),
                           RAJA::RangeSegment(0, num_g),
                           RAJA::RangeSegment(0, num_m)),
          res,
          [=] __device__ (Index_type d, Index_type z,
                          Index_type g, Index_type m) {
            LTIMES_NOVIEW_BODY;
          }
        );

      }
      stopTimer();

    } else if constexpr (tune_idx == 1) {

      using EXEC_POL =
        RAJA::KernelPolicy<
          RAJA::statement::CudaKernelFixedAsync<factorized_m_block_sz*factorized_g_block_sz*factorized_z_block_sz,
            RAJA::statement::For<1, RAJA::cuda_global_size_z_direct<factorized_z_block_sz>,     // z
              RAJA::statement::For<2, RAJA::cuda_global_size_y_direct<factorized_g_block_sz>,   // g
                RAJA::statement::For<3, RAJA::cuda_global_size_x_direct<factorized_m_block_sz>, // m
                  RAJA::statement::For<0, RAJA::seq_exec,                                // d
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
          RAJA::make_tuple(RAJA::RangeSegment(0, num_d),
                           RAJA::RangeSegment(0, num_z),
                           RAJA::RangeSegment(0, num_g),
                           RAJA::RangeSegment(0, num_m)),
          res,
          [=] __device__ (Index_type d, Index_type z,
                          Index_type g, Index_type m) {
            LTIMES_NOVIEW_BODY;
          }
        );

      }
      stopTimer();

    } else if constexpr (tune_idx == 2) {

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
            RAJA::LaunchParams(RAJA::Teams(num_z, num_g, 1),
                               RAJA::Threads(block_size, 1, 1)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<z_policy>(ctx, RAJA::RangeSegment(0, num_z),
                [&](Index_type z) {
                  RAJA::loop<g_policy>(ctx, RAJA::RangeSegment(0, num_g),
                    [&](Index_type g) {
                      RAJA::loop<m_policy>(ctx, RAJA::RangeSegment(0, num_m),
                        [&](Index_type m) {
                          RAJA::loop<d_policy>(ctx, RAJA::RangeSegment(0, num_d),
                            [&](Index_type d) {
                              LTIMES_NOVIEW_BODY
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
    } else if constexpr (tune_idx == 3) {

      constexpr bool async = true;

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, factorized_m_block_sz*factorized_g_block_sz*factorized_z_block_sz>>;

      using z_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_z_loop<factorized_z_block_sz>>;

      using g_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_y_loop<factorized_g_block_sz>>;

      using m_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_x_loop<factorized_m_block_sz>>;

      using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

      const size_t z_grid_sz = RAJA_DIVIDE_CEILING_INT(num_z, factorized_z_block_sz);

      const size_t g_grid_sz = RAJA_DIVIDE_CEILING_INT(num_g, factorized_g_block_sz);

      const size_t m_grid_sz = RAJA_DIVIDE_CEILING_INT(num_m, factorized_m_block_sz);

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(RAJA::Teams(m_grid_sz, g_grid_sz, z_grid_sz),
                               RAJA::Threads(factorized_m_block_sz, factorized_g_block_sz, factorized_z_block_sz)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<z_policy>(ctx, RAJA::RangeSegment(0, num_z),
                [&](Index_type z) {
                  RAJA::loop<g_policy>(ctx, RAJA::RangeSegment(0, num_g),
                    [&](Index_type g) {
                      RAJA::loop<m_policy>(ctx, RAJA::RangeSegment(0, num_m),
                        [&](Index_type m) {
                          RAJA::loop<d_policy>(ctx, RAJA::RangeSegment(0, num_d),
                            [&](Index_type d) {
                              LTIMES_NOVIEW_BODY
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
     getCout() << "\n LTIMES_NOVIEW : Unknown Cuda variant id = " << vid << std::endl;
  }
}


void LTIMES_NOVIEW::defineCudaVariantTunings()
{

  for (VariantID vid : {Base_CUDA, Lambda_CUDA, RAJA_CUDA}) {

    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        if (vid == RAJA_CUDA) {

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 0>>(
              vid, "kernel_block_m");

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 2>>(
              vid, "launch_block_m");

        } else {

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 0>>(
              vid, "block_m");

        }

      }

    });

  }

  for (VariantID vid : {Base_CUDA, Lambda_CUDA, RAJA_CUDA}) {

    seq_for(factorized_gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        if (vid == RAJA_CUDA) {

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 1>>(
              vid, "kernel_block_m32_gBigFact_zLowFact_"+std::to_string(block_size));

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 3>>(
              vid, "launch_block_m32_gBigFact_zLowFact_"+std::to_string(block_size));

        } else {

          addVariantTuning<&LTIMES_NOVIEW::runCudaVariantImpl<block_size, 1>>(
              vid, "block_m32_gBigFact_zLowFact_"+std::to_string(block_size));

        }

      }

    });

  }

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
