//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives loop unrolling
//#define USE_RAJAPERF_UNROLL

// Uncomment to use direct policies
#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void MassVec3DPA_BLOCKDIM_LOOP_BOUNDS(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const int e = hipBlockIdx_x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D(q, d, MVPA_Q1D, MVPA_D1D) {
    MASSVEC3DPA_1;
  }

  for (int c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void MassVec3DPA_RUNTIME_LOOP_BOUNDS(const Real_ptr B, const Real_ptr Bt,
                const Real_ptr D, const Real_ptr X, Real_ptr Y, Index_type runtime_block_size) {

  const int e = hipBlockIdx_x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, MVPA_Q1D, MVPA_D1D, runtime_block_size) {
    MASSVEC3DPA_1;
  }

  for (int c = 0; c < 3; ++c) {
   GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D, runtime_block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D, runtime_block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D, runtime_block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D, runtime_block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D, runtime_block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D, runtime_block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D, runtime_block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void MassVec3DPA_COMPILE_LOOP_BOUNDS(const Real_ptr B, const Real_ptr Bt,
                const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const int e = hipBlockIdx_x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, MVPA_Q1D, MVPA_D1D, block_size) {
    MASSVEC3DPA_1;
  }

  for (int c = 0; c < 3; ++c) {
   GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D, block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D, block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D, block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D, block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D, block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D, block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D, block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void MassVec3DPA_DIRECT(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const int e = hipBlockIdx_x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_DIRECT_2D(q, d, MVPA_Q1D, MVPA_D1D) {
    MASSVEC3DPA_1;
  }

  for (int c = 0; c < 3; ++c) {
    GPU_SHARED_DIRECT_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template < size_t block_size >
void MASSVEC3DPA::runHipVariantImpl(VariantID vid, size_t tune_idx) {
  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_HIP: {

  if(tune_idx == 0) {
     std::cout<<"MassVec3DPA_BLOCKDIM_LOOP_BOUNDS "<<std::endl;
    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (MassVec3DPA_BLOCKDIM_LOOP_BOUNDS<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, Bt, D, X, Y );
    }
    stopTimer();

  //Loop constants
  } else if (tune_idx == 1) {

  std::cout<<"MassVec3DPA_RUNTIME_LOOP_BOUNDS "<<std::endl;
    //Mark volatile because we want the value to be treated as a runtime value
    volatile Index_type runtime_loop_bounds = MVPA_Q1D;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (MassVec3DPA_RUNTIME_LOOP_BOUNDS<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, Bt, D, X, Y, runtime_loop_bounds );
    }
    stopTimer();

  } else if (tune_idx == 2) {

  std::cout<<"MassVec3DPA_COMPILE_LOOP_BOUNDS "<<std::endl;
    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (MassVec3DPA_COMPILE_LOOP_BOUNDS<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, Bt, D, X, Y );
    }
    stopTimer();

  } else if (tune_idx == 3) {

    std::cout<<"MassVec3DPA_DIRECT "<<std::endl;
    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (MassVec3DPA_DIRECT<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, Bt, D, X, Y);
    }
    stopTimer();

  }

    break;
  }

  case RAJA_HIP: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::hip_launch_t<async, MVPA_Q1D*MVPA_Q1D*MVPA_Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::hip_block_x_direct>;

    /*Compile time loop bounds */
    /*
#if defined(USE_DIRECT)
    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_size_x_direct<MVPA_Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_size_y_direct<MVPA_Q1D>>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_size_z_direct<MVPA_Q1D>>;
#else
    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_size_x_loop<MVPA_Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_size_y_loop<MVPA_Q1D>>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_size_z_loop<MVPA_Q1D>>;
#endif
    */

#if defined(USE_DIRECT)
    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_x_direct>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_y_direct>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_z_direct>;
#else
    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_x_loop>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_y_loop>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_z_loop>;
#endif

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

            MASSVEC3DPA_0_GPU

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1), [&](int ) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D), [&](int d) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D), [&](int q) {
                  MASSVEC3DPA_1;
                });
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

//RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASSVEC3DPA, Hip)

void MASSVEC3DPA::runHipVariant(VariantID vid, size_t tune_idx)
{

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    setBlockSize(block_size);
    runHipVariantImpl<block_size>(vid, tune_idx);
  });

}

void MASSVEC3DPA::setHipTuningDefinitions(VariantID vid)
{

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (vid == RAJA_HIP) {
        addVariantTuningName(vid, "BLOCKDIM_LOOP_BOUNDS");
        addVariantTuningName(vid, "RUNTIME_LOOP_BOUNDS");
        addVariantTuningName(vid, "COMPILE_LOOP_BOUNDS");
        addVariantTuningName(vid, "DIRECT");
      }

      if(vid == Base_HIP) {
        addVariantTuningName(vid, "BLOCKDIM_LOOP_BOUNDS");
        addVariantTuningName(vid, "RUNTIME_LOOP_BOUNDS");
        addVariantTuningName(vid, "COMPILE_LOOP_BOUNDS");
        addVariantTuningName(vid, "DIRECT");
      }

    }

  });

}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_HIP
