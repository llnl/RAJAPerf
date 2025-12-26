//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives for loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "DIFFUSION3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

void DIFFUSION3DPA::runSeqVariant(VariantID vid) {
  const Index_type run_reps = getRunReps();

  DIFFUSION3DPA_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      for (Index_type e = 0; e < NE; ++e) {

        DIFFUSION3DPA_0_CPU;

        CPU_FOREACH(dz,z,DPA_D1D) {
          CPU_FOREACH(dy,y,DPA_D1D) {
            CPU_FOREACH(dx,x,DPA_D1D) {
             s_X[dz][dy][dx] = DPA_X(dx,dy,dz,e);
            }
          }
        }

        CPU_FOREACH(dy,y,DPA_D1D) {
          CPU_FOREACH(qx,x,DPA_Q1D) {
            B[qx][dy] = DPA_b(qx,dy);
            G[qx][dy] = DPA_g(qx,dy);
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(dz,z,DPA_D1D) {
          CPU_FOREACH(dy,y,DPA_D1D) {
            CPU_FOREACH(qx,x,DPA_Q1D) {

              double u = 0.0, v = 0.0;
              RAJAPERF_UNROLL(MD1)
              for (int dx = 0; dx < DPA_D1D; ++dx)
              {
                const double coords = s_X[dz][dy][dx];
                u += coords * B[qx][dx];
                v += coords * G[qx][dx];
              }
              DDQ0[dz][dy][qx] = u;
              DDQ1[dz][dy][qx] = v;
            }
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(dz,z,DPA_D1D) {
          CPU_FOREACH(qy,y,DPA_Q1D) {
            CPU_FOREACH(qx,x,DPA_Q1D) {

              double u = 0.0, v = 0.0, w = 0.0;
              RAJAPERF_UNROLL(MD1)
              for (int dy = 0; dy < DPA_D1D; ++dy)
              {
                u += DDQ1[dz][dy][qx] * B[qy][dy];
                v += DDQ0[dz][dy][qx] * G[qy][dy];
                w += DDQ0[dz][dy][qx] * B[qy][dy];
              }
              DQQ0[dz][qy][qx] = u;
              DQQ1[dz][qy][qx] = v;
              DQQ2[dz][qy][qx] = w;
            }
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(qz,z,DPA_Q1D) {
          CPU_FOREACH(qy,y,DPA_Q1D) {
            CPU_FOREACH(qx,x,DPA_Q1D) {

              double u = 0.0, v = 0.0, w = 0.0;
              RAJAPERF_UNROLL(MD1)
              for (int dz = 0; dz < DPA_D1D; ++dz)
              {
                u += DQQ0[dz][qy][qx] * B[qz][dz];
                v += DQQ1[dz][qy][qx] * B[qz][dz];
                w += DQQ2[dz][qy][qx] * G[qz][dz];
              }
              const double O11 = DPA_d(qx,qy,qz,0,e);
              const double O12 = DPA_d(qx,qy,qz,1,e);
              const double O13 = DPA_d(qx,qy,qz,2,e);
              const double O21 = symmetric ? O12 : DPA_d(qx,qy,qz,3,e);
              const double O22 = symmetric ? DPA_d(qx,qy,qz,3,e) : DPA_d(qx,qy,qz,4,e);
              const double O23 = symmetric ? DPA_d(qx,qy,qz,4,e) : DPA_d(qx,qy,qz,5,e);
              const double O31 = symmetric ? O13 : DPA_d(qx,qy,qz,6,e);
              const double O32 = symmetric ? O23 : DPA_d(qx,qy,qz,7,e);
              const double O33 = symmetric ? DPA_d(qx,qy,qz,5,e) : DPA_d(qx,qy,qz,8,e);
              const double gX = u;
              const double gY = v;
              const double gZ = w;
              QQQ0[qz][qy][qx] = (O11*gX) + (O12*gY) + (O13*gZ);
              QQQ1[qz][qy][qx] = (O21*gX) + (O22*gY) + (O23*gZ);
              QQQ2[qz][qy][qx] = (O31*gX) + (O32*gY) + (O33*gZ);

            }
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(dy,y,DPA_D1D) {
          CPU_FOREACH(qx,x,DPA_Q1D) {
            Bt[dy][qx] = DPA_b(qx,dy);
            Gt[dy][qx] = DPA_g(qx,dy);
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(qz,z,DPA_Q1D) {
          CPU_FOREACH(qy,y,DPA_Q1D) {
            CPU_FOREACH(dx,x,DPA_D1D) {

              double u = 0.0, v = 0.0, w = 0.0;
              RAJAPERF_UNROLL(MQ1)
              for (int qx = 0; qx < DPA_Q1D; ++qx)
              {
               u += QQQ0[qz][qy][qx] * Gt[dx][qx];
               v += QQQ1[qz][qy][qx] * Bt[dx][qx];
               w += QQQ2[qz][qy][qx] * Bt[dx][qx];
              }
              QQD0[qz][qy][dx] = u;
              QQD1[qz][qy][dx] = v;
              QQD2[qz][qy][dx] = w;
            }
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(qz,z,DPA_Q1D) {
          CPU_FOREACH(dy,y,DPA_D1D) {
            CPU_FOREACH(dx,x,DPA_D1D) {

              double u = 0.0, v = 0.0, w = 0.0;
              RAJAPERF_UNROLL(DPA_Q1D)
              for (int qy = 0; qy < DPA_Q1D; ++qy)
              {
                u += QQD0[qz][qy][dx] * Bt[dy][qy];
                v += QQD1[qz][qy][dx] * Gt[dy][qy];
                w += QQD2[qz][qy][dx] * Bt[dy][qy];
              }
              QDD0[qz][dy][dx] = u;
              QDD1[qz][dy][dx] = v;
              QDD2[qz][dy][dx] = w;
            }
          }
        }

        //MFEM_SYNC_THREAD;
        CPU_FOREACH(dz,z,DPA_D1D) {
          CPU_FOREACH(dy,y,DPA_D1D) {
            CPU_FOREACH(dx,x,DPA_D1D) {

              double u = 0.0, v = 0.0, w = 0.0;
              RAJAPERF_UNROLL(MQ1)
              for (int qz = 0; qz < DPA_Q1D; ++qz)
              {
                 u += QDD0[qz][dy][dx] * Bt[dz][qz];
                 v += QDD1[qz][dy][dx] * Bt[dz][qz];
                 w += QDD2[qz][dy][dx] * Gt[dz][qz];
              }
              DPA_Y(dx,dy,dz,e) += (u + v + w);
            }
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

    // Currently Teams requires two policies if compiled with a device
    using launch_policy = RAJA::LaunchPolicy<RAJA::seq_launch_t>;

    using outer_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_y = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_z = RAJA::LoopPolicy<RAJA::seq_exec>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      // Grid is empty as the host does not need a compute grid to be specified
      RAJA::launch<launch_policy>( res,
          RAJA::LaunchParams(),
          [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

              DIFFUSION3DPA_0_CPU;

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                [&](Index_type dz) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                    [&](Index_type dy) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                        [&](Index_type dx) {

                          DIFFUSION3DPA_1;

                        } // lambda (dx)
                      ); // RAJA::loop<inner_x>
                    } // lambda (dy)
                  );  //RAJA::loop<inner_y>
                } // lambda (dz)
              );  //RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                [&](Index_type RAJA_UNUSED_ARG(dz)) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                    [&](Index_type dy) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                        [&](Index_type qx) {

                          DIFFUSION3DPA_2;

                        } // lambda (qx)
                      ); // RAJA::loop<inner_x>
                    } // lambda (dy)
                  );  //RAJA::loop<inner_y>
                } // lambda (dz)
              );  //RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                [&](Index_type dz) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                    [&](Index_type dy) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                        [&](Index_type qx) {

                          DIFFUSION3DPA_3;

                        } // lambda (qx)
                      ); // RAJA::loop<inner_x>
                    } // lambda (dy)
                  );  //RAJA::loop<inner_y>
                } // lambda (dz)
              );  //RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                [&](Index_type dz) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                    [&](Index_type qy) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                        [&](Index_type qx) {

                          DIFFUSION3DPA_4;

                        } // lambda (qx)
                      ); // RAJA::loop<inner_x>
                    } // lambda (qy)
                  );  //RAJA::loop<inner_y>
                } // lambda (dz)
              );  //RAJA::loop<inner_z>

             ctx.teamSync();

             RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
               [&](Index_type qz) {
                 RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                   [&](Index_type qy) {
                     RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                       [&](Index_type qx) {

                         DIFFUSION3DPA_5;

                       } // lambda (qx)
                     ); // RAJA::loop<inner_x>
                   } // lambda (qy)
                 );  //RAJA::loop<inner_y>
               } // lambda (qz)
             );  //RAJA::loop<inner_z>

             ctx.teamSync();

             RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
               [&](Index_type RAJA_UNUSED_ARG(dz)) {
                 RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                   [&](Index_type d) {
                     RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                       [&](Index_type q) {

                         DIFFUSION3DPA_6;

                       } // lambda (q)
                     ); // RAJA::loop<inner_x>
                   } // lambda (d)
                 );  //RAJA::loop<inner_y>
               } // lambda (dz)
             );  //RAJA::loop<inner_z>

             ctx.teamSync();

             RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
               [&](Index_type qz) {
                 RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
                   [&](Index_type qy) {
                     RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                       [&](Index_type dx) {

                         DIFFUSION3DPA_7;

                       } // lambda (dx)
                     ); // RAJA::loop<inner_x>
                   } // lambda (qy)
                 );  //RAJA::loop<inner_y>
               } // lambda (qz)
             );  //RAJA::loop<inner_z>

             ctx.teamSync();

             RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_Q1D),
               [&](Index_type qz) {
                 RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                   [&](Index_type dy) {
                     RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                       [&](Index_type dx) {

                         DIFFUSION3DPA_8;

                       } // lambda (dx)
                     ); // RAJA::loop<inner_x>
                   } // lambda (dy)
                 );  //RAJA::loop<inner_y>
               } // lambda (qz)
             );  //RAJA::loop<inner_z>

             ctx.teamSync();

             RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, DPA_D1D),
               [&](Index_type dz) {
                 RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                   [&](Index_type dy) {
                     RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, DPA_D1D),
                       [&](Index_type dx) {

                         DIFFUSION3DPA_9;

                       } // lambda (dx)
                     ); // RAJA::loop<inner_x>
                   } // lambda (dy)
                 );  //RAJA::loop<inner_y>
               } // lambda (dz)
             );  //RAJA::loop<inner_z>

            } // lambda (e)
          ); // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch
    }  // loop over kernel reps
    stopTimer();

    return;
  }
#endif // RUN_RAJA_SEQ

  default:
    getCout() << "\n DIFFUSION3DPA : Unknown Seq variant id = " << vid
              << std::endl;
  }
}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(DIFFUSION3DPA, Seq, Base_Seq, RAJA_Seq)

} // end namespace apps
} // end namespace rajaperf
