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

#include <iostream>

namespace rajaperf {
namespace apps {


void MASSVEC3DPA::runOpenMPVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {

#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  const Index_type run_reps = getRunReps();
  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_OpenMP: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

#pragma omp parallel for
      for (int e = 0; e < NE; ++e) {

        MASSVEC3DPA_0_CPU;

        SHARED_LOOP_2D_DIRECT(q, d, MVPA_Q1D, MVPA_D1D) {
          MASSVEC3DPA_1;
          MASSVEC3DPA_1;
        }

        for (int c = 0; c < 3; ++c) {
          SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {

            MASSVEC3DPA_2;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {

            MASSVEC3DPA_3;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
            MASSVEC3DPA_4;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
            MASSVEC3DPA_5;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
            MASSVEC3DPA_6;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
            MASSVEC3DPA_7;
          }
          /*TEAM_SYNC;*/

          SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
            MASSVEC3DPA_8;
          }
          /*TEAM_SYNC;*/
        }

      } // element loop
    }
    stopTimer();

    break;
  }

  case RAJA_OpenMP: {

    auto res{getHostResource()};

    using launch_policy = RAJA::LaunchPolicy<RAJA::omp_launch_t>;

    using outer_x = RAJA::LoopPolicy<RAJA::omp_for_exec>;

    using inner_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_y = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_z = RAJA::LoopPolicy<RAJA::seq_exec>;    

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      //Grid is empty as the host does not need a compute grid to be specified
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

            MASSVEC3DPA_0_CPU;

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
      );  // // RAJA::launch

    }  // loop over kernel reps
    stopTimer();

    return;
  }

  default:
    getCout() << "\n MASSVEC3DPA : Unknown OpenMP variant id = " << vid
              << std::endl;
  }

#else
  RAJA_UNUSED_VAR(vid);
#endif
}

} // end namespace apps
} // end namespace rajaperf
