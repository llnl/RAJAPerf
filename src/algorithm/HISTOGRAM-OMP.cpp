//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HISTOGRAM.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace algorithm
{


void HISTOGRAM::runOpenMPVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  HISTOGRAM_DATA_SETUP;

  switch ( vid ) {

    case Base_OpenMP : {

      HISTOGRAM_SETUP_COUNTS;

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        HISTOGRAM_INIT_COUNTS;

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          HISTOGRAM_BODY(RAJAPERF_ATOMIC_ADD_OMP);
        }

        HISTOGRAM_FINALIZE_COUNTS;

      }
      stopTimer();

      HISTOGRAM_TEARDOWN_COUNTS;

      break;
    }

    case Lambda_OpenMP : {

      HISTOGRAM_SETUP_COUNTS;

      auto histogram_base_lam = [=](Index_type i) {
            HISTOGRAM_BODY(RAJAPERF_ATOMIC_ADD_OMP);
          };

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        HISTOGRAM_INIT_COUNTS;

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          histogram_base_lam(i);
        }

        HISTOGRAM_FINALIZE_COUNTS;

      }
      stopTimer();

      HISTOGRAM_TEARDOWN_COUNTS;

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()};

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        HISTOGRAM_INIT_COUNTS_RAJA(RAJA::omp_multi_reduce);

        RAJA::forall<RAJA::omp_parallel_for_exec>( res,
          RAJA::RangeSegment(ibegin, iend), [=](Index_type i) {
            HISTOGRAM_BODY(RAJAPERF_ADD);
        });

        HISTOGRAM_FINALIZE_COUNTS_RAJA(RAJA::omp_multi_reduce);

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n  HISTOGRAM : Unknown variant id = " << vid << std::endl;
    }

  }

  HISTOGRAM_DATA_TEARDOWN;

#else
  RAJA_UNUSED_VAR(vid);
#endif
}

} // end namespace algorithm
} // end namespace rajaperf
