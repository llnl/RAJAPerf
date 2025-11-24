//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives for loop unrolling
// #define USE_RAJAPERF_UNROLL

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

void MASSVEC3DPA::runSeqVariant(VariantID vid,
                                size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  const Index_type run_reps = getRunReps();

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      for (Index_type e = 0; e < NE; ++e) {

        MASSVEC3DPA_0_CPU;

        SHARED_LOOP_2D(q, d, MVPA_Q1D, MVPA_D1D) {
          MASSVEC3DPA_1;
        }

        for (Index_type c = 0; c < 3; ++c) {
          SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {

            MASSVEC3DPA_2;
          }

          SHARED_LOOP_3D(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {

            MASSVEC3DPA_3;
          }

          SHARED_LOOP_3D(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
            MASSVEC3DPA_4;
          }

          SHARED_LOOP_3D(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
            MASSVEC3DPA_5;
          }

          SHARED_LOOP_3D(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
            MASSVEC3DPA_6;
          }

          SHARED_LOOP_3D(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
            MASSVEC3DPA_7;
          }

          SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
            MASSVEC3DPA_8;
          }

        }

      } // element loop
    }
    stopTimer();

    break;
  }

#if defined(RUN_RAJA_SEQ)
  case RAJA_Seq: {

    auto res{getHostResource()};

    using launch_policy = RAJA::LaunchPolicy<RAJA::seq_launch_t>;

    using outer_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_y = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_z = RAJA::LoopPolicy<RAJA::seq_exec>;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      // clang-format off
      RAJA::launch<launch_policy>(res, RAJA::LaunchParams(),

          [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

            MASSVEC3DPA_0_CPU;

            //3 loops to remain consistent with the GPU versions
            //Masking out of the z-dimension thread is done with GPU versions
            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
              [&](Index_type) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&](Index_type d) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                      [&](Index_type q) {
                        MASSVEC3DPA_1;
                      } // lambda (q)
                    ); // RAJA::loop<inner_x>
                  } // lambda (d)
                ); // RAJA::loop<inner_y>
              } // lambda ()
            ); // RAJA::loop<inner_z>

            for (Index_type c = 0; c < 3; ++c) {

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                      [&](Index_type dx) {
                        MASSVEC3DPA_2;
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                      [&](Index_type qx) {
                        MASSVEC3DPA_3;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                      [&](Index_type qx) {
                        MASSVEC3DPA_4;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                      [&](Index_type qx) {
                        MASSVEC3DPA_5;
                      } // lambda (qx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (qz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&](Index_type qy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                      [&](Index_type dx) {
                        MASSVEC3DPA_6;
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (qy)
                ); // RAJA::loop<inner_y>
              } // lambda (qz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
              [&](Index_type qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                      [&](Index_type dx) {
                        MASSVEC3DPA_7;
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (qz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
              [&](Index_type dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&](Index_type dy) {
                    RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                      [&](Index_type dx) {
                      MASSVEC3DPA_8;
                      } // lambda (dx)
                    ); // RAJA::loop<inner_x>
                  } // lambda (dy)
                ); // RAJA::loop<inner_y>
              } // lambda (dz)
            ); // RAJA::loop<inner_z>

            ctx.teamSync();

            } // c - dim loop
          }  // lambda (e)
         );  // RAJA::loop<outer_x>

        } // outer lambda (ctx)
      ); // RAJA::launch
      // clang-format on

    } // loop over kernel reps
    stopTimer();

    return;
  }
#endif // RUN_RAJA_SEQ

  default:
    getCout() << "\n MASSVEC3DPA : Unknown Seq variant id = " << vid
              << std::endl;
  }
}

} // end namespace apps
} // end namespace rajaperf
