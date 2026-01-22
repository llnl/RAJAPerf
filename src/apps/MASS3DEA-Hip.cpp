//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DEA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void Mass3DEA(const Real_ptr B, const Real_ptr D, Real_ptr M) {

  const Index_type e = blockIdx.x;

  MASS3DEA_0

  GPU_FOREACH_THREAD(iz, z, 1) {
    GPU_FOREACH_THREAD(d, x, mea::D1D) {
      GPU_FOREACH_THREAD(q, y, mea::Q1D) {
        MASS3DEA_1
      }
    }
  }

  MASS3DEA_2

  GPU_FOREACH_THREAD(k1, x, mea::Q1D) {
    GPU_FOREACH_THREAD(k2, y, mea::Q1D) {
      GPU_FOREACH_THREAD(k3, z, mea::Q1D) {
        MASS3DEA_3
      }
    }
  }

  __syncthreads();

  GPU_FOREACH_THREAD(i1, x, mea::D1D) {
    GPU_FOREACH_THREAD(i2, y, mea::D1D) {
      GPU_FOREACH_THREAD(i3, z, mea::D1D) {
        MASS3DEA_4
      }
    }
  }

}

template < size_t block_size >
void MASS3DEA::runHipVariantImpl(VariantID vid) {
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  MASS3DEA_DATA_SETUP;

  switch (vid) {

  case Base_HIP: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      dim3 nthreads_per_block(mea::D1D, mea::D1D, mea::D1D);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (Mass3DEA<block_size>),
                         NE, nthreads_per_block,
                         shmem, res.get_stream(),
                         B, D, M );
    }
    stopTimer();

    break;
  }

  case RAJA_HIP: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::hip_launch_t<async, mea::D1D*mea::D1D*mea::D1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::hip_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_size_x_loop<mea::D1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_size_y_loop<mea::D1D>>;

    using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_size_z_loop<mea::D1D>>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      //clang-format off
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(mea::D1D, mea::D1D, mea::D1D)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

              MASS3DEA_0

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                [&](Index_type ) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mea::D1D),
                    [&](Index_type d) {
                      RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mea::Q1D),
                        [&](Index_type q) {
                          MASS3DEA_1
                        }
                      ); // RAJA::loop<inner_y>
                    }
                  ); // RAJA::loop<inner_x>
                }
              ); // RAJA::loop<inner_z>


              MASS3DEA_2

              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mea::Q1D),
                [&](Index_type k1) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mea::Q1D),
                    [&](Index_type k2) {
                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mea::Q1D),
                        [&](Index_type k3) {
                          MASS3DEA_3
                        }
                      ); // RAJA::loop<inner_x>
                    }
                  ); // RAJA::loop<inner_y>
                }
              ); // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mea::D1D),
                [&](Index_type i1) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mea::D1D),
                    [&](Index_type i2) {
                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mea::D1D),
                        [&](Index_type i3) {
                          MASS3DEA_4
                        }
                      ); // RAJA::loop<inner_x>
                    }
                  ); // RAJA::loop<inner_y>
                }
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

    getCout() << "\n MASS3DEA : Unknown Hip variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASS3DEA, Hip, Base_HIP, RAJA_HIP)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_HIP
