//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives for loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASS3DPA_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {


void MASS3DPA_ATOMIC::runSeqVariant(VariantID vid) {
  const Index_type run_reps = getRunReps();

  MASS3DPA_ATOMIC_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    //for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {
    {

      for (Index_type e = 0; e < NE; ++e) {

      constexpr int MQ1 = mpa3d_at::Q1D;
      constexpr int MD1 = mpa3d_at::D1D;

      constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;

      double sm_B[MQ1][MD1];
      double sm_Bt[MD1][MQ1];

      double sm0[MDQ * MDQ * MDQ];
      double sm1[MDQ * MDQ * MDQ];
      double(*sm_X)[MD1][MD1]   = (double(*)[MD1][MD1])sm0;
      double(*DDQ)[MD1][MQ1]    = (double(*)[MD1][MQ1])sm1;
      double(*DQQ)[MQ1][MQ1]    = (double(*)[MQ1][MQ1])sm0;
      double(*QQQ)[MQ1][MQ1]    = (double(*)[MQ1][MQ1])sm1;
      double(*QQD)[MQ1][MD1]    = (double(*)[MQ1][MD1])sm0;
      double(*QDD)[MD1][MD1]    = (double(*)[MD1][MD1])sm1;

      int thread_dofs[MD1 * MD1 * MD1];

      //1.
      for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
        for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
          for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
            int j          = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);
            //missing dof_map for lexicographical ordering
            thread_dofs[j] = m_elemToDoF[j + mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D * e];
            sm_X[dz][dy][dx]  = X[thread_dofs[j]];
          }
        }
      }

      //2.
      for(int d=0; d<mpa3d_at::D1D; ++d) {
        for(int q=0; q<mpa3d_at::Q1D; ++q) {
          sm_B[q][d]  = MPAT_B(q, d);
          sm_Bt[d][q] = sm_B[q][d];
        }
      }

      //3.
      for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
        for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
          for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {
          double u = 0.0;
          for (int dx = 0; dx < mpa3d_at::D1D; ++dx)
          {
          u += sm_X[dz][dy][dx] * sm_B[qx][dx];
          }
          DDQ[dz][dy][qx] = u;
          }
        }
      }

      //4.
      for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
        for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
          for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {

          double u = 0.0;
          for (int dy = 0; dy < mpa3d_at::D1D; ++dy)
          {
          u += DDQ[dz][dy][qx] * sm_B[qy][dy];
          }
          DQQ[dz][qy][qx] = u;
          }
        }
      }

      //5.
      for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
        for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
          for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {

            double u = 0.0;
            for (int dz = 0; dz < mpa3d_at::D1D; ++dz)
            {
            u += DQQ[dz][qy][qx] * sm_B[qz][dz];
            }
            QQQ[qz][qy][qx] = u * MPAT_D(qx, qy, qz, e);
          }
        }
      }

      //6.
      for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
        for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
          for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
          double u = 0.0;
          for (int qx = 0; qx < mpa3d_at::Q1D; ++qx)
          {
          u += QQQ[qz][qy][qx] * sm_Bt[dx][qx];
          }
          QQD[qz][qy][dx] = u;
          }
        }
      }

      //7.
      for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
        for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
          for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
            double u = 0.0;
            for (int qy = 0; qy<mpa3d_at::Q1D; ++qy)
            {
            u += QQD[qz][qy][dx] * sm_Bt[dy][qy];
            }
            QDD[qz][dy][dx] = u;
          }
        }
      }

      //7.
      for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
        for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
          for(int dx=0; dx<mpa3d_at::D1D; ++dx) {

            double u = 0.0;
            for (int qz = 0; qz < mpa3d_at::Q1D; ++qz)
            {
               u += QDD[qz][dy][dx] * sm_Bt[dz][qz];
            }
            const int j = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);
            Y[thread_dofs[j]] += u; //atomic add
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

    //Currently Teams requires two policies if compiled with a device
    using launch_policy = RAJA::LaunchPolicy<RAJA::seq_launch_t>;

    using outer_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_x = RAJA::LoopPolicy<RAJA::seq_exec>;

    using inner_y = RAJA::LoopPolicy<RAJA::seq_exec>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

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
    getCout() << "\n MASS3DPA_ATOMIC : Unknown Seq variant id = " << vid << std::endl;
  }
}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(MASS3DPA_ATOMIC, Seq, Base_Seq, RAJA_Seq)

} // end namespace apps
} // end namespace rajaperf
