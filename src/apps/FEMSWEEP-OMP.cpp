//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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


void FEMSWEEP::runOpenMPVariant(VariantID vid)
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  const Index_type run_reps = getRunReps();

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         #pragma omp parallel for
         for (int ag = 0; ag < na * ng; ++ag)
         {
            FEMSWEEP_KERNEL;
         }

      }
      stopTimer();

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()};

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::omp_launch_t>;

      using outer_x =
          RAJA::LoopPolicy<RAJA::omp_for_exec>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

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
      getCout() << "\n FEMSWEEP : Unknown OpenMP variant id = " << vid << std::endl;
    }

  }

#else
  RAJA_UNUSED_VAR(vid);
#endif

}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(FEMSWEEP, OpenMP, Base_OpenMP, RAJA_OpenMP)

} // end namespace apps
} // end namespace rajaperf
