//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives for loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {


void MASSVEC3DPA::runSeqVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  const Index_type run_reps = getRunReps();

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      for (int e = 0; e < NE; ++e) {

      constexpr int MQ1 = MVPA_Q1D;
      constexpr int MD1 = MVPA_D1D;
      constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;

#if 0
      TEAM_SHARED double B[MQ1][MD1];
      TEAM_SHARED double Bt[MD1][MQ1];

      TEAM_SHARED double sm0[MDQ * MDQ * MDQ];
      TEAM_SHARED double sm1[MDQ * MDQ * MDQ];
      double(*X)[MD1][MD1]   = (double(*)[MD1][MD1])sm0;
      double(*DDQ)[MD1][MQ1] = (double(*)[MD1][MQ1])sm1;
      double(*DQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm0;
      double(*QQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm1;
      double(*QQD)[MQ1][MD1] = (double(*)[MQ1][MD1])sm0;
      double(*QDD)[MD1][MD1] = (double(*)[MD1][MD1])sm1;

      SHARED_LOOP_2D_DIRECT(q, d, Q1D, D1D)
      {
         B[q][d]  = b(q, d);
         Bt[d][q] = B[q][d];
      }

      for (int c = 0; c < 3; ++c)
      {
         SHARED_LOOP_3D_DIRECT(dx, dy, dz, D1D, D1D, D1D)
         {
            X[dz][dy][dx] = dview_x_local(dx, dy, dz, c, e);
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(qx, dy, dz, Q1D, D1D, D1D)
         {
            double u = 0.0;
            for (int dx = 0; dx < D1D; ++dx)
            {
               u += X[dz][dy][dx] * B[qx][dx];
            }
            DDQ[dz][dy][qx] = u;
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(qx, qy, dz, Q1D, Q1D, D1D)
         {
            double u = 0.0;
            for (int dy = 0; dy < D1D; ++dy)
            {
               u += DDQ[dz][dy][qx] * B[qy][dy];
            }
            DQQ[dz][qy][qx] = u;
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(qx, qy, qz, Q1D, Q1D, Q1D)
         {
            double u = 0.0;
            for (int dz = 0; dz < D1D; ++dz)
            {
               u += DQQ[dz][qy][qx] * B[qz][dz];
            }
            QQQ[qz][qy][qx] = u * D(qx, qy, qz, e);
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(dx, qy, qz, D1D, Q1D, Q1D)
         {
            double u = 0.0;
            for (int qx = 0; qx < Q1D; ++qx)
            {
               u += QQQ[qz][qy][qx] * Bt[dx][qx];
            }
            QQD[qz][qy][dx] = u;
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(dx, dy, qz, D1D, D1D, Q1D)
         {
            double u = 0.0;
            for (int qy = 0; qy < Q1D; ++qy)
            {
               u += QQD[qz][qy][dx] * Bt[dy][qy];
            }
            QDD[qz][dy][dx] = u;
         }
         TEAM_SYNC;

         SHARED_LOOP_3D_DIRECT(dx, dy, dz, D1D, D1D, D1D)
         {
            double u = 0.0;
            for (int qz = 0; qz < Q1D; ++qz)
            {
               u += QDD[qz][dy][dx] * Bt[dz][qz];
            }
            dview_y_local(dx, dy, dz, c, e) = u;
         }
         TEAM_SYNC;
      }

#endif
      } //element loop



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
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

              MASSVEC3DPA_0_CPU

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
                } // lambda (dy)
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
                  ); // RAJA::loop<inner_x>
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

    return;
  }
#endif // RUN_RAJA_SEQ

  default:
    getCout() << "\n MASSVEC3DPA : Unknown Seq variant id = " << vid << std::endl;
  }
}

} // end namespace apps
} // end namespace rajaperf
