//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DAXPY_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace basic
{


void DAXPY_ATOMIC::runOpenMPVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  DAXPY_ATOMIC_DATA_SETUP;

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          DAXPY_ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_OMP);
        }

      }
      stopTimer();

      break;
    }

    case Lambda_OpenMP : {

      auto daxpy_atomic_lam = [=](Index_type i) {
            DAXPY_ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_OMP);
          };

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          daxpy_atomic_lam(i);
        }

      }
      stopTimer();

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()};

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        RAJA::forall<RAJA::omp_parallel_for_exec>( res,
          RAJA::RangeSegment(ibegin, iend), [=](Index_type i) {
          DAXPY_ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_RAJA_OMP);
        });

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n  DAXPY_ATOMIC : Unknown variant id = " << vid << std::endl;
    }

  }

#else
  RAJA_UNUSED_VAR(vid);
#endif
}

} // end namespace basic
} // end namespace rajaperf
