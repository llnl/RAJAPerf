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

#if 0
      constexpr int MQ1 = MVPA_Q1D;
      constexpr int MD1 = MVPA_D1D;
      constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;

      /*RAJA_TEAM_SHARED*/ double smB[MQ1][MD1];
      /*RAJA_TEAM_SHARED*/ double smBt[MD1][MQ1];

      /*RAJA_TEAM_SHARED*/ double sm0[MDQ * MDQ * MDQ];
      /*RAJA_TEAM_SHARED*/ double sm1[MDQ * MDQ * MDQ];
      double(*smX)[MD1][MD1]   = (double(*)[MD1][MD1])sm0;
      double(*DDQ)[MD1][MQ1] = (double(*)[MD1][MQ1])sm1;
      double(*DQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm0;
      double(*QQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm1;
      double(*QQD)[MQ1][MD1] = (double(*)[MQ1][MD1])sm0;
      double(*QDD)[MD1][MD1] = (double(*)[MD1][MD1])sm1;
#endif

      MASSVEC3DPA_0_CPU;

      SHARED_LOOP_2D_DIRECT(q, d, MVPA_Q1D, MVPA_D1D)
      {
        //smB[q][d]  = mvpaB_(q, d);
        //smBt[d][q] = smB[q][d];
        MASSVEC3DPA_1;
        MASSVEC3DPA_1;
      }

      for (int c = 0; c < 3; ++c)
      {
         SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D)
         {
           //smX[dz][dy][dx] = mvpaX_(dx, dy, dz, c, e);
           MASSVEC3DPA_2;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D)
         {
           /*
            double u = 0.0;
            for (int dx = 0; dx < MVPA_D1D; ++dx)
            {
               u += smX[dz][dy][dx] * smB[qx][dx];
            }
            DDQ[dz][dy][qx] = u;
           */
           MASSVEC3DPA_3;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D)
         {
           /*
            double u = 0.0;
            for (int dy = 0; dy < MVPA_D1D; ++dy)
            {
               u += DDQ[dz][dy][qx] * smB[qy][dy];
            }
            DQQ[dz][qy][qx] = u;
           */
           MASSVEC3DPA_4;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)
         {
           /*
            double u = 0.0;
            for (int dz = 0; dz < MVPA_D1D; ++dz)
            {
               u += DQQ[dz][qy][qx] * smB[qz][dz];
            }
            QQQ[qz][qy][qx] = u * mvpaD_(qx, qy, qz, e);
           */
           MASSVEC3DPA_5;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D)
         {
           /*
            double u = 0.0;
            for (int qx = 0; qx < MVPA_Q1D; ++qx)
            {
               u += QQQ[qz][qy][qx] * smBt[dx][qx];
            }
            QQD[qz][qy][dx] = u;
           */
           MASSVEC3DPA_6;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D)
         {
           /*
            double u = 0.0;
            for (int qy = 0; qy < MVPA_Q1D; ++qy)
            {
               u += QQD[qz][qy][dx] * smBt[dy][qy];
            }
            QDD[qz][dy][dx] = u;
           */
           MASSVEC3DPA_7;
         }
         /*TEAM_SYNC;*/

         SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D)
         {
         /*
            double u = 0.0;
            for (int qz = 0; qz < MVPA_Q1D; ++qz)
            {
               u += QDD[qz][dy][dx] * smBt[dz][qz];
            }
            mvpaY_(dx, dy, dz, c, e) = u;
         */
         MASSVEC3DPA_8;
         }
         /*TEAM_SYNC;*/

      }

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

    using inner_z = RAJA::LoopPolicy<RAJA::seq_exec>;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {

              MASSVEC3DPA_0_CPU

              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
              [&] (int q) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int d) {
                MASSVEC3DPA_1;
                });
              });

              for (int c = 0; c < 3; ++c)
                {
                  //SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D)
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
              [&] (int dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                      [&] (int dx) {

                      //smX[dz][dy][dx] = mvpaX_(dx, dy, dz, c, e);
                      MASSVEC3DPA_2;
                  });
                });
              });

                  /*TEAM_SYNC;*/

              //SHARED_LOOP_3D_DIRECT(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D)
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&] (int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&] (int qx) {

                    MASSVEC3DPA_3;
                    });
                  });
                });
                  /*TEAM_SYNC;*/

              //SHARED_LOOP_3D_DIRECT(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D)
              //{
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&] (int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&] (int qx) {
                      MASSVEC3DPA_4;
                    });
                  });
                });
                  /*TEAM_SYNC;*/

              //SHARED_LOOP_3D_DIRECT(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&] (int qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&] (int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                    [&] (int qx) {
                      /*
                        double u = 0.0;
                        for (int dz = 0; dz < MVPA_D1D; ++dz)
                        {
                        u += DQQ[dz][qy][qx] * smB[qz][dz];
                        }
                        QQQ[qz][qy][qx] = u * mvpaD_(qx, qy, qz, e);
                      */
                      MASSVEC3DPA_5;
                    });
                  });
                });
                  /*TEAM_SYNC;*/

              //SHARED_LOOP_3D_DIRECT(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D)
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&] (int qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                  [&] (int qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                    [&] (int dx) {
                      /*
                        double u = 0.0;
                        for (int qx = 0; qx < MVPA_Q1D; ++qx)
                        {
                        u += QQQ[qz][qy][qx] * smBt[dx][qx];
                        }
                        QQD[qz][qy][dx] = u;
                      */
                      MASSVEC3DPA_6;
                    });
                  });
                });
                  /*TEAM_SYNC;*/


              //SHARED_LOOP_3D_DIRECT(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D)
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                [&] (int qz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                  [&] (int dx) {
                      /*
                        double u = 0.0;
                        for (int qy = 0; qy < MVPA_Q1D; ++qy)
                        {
                        u += QQD[qz][qy][dx] * smBt[dy][qy];
                        }
                        QDD[qz][dy][dx] = u;
                      */
                      MASSVEC3DPA_7;
                  });
                });
                });
                  /*TEAM_SYNC;*/

                  //SHARED_LOOP_3D_DIRECT(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D)
                  //{
              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dz) {
                RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                [&] (int dx) {

                MASSVEC3DPA_8;

                });
                });
                });
                  /*TEAM_SYNC;*/
              }

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
