//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES_NOVIEW.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{


void LTIMES_NOVIEW::runSeqVariant(VariantID vid, size_t tune_idx)
{
  const Index_type run_reps = getRunReps();

  LTIMES_NOVIEW_DATA_SETUP;

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type z = 0; z < num_z; ++z ) {
          for (Index_type g = 0; g < num_g; ++g ) {
            for (Index_type m = 0; m < num_m; ++m ) {
              for (Index_type d = 0; d < num_d; ++d ) {
                LTIMES_NOVIEW_BODY;
              }
            }
          }
        }

      }
      stopTimer();

      break;
    }

#if defined(RUN_RAJA_SEQ)
    case Lambda_Seq : {

      auto ltimesnoview_lam = [=](Index_type d, Index_type z,
                                  Index_type g, Index_type m) {
                                LTIMES_NOVIEW_BODY;
                              };

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type z = 0; z < num_z; ++z ) {
          for (Index_type g = 0; g < num_g; ++g ) {
            for (Index_type m = 0; m < num_m; ++m ) {
              for (Index_type d = 0; d < num_d; ++d ) {
                ltimesnoview_lam(d, z, g, m);
              }
            }
          }
        }

      }
      stopTimer();

      break;
    }

    case RAJA_Seq : {

      auto res{getHostResource()};

      if (tune_idx == 0) {

        auto ltimesnoview_lam = [=](Index_type d, Index_type z,
                                    Index_type g, Index_type m) {
                                  LTIMES_NOVIEW_BODY;
                                };

        using EXEC_POL =
          RAJA::KernelPolicy<
            RAJA::statement::For<1, RAJA::seq_exec,       // z
              RAJA::statement::For<2, RAJA::seq_exec,     // g
                RAJA::statement::For<3, RAJA::seq_exec,   // m
                  RAJA::statement::For<0, RAJA::seq_exec, // d
                    RAJA::statement::Lambda<0>
                  >
                >
              >
            >
          >;

        startTimer();
        for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

          RAJA::kernel_resource<EXEC_POL>( RAJA::make_tuple(RAJA::RangeSegment(0, num_d),
                                                            RAJA::RangeSegment(0, num_z),
                                                            RAJA::RangeSegment(0, num_g),
                                                            RAJA::RangeSegment(0, num_m)),
                                           res,
                                           ltimesnoview_lam
                                         );

        }
        stopTimer();

      } else if (tune_idx == 1) {

        using launch_policy = RAJA::LaunchPolicy<RAJA::seq_launch_t>;

        using z_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

        using g_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

        using m_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

        using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

        startTimer();
        for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

          RAJA::launch<launch_policy>( res,
              RAJA::LaunchParams(),
              [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

                RAJA::loop<z_policy>(ctx, RAJA::RangeSegment(0, num_z),
                  [&](Index_type z) {
                    RAJA::loop<g_policy>(ctx, RAJA::RangeSegment(0, num_g),
                      [&](Index_type g) {
                        RAJA::loop<m_policy>(ctx, RAJA::RangeSegment(0, num_m),
                          [&](Index_type m) {
                            RAJA::loop<d_policy>(ctx, RAJA::RangeSegment(0, num_d),
                              [&](Index_type d) {
                                LTIMES_NOVIEW_BODY
                              }
                            ); // RAJA::loop<d_policy>
                          }
                        ); // RAJA::loop<m_policy>
                      }
                    ); // RAJA::loop<g_policy>
                  }
                ); // RAJA::loop<z_policy>

              } // outer lambda (ctx)
          );    // RAJA::launch

        } // loop over kernel reps
        stopTimer();

      }

      break;
    }
#endif // RUN_RAJA_SEQ

    default : {
      getCout() << "\n LTIMES_NOVIEW : Unknown variant id = " << vid << std::endl;
    }

  }

}

void LTIMES_NOVIEW::setSeqTuningDefinitions(VariantID vid)
{

  if (vid == RAJA_Seq) {
    addVariantTuningName(vid, "kernel");
    addVariantTuningName(vid, "launch");
  } else {
    addVariantTuningName(vid, "default");
  }

}

} // end namespace apps
} // end namespace rajaperf
