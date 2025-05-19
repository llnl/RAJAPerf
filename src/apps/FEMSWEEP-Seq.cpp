//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{


void FEMSWEEP::runSeqVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

         for (int ag = 0; ag < na * ng; ++ag)
         {
            FEMSWEEP_KERNEL;
         }

      }
      stopTimer();

      break;
    }

    case RAJA_Seq : {

      auto res{getHostResource()};

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::seq_launch_t>;

      using outer_x =
          RAJA::LoopPolicy<RAJA::seq_exec>;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

         RAJA::launch<launch_policy>( res,
             RAJA::LaunchParams(),
             [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
             RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, na * ng),
               [&](int ag) {
                 FEMSWEEP_KERNEL;
               });
         });

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown Sequential variant id = " << vid << std::endl;
    }

  }

}

} // end namespace apps
} // end namespace rajaperf
