//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "TRAP_INT.hpp"

#include "RAJA/RAJA.hpp"

#include "TRAP_INT-func.hpp"

#include <iostream>

namespace rajaperf
{
namespace basic
{


void TRAP_INT::runOpenMPVariant(VariantID vid, size_t tune_idx)
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  TRAP_INT_DATA_SETUP;

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        Real_type sumx = m_sumx_init;

        #pragma omp parallel for reduction(+:sumx)
        for (Index_type i = ibegin; i < iend; ++i ) {
          TRAP_INT_BODY;
        }

        m_sumx += sumx * h;

      }
      stopTimer();

      break;
    }

    case Lambda_OpenMP : {

      auto trapint_base_lam = [=](Index_type i) -> Real_type {
                                Real_type x = x0 + i*h;
                                return trap_int_func(x, y, xp, yp);
                              };

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        Real_type sumx = m_sumx_init;

        #pragma omp parallel for reduction(+:sumx)
        for (Index_type i = ibegin; i < iend; ++i ) {
          sumx += trapint_base_lam(i);
        }

        m_sumx += sumx * h;

      }
      stopTimer();

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()}; 

      if (tune_idx == 0) {

        startTimer();
        // Awkward expression for loop counter quiets C++20 compiler warning
        for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

          RAJA::ReduceSum<RAJA::omp_reduce, Real_type> sumx(m_sumx_init);

          RAJA::forall<RAJA::omp_parallel_for_exec>( res,
            RAJA::RangeSegment(ibegin, iend), [=](Index_type i) {
            TRAP_INT_BODY;
          });

          m_sumx += static_cast<Real_type>(sumx.get()) * h;

        }
        stopTimer();
  
      } else if (tune_idx == 1) {

        startTimer();
        // Awkward expression for loop counter quiets C++20 compiler warning
        for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

          Real_type tsumx = m_sumx_init;

          RAJA::forall<RAJA::omp_parallel_for_exec>( res,
            RAJA::RangeSegment(ibegin, iend),
            RAJA::expt::Reduce<RAJA::operators::plus>(&tsumx),
            [=] (Index_type i,
              RAJA::expt::ValOp<Real_type, RAJA::operators::plus>& sumx) {
              TRAP_INT_BODY;
            }
          );

          m_sumx += static_cast<Real_type>(tsumx) * h;

        }
        stopTimer();

      } else {
        getCout() << "\n  TRAP_INT : Unknown OpenMP tuning index = " << tune_idx << std::endl;
      }

      break;
    }

    default : {
      getCout() << "\n  TRAP_INT : Unknown variant id = " << vid << std::endl;
    }

  }

#else
  RAJA_UNUSED_VAR(vid);
  RAJA_UNUSED_VAR(tune_idx);
#endif
}

void TRAP_INT::setOpenMPTuningDefinitions(VariantID vid)
{
  addVariantTuningName(vid, "default");
  if (vid == RAJA_OpenMP) {
    addVariantTuningName(vid, "new");
  }
}

} // end namespace basic
} // end namespace rajaperf
