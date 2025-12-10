//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "PI_REDUCE.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace basic
{

template < size_t tune_idx >
void PI_REDUCE::runSeqVariant(VariantID vid)
{
#if !defined(RUN_RAJA_SEQ)
  RAJA_UNUSED_VAR(tune_idx);
#endif
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  PI_REDUCE_DATA_SETUP;

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        Real_type pi = m_pi_init;

        for (Index_type i = ibegin; i < iend; ++i ) {
          PI_REDUCE_BODY;
        }

        m_pi = 4.0 * pi;

      }
      stopTimer();

      break;
    }

#if defined(RUN_RAJA_SEQ)
    case Lambda_Seq : {

      auto pireduce_base_lam = [=](Index_type i, Real_type& pi) {
            PI_REDUCE_BODY;
          };

      startTimer();
      // Awkward expression for loop counter quiets C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

        Real_type pi = m_pi_init;

        for (Index_type i = ibegin; i < iend; ++i ) {
          pireduce_base_lam(i, pi);
        }

        m_pi = 4.0 * pi;

      }
      stopTimer();

      break;
    }

    case RAJA_Seq : {

      auto res{getHostResource()};

      if constexpr (tune_idx == 0) {

        startTimer();
        // Awkward expression for loop counter quiets C++20 compiler warning
        for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

          RAJA::ReduceSum<RAJA::seq_reduce, Real_type> pi(m_pi_init);
  
          RAJA::forall<RAJA::seq_exec>( res,
            RAJA::RangeSegment(ibegin, iend),
            [=](Index_type i) {
              PI_REDUCE_BODY;
          });

          m_pi = 4.0 * pi.get();

        }
        stopTimer();

      } else if constexpr (tune_idx == 1) {

        startTimer();
        // Awkward expression for loop counter quiets C++20 compiler warning
        for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

          Real_type tpi = m_pi_init;
 
          RAJA::forall<RAJA::seq_exec>( res,
            RAJA::RangeSegment(ibegin, iend),
            RAJA::expt::Reduce<RAJA::operators::plus>(&tpi),
            [=] (Index_type i,
              RAJA::expt::ValOp<Real_type, RAJA::operators::plus>& pi) {
              PI_REDUCE_BODY;
            }
          );

          m_pi = static_cast<Real_type>(tpi) * 4.0;

        }
        stopTimer();       
  
      } else {
        getCout() << "\n  PI_REDUCE : Unknown Seq tuning index = " << tune_idx << std::endl;
      }

      break;
    }
#endif

    default : {
      getCout() << "\n  PI_REDUCE : Unknown variant id = " << vid << std::endl;
    }

  }

}

void PI_REDUCE::defineSeqVariantTunings()
{

  for (VariantID vid : {Base_Seq, Lambda_Seq, RAJA_Seq}) {

    addVariantTuning<&PI_REDUCE::runSeqVariant<0>>(
        vid, "default");

    if (vid == RAJA_Seq) {

      addVariantTuning<&PI_REDUCE::runSeqVariant<1>>(
          vid, "new");

    }

  }

}

} // end namespace basic
} // end namespace rajaperf
