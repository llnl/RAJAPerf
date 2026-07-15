//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives for loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASS3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {


template <Index_type D1D, Index_type Q1D>
void MASS3DPA::runSeqVariantImpl(VariantID vid) {
  constexpr Index_type MD1 = D1D;
  constexpr Index_type MQ1 = Q1D;

  const Index_type run_reps = getRunReps();

  MASS3DPA_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      for (Index_type e = 0; e < NE; ++e) {

        MASS3DPA_0_CPU

        CPU_FOREACH(dy, y, MD1) {
          CPU_FOREACH(dx, x, MD1){
            MASS3DPA_1
          }
          CPU_FOREACH(dx, x, MQ1) {
            MASS3DPA_2
          }
        }

        CPU_FOREACH(dy, y, MD1) {
          CPU_FOREACH(qx, x, MQ1) {
            MASS3DPA_3
          }
        }

        CPU_FOREACH(qy, y, MQ1) {
          CPU_FOREACH(qx, x, MQ1) {
            MASS3DPA_4
          }
        }

        CPU_FOREACH(qy, y, MQ1) {
          CPU_FOREACH(qx, x, MQ1) {
            MASS3DPA_5
          }
        }

        CPU_FOREACH(d, y, MD1) {
          CPU_FOREACH(q, x, MQ1) {
            MASS3DPA_6
          }
        }

        CPU_FOREACH(qy, y, MQ1) {
          CPU_FOREACH(dx, x, MD1) {
            MASS3DPA_7
          }
        }

        CPU_FOREACH(dy, y, MD1) {
          CPU_FOREACH(dx, x, MD1) {
            MASS3DPA_8
          }
        }

        CPU_FOREACH(dy, y, MD1) {
          CPU_FOREACH(dx, x, MD1) {
            MASS3DPA_9
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

    //Currently Teams requires two policies if compiled with a device
    using launch_policy = RAJA::LaunchPolicy<RAJA::seq_launch_t>;

    using outer_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_y = RAJA::LoopPolicy<RAJA::seq_exec>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      //clang-format off
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

              MASS3DPA_0_CPU

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type dx) {
                      MASS3DPA_1
                    }
                  );  // RAJA::loop<inner_x>

                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                    [&](Index_type dx) {
                      MASS3DPA_2
                    }
                  );  // RAJA::loop<inner_x>
                } // lambda (dy)
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                    [&](Index_type qx) {
                      MASS3DPA_3
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                    [&](Index_type qx) {
                      MASS3DPA_4
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                    [&](Index_type qx) {
                      MASS3DPA_5
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                [&](Index_type d) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                    [&](Index_type q) {
                      MASS3DPA_6
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type dx) {
                      MASS3DPA_7
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type dx) {
                      MASS3DPA_8
                    }
                  ); // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

              ctx.teamSync();

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type dx) {
                      MASS3DPA_9
                    }
                  );  // RAJA::loop<inner_x>
                }
              );  // RAJA::loop<inner_y>

            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch
      //clang-format on

    }  // loop over kernel reps
    stopTimer();

    return;
  }
#endif // RUN_RAJA_SEQ

  default:
    getCout() << "\n MASS3DPA : Unknown Seq variant id = " << vid << std::endl;
  }
}

void MASS3DPA::runSeqVariant(VariantID vid)
{
  runSeqVariantImpl<mpa::D1D, mpa::Q1D>(vid);
}

void MASS3DPA::defineSeqVariantTunings()
{
  for (VariantID vid : {Base_Seq, RAJA_Seq}) {
#define MASS3DPA_SEQ_TUNING(name, tuning_name, d1d, q1d)                      \
    addVariantTuning<&MASS3DPA::runSeqVariantImpl<d1d, q1d>>(vid, tuning_name);
    MASS3DPA_GEOMETRIES(MASS3DPA_SEQ_TUNING)
#undef MASS3DPA_SEQ_TUNING
  }
}

} // end namespace apps
} // end namespace rajaperf
