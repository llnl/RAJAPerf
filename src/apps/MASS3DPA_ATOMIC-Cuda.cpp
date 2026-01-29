//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASS3DPA_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void Mass3DPA_Atomic(const Real_ptr B,
                                const Real_ptr D, const Real_ptr X, const Index_ptr ElemToDoF, Real_ptr Y) {

  const Index_type e = blockIdx.x;

  MASS3DPA_ATOMIC_0_GPU;


  GPU_FOREACH_THREAD_DIRECT(dz, z, mpa_at::D1D) {
    GPU_FOREACH_THREAD_DIRECT(dy, y, mpa_at::D1D) {
      GPU_FOREACH_THREAD_DIRECT(dx, x, mpa_at::D1D) {
        MASS3DPA_ATOMIC_1;
      }
    }
  }

  GPU_FOREACH_THREAD_DIRECT(dz, z, 1) {
    GPU_FOREACH_THREAD_DIRECT(d, y, mpa_at::D1D) {
      GPU_FOREACH_THREAD_DIRECT(q, x, mpa_at::Q1D) {
        MASS3DPA_ATOMIC_2;
      }
    }
  }


  GPU_FOREACH_THREAD_DIRECT(dz, z, mpa_at::D1D) {
    GPU_FOREACH_THREAD_DIRECT(dy, y, mpa_at::D1D) {
      GPU_FOREACH_THREAD_DIRECT(qx, x, mpa_at::Q1D) {
        MASS3DPA_ATOMIC_3;
      }
    }
  }


  GPU_FOREACH_THREAD_DIRECT(dz, z, mpa_at::D1D) {
    GPU_FOREACH_THREAD_DIRECT(qy, y, mpa_at::Q1D) {
      GPU_FOREACH_THREAD_DIRECT(qx, x, mpa_at::Q1D) {
      MASS3DPA_ATOMIC_4;
      }
    }
  }

  GPU_FOREACH_THREAD_DIRECT(qz, z, mpa_at::Q1D) {
    GPU_FOREACH_THREAD_DIRECT(qy, y, mpa_at::Q1D) {
      GPU_FOREACH_THREAD_DIRECT(qx, x, mpa_at::Q1D) {
        MASS3DPA_ATOMIC_5;
      }
    }
  }

  GPU_FOREACH_THREAD_DIRECT(qz, z, mpa_at::Q1D) {
    GPU_FOREACH_THREAD_DIRECT(qy, y, mpa_at::Q1D) {
      GPU_FOREACH_THREAD_DIRECT(dx, x, mpa_at::D1D) {
      MASS3DPA_ATOMIC_6;
      }
    }
  }

  GPU_FOREACH_THREAD_DIRECT(qz, z, mpa_at::Q1D) {
    GPU_FOREACH_THREAD_DIRECT(dy, y, mpa_at::D1D) {
      GPU_FOREACH_THREAD_DIRECT(dx, x, mpa_at::D1D) {
        MASS3DPA_ATOMIC_7;
      }
    }
  }

  GPU_FOREACH_THREAD_DIRECT(dz, z, mpa_at::D1D) {
    GPU_FOREACH_THREAD_DIRECT(dy, y, mpa_at::D1D) {
      GPU_FOREACH_THREAD_DIRECT(dx, x, mpa_at::D1D) {
      MASS3DPA_ATOMIC_8;
      MASS3DPA_ATOMIC_9(RAJAPERF_ATOMIC_ADD_CUDA);
      }
    }
  }

}

template < size_t block_size >
void MASS3DPA_ATOMIC::runCudaVariantImpl(VariantID vid) {
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  MASS3DPA_ATOMIC_DATA_SETUP;

  switch (vid) {

  case Base_CUDA: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      dim3 nthreads_per_block(mpa_at::Q1D, mpa_at::Q1D, mpa_at::Q1D);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (Mass3DPA_Atomic<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, D, X, ElemToDoF, Y );

    }
    stopTimer();

    break;
  }

  case RAJA_CUDA: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_size_x_loop<mpa_at::Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_size_y_loop<mpa_at::Q1D>>;

    using inner_z = RAJA::LoopPolicy<RAJA::cuda_thread_size_z_loop<mpa_at::Q1D>>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      //clang-format off
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(mpa_at::Q1D, mpa_at::Q1D, mpa_at::Q1D)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {


            MASS3DPA_ATOMIC_0_GPU;

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                      [&](Index_type dx) {
                      MASS3DPA_ATOMIC_1;
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>


            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
              [&](Index_type ) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                  [&](Index_type d) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                      [&](Index_type q) {
                      MASS3DPA_ATOMIC_2;
                      } // lambda (q)
                    ); // RAJA::loop<inner_x>
                  } // lambda (d)
                ); // RAJA::loop<inner_y>
              } // lambda ()
            ); // RAJA::loop<inner_z>


            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                      [&](Index_type qx) {
                      MASS3DPA_ATOMIC_3;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                      [&](Index_type qx) {
                      MASS3DPA_ATOMIC_4;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                      [&](Index_type qx) {
                      MASS3DPA_ATOMIC_5;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (qz)
            ); // RAJA::loop<inner_z>

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                      [&](Index_type dx) {
                      MASS3DPA_ATOMIC_6;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                      [&](Index_type dx) {
                      MASS3DPA_ATOMIC_7;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>


            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mpa_at::D1D),
                      [&](Index_type dx) {
                      MASS3DPA_ATOMIC_8;
                      MASS3DPA_ATOMIC_9(RAJAPERF_ATOMIC_ADD_RAJA_CUDA);
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch
      //clang-format on

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASS3DPA_ATOMIC : Unknown Cuda variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASS3DPA_ATOMIC, Cuda, Base_CUDA, RAJA_CUDA)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
