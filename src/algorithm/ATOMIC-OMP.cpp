//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

#include <iostream>

namespace rajaperf
{
namespace algorithm
{

template < size_t replication >
void ATOMIC::runOpenMPVariantReplicate(VariantID vid)
{

  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  ATOMIC_DATA_SETUP(replication);

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; static_cast<void>(((irep = irep + 1), 0))) {

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_OMP, i, ATOMIC_VALUE);
        }

      }
      stopTimer();

      break;
    }

    case Lambda_OpenMP : {

      auto atomic_base_lam = [=](Index_type i) {
                                 ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_OMP, i, ATOMIC_VALUE);
                               };

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; static_cast<void>(((irep = irep + 1), 0))) {

        #pragma omp parallel for
        for (Index_type i = ibegin; i < iend; ++i ) {
          atomic_base_lam(i);
        }

      }
      stopTimer();

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()};

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; static_cast<void>(((irep = irep + 1), 0))) {

        RAJA::forall<RAJA::omp_parallel_for_exec>( res,
          RAJA::RangeSegment(ibegin, iend), [=](Index_type i) {
            ATOMIC_BODY(RAJAPERF_ATOMIC_ADD_RAJA_OMP, i, ATOMIC_VALUE);
        });

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n  ATOMIC : Unknown variant id = " << vid << std::endl;
    }

  }

  ATOMIC_DATA_TEARDOWN(replication);

}


void ATOMIC::defineOpenMPVariantTunings()
{

  for (VariantID vid : {Base_OpenMP, Lambda_OpenMP, RAJA_OpenMP}) {

    seq_for(cpu_atomic_replications_type{}, [&](auto replication) {

      if (run_params.numValidAtomicReplication() == 0u ||
          run_params.validAtomicReplication(replication)) {

        addVariantTuning<&ATOMIC::runOpenMPVariantReplicate<replication>>(
            vid, "replicate_"+std::to_string(replication));

      }

    });

  }

}

} // end namespace algorithm
} // end namespace rajaperf

#endif  // RAJA_ENABLE_OPENMP
