//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void MassVec3DPA(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const int e = hipBlockIdx_x;
  
  MASSVEC3DPA_0_GPU;
  
  GPU_SHARED_LOOP_2D_DIRECT(q, d, MVPA_Q1D, MVPA_D1D) {
    MASSVEC3DPA_1;
  }

  for (int c = 0; c < 3; ++c) {        
    GPU_SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_DIRECT(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_DIRECT(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();
          
    GPU_SHARED_LOOP_3D_DIRECT(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();
    
    GPU_SHARED_LOOP_3D_DIRECT(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();
          
    GPU_SHARED_LOOP_3D_DIRECT(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();
    
    GPU_SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();
    
  } // (c) dimension loop
}

template < size_t block_size >
void MASSVEC3DPA::runHipVariantImpl(VariantID vid) {
  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_HIP: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (MassVec3DPA<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, Bt, D, X, Y );

    }
    stopTimer();

    break;
  }

  case RAJA_HIP: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::hip_launch_t<async, MVPA_Q1D*MVPA_Q1D*MVPA_Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::hip_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_size_x_loop<MVPA_Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_size_y_loop<MVPA_Q1D>>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_size_z_loop<MVPA_Q1D>>;    

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

            MASSVEC3DPA_0_GPU
            RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int q) {
              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int d) {
                MASSVEC3DPA_1;
              });
            });

            for (int c = 0; c < 3; ++c) {

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dx) {
                  MASSVEC3DPA_2;
                });
              });
            });

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qx) {
                  MASSVEC3DPA_3;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qx) {
                MASSVEC3DPA_4;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qx) {

                MASSVEC3DPA_5;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dx) {

                MASSVEC3DPA_6;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dx) {

                MASSVEC3DPA_7;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int dx) {
                  MASSVEC3DPA_8;
                });
              });
            });
            ctx.teamSync();

            } //c - dim loop
            
            }  // lambda (e)
          );  // RAJA::loop<outer_x>
          
        }  // outer lambda (ctx)
      );  // RAJA::launch

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASSVEC3DPA : Unknown Hip variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASSVEC3DPA, Hip)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_HIP
