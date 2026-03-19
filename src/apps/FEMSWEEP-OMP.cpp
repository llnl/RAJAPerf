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


#define USE_OMP_COLLAPSE
//#undef USE_OMP_COLLAPSE


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

#if defined(USE_OMP_COLLAPSE)
        #pragma omp parallel for collapse(2)
        for (Index_type a = 0; a < na; ++a)
        for (Index_type g = 0; g < ng; ++g)
        {
#else
        #pragma omp parallel for
        for (Index_type ag = 0; ag < na * ng; ++ag)
        {
           const Index_type a = ag / ng;
           const Index_type g = ag % ng;
#endif
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

      // TODO: add omp_parallel_collapse_exec version

      using outer_x =
          RAJA::LoopPolicy<RAJA::omp_for_exec>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, na * ng),
              [&](Index_type ag) {
            const Index_type a = ag / ng;
            const Index_type g = ag % ng;
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
