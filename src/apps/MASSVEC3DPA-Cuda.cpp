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

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void Mass3DPA(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const int e = blockIdx.x;

  MASSVEC3DPA_0_GPU

  GPU_FOREACH_THREAD(dy, y, MVPA_D1D) {
    GPU_FOREACH_THREAD(dx, x, MVPA_D1D){
      MASSVEC3DPA_1
    }
    GPU_FOREACH_THREAD(dx, x, MVPA_Q1D) {
      MASSVEC3DPA_2
    }
  }
  __syncthreads();
  GPU_FOREACH_THREAD(dy, y, MVPA_D1D) {
    GPU_FOREACH_THREAD(qx, x, MVPA_Q1D) {
      MASSVEC3DPA_3
    }
  }
  __syncthreads();
  GPU_FOREACH_THREAD(qy, y, MVPA_Q1D) {
    GPU_FOREACH_THREAD(qx, x, MVPA_Q1D) {
      MASSVEC3DPA_4
    }
  }
  __syncthreads();
  GPU_FOREACH_THREAD(qy, y, MVPA_Q1D) {
    GPU_FOREACH_THREAD(qx, x, MVPA_Q1D) {
      MASSVEC3DPA_5
    }
  }

  __syncthreads();
  GPU_FOREACH_THREAD(d, y, MVPA_D1D) {
    GPU_FOREACH_THREAD(q, x, MVPA_Q1D) {
      MASSVEC3DPA_6
    }
  }

  __syncthreads();
  GPU_FOREACH_THREAD(qy, y, MVPA_Q1D) {
    GPU_FOREACH_THREAD(dx, x, MVPA_D1D) {
      MASSVEC3DPA_7
    }
  }
  __syncthreads();

  GPU_FOREACH_THREAD(dy, y, MVPA_D1D) {
    GPU_FOREACH_THREAD(dx, x, MVPA_D1D) {
      MASSVEC3DPA_8
    }
  }

  __syncthreads();
  GPU_FOREACH_THREAD(dy, y, MVPA_D1D) {
    GPU_FOREACH_THREAD(dx, x, MVPA_D1D) {
      MASSVEC3DPA_9
    }
  }
}

template < size_t block_size >
void MASSVEC3DPA::runCudaVariantImpl(VariantID vid) {
  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_CUDA: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, 1);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (Mass3DPA<block_size>),
                          NE, nthreads_per_block,
                          shmem, res.get_stream(),
                          B, Bt, D, X, Y );
    }
    stopTimer();

    break;
  }

  case RAJA_CUDA: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, MVPA_Q1D*MVPA_Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_size_x_loop<MVPA_Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_size_y_loop<MVPA_Q1D>>;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(MVPA_Q1D, MVPA_Q1D, 1)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

              MASSVEC3DPA_0_GPU

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&](int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                    [&](int dx) {
                      MASSVEC3DPA_1
                    }
                  );  // RAJA::loop<inner_x>

                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&](int dx) {
                      MASSVEC3DPA_2
                    }
                  );  // RAJA::loop<inner_x>
                }  // lambda (dy)
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&](int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&](int qx) {
                      MASSVEC3DPA_3
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&](int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&](int qx) {
                      MASSVEC3DPA_4
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&](int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&](int qx) {
                      MASSVEC3DPA_5
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&](int d) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&](int q) {
                      MASSVEC3DPA_6
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&](int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                    [&](int dx) {
                      MASSVEC3DPA_7
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&](int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                    [&](int dx) {
                      MASSVEC3DPA_8
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&](int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                    [&](int dx) {
                      MASSVEC3DPA_9
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASSVEC3DPA : Unknown Cuda variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASSVEC3DPA, Cuda)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
