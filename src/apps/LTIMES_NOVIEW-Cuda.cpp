//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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
#define m_block_sz (32)
#define g_block_sz (integer::greater_of_squarest_factor_pair(block_size/m_block_sz))
#define z_block_sz (integer::lesser_of_squarest_factor_pair(block_size/m_block_sz))

#define LTIMES_NOVIEW_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA \
  m_block_sz, g_block_sz, z_block_sz

#define LTIMES_NOVIEW_THREADS_PER_BLOCK_CUDA \
  dim3 nthreads_per_block(LTIMES_NOVIEW_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA);

#define LTIMES_NOVIEW_NBLOCKS_CUDA \
  dim3 nblocks(static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_m, m_block_sz)), \
               static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_g, g_block_sz)), \
               static_cast<size_t>(RAJA_DIVIDE_CEILING_INT(num_z, z_block_sz)));


template < size_t m_block_size, size_t g_block_size, size_t z_block_size >
__launch_bounds__(m_block_size*g_block_size*z_block_size)
__global__ void ltimes_noview(Real_ptr phidat, Real_ptr elldat, Real_ptr psidat,
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

template < size_t m_block_size, size_t g_block_size, size_t z_block_size, typename Lambda >
__launch_bounds__(m_block_size*g_block_size*z_block_size)
__global__ void ltimes_noview_lam(Index_type num_m, Index_type num_g, Index_type num_z,
                                  Lambda body)
{
   Index_type m = blockIdx.x * m_block_size + threadIdx.x;
   Index_type g = blockIdx.y * g_block_size + threadIdx.y;
   Index_type z = blockIdx.z * z_block_size + threadIdx.z;

   if (m < num_m && g < num_g && z < num_z) {
     body(z, g, m);
   }
}


template < size_t block_size >
void LTIMES_NOVIEW::runCudaVariantImpl(VariantID vid, size_t tune_idx)
{
  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  LTIMES_NOVIEW_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      LTIMES_NOVIEW_THREADS_PER_BLOCK_CUDA;
      LTIMES_NOVIEW_NBLOCKS_CUDA;
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( 
        (ltimes_noview<LTIMES_NOVIEW_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA>),
        nblocks, nthreads_per_block,
        shmem, res.get_stream(),
        phidat, elldat, psidat,
        num_d, num_m, num_g, num_z );

    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      auto ltimes_noview_lambda = 
        [=] __device__ (Index_type z, Index_type g, Index_type m) {
          for (Index_type d = 0; d < num_d; ++d ) {
            LTIMES_NOVIEW_BODY;
          }
        }; 

      LTIMES_NOVIEW_THREADS_PER_BLOCK_CUDA;
      LTIMES_NOVIEW_NBLOCKS_CUDA;
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( 
        (ltimes_noview_lam<LTIMES_NOVIEW_THREADS_PER_BLOCK_TEMPLATE_PARAMS_CUDA,
                           decltype(ltimes_noview_lambda)>),
        nblocks, nthreads_per_block,
        shmem, res.get_stream(),
        num_m, num_g, num_z,
        ltimes_noview_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    if (tune_idx == 0) {

      using EXEC_POL =
        RAJA::KernelPolicy<
          RAJA::statement::CudaKernelFixedAsync<m_block_sz*g_block_sz*z_block_sz,
            RAJA::statement::For<1, RAJA::cuda_global_size_z_direct<z_block_sz>,     //z
              RAJA::statement::For<2, RAJA::cuda_global_size_y_direct<g_block_sz>,   //g
                RAJA::statement::For<3, RAJA::cuda_global_size_x_direct<m_block_sz>, //m
                  RAJA::statement::For<0, RAJA::seq_exec,           //d
                    RAJA::statement::Lambda<0>
                  >
                >
              >
            >
          >
        >;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

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

    } else if (tune_idx == 1) {

      constexpr bool async = true;

      using launch_policy = RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, m_block_sz*g_block_sz*z_block_sz>>;

      using z_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_z_loop<z_block_sz>>;

      using g_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_y_loop<g_block_sz>>;

      using m_policy = RAJA::LoopPolicy<RAJA::cuda_global_size_x_loop<m_block_sz>>;

      using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

      const size_t z_grid_sz = RAJA_DIVIDE_CEILING_INT(num_z, z_block_sz);

      const size_t g_grid_sz = RAJA_DIVIDE_CEILING_INT(num_g, g_block_sz);

      const size_t m_grid_sz = RAJA_DIVIDE_CEILING_INT(num_m, m_block_sz);

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(RAJA::Teams(m_grid_sz, g_grid_sz, z_grid_sz),
                               RAJA::Threads(m_block_sz, g_block_sz, z_block_sz)),
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

void LTIMES_NOVIEW::runCudaVariant(VariantID vid, size_t tune_idx)
{
  size_t t = 0;

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (vid == RAJA_CUDA) {

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantImpl<block_size>(vid, 0);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantImpl<block_size>(vid, 1);

        }

        t += 1;

      } else {

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantImpl<block_size>(vid, 0);

        }

        t += 1;
      }

    }

  });
}

void LTIMES_NOVIEW::setCudaTuningDefinitions(VariantID vid)
{

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (vid == RAJA_CUDA) {
        addVariantTuningName(vid, "kernel_"+std::to_string(block_size));
        addVariantTuningName(vid, "launch_"+std::to_string(block_size));
      } else {
        addVariantTuningName(vid, "block_"+std::to_string(block_size));
      }

    }

  });

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
