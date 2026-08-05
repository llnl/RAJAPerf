//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "TRANSPORT3DMC.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{


void TRANSPORT3DMC::runSeqVariant(VariantID vid)
{
  const Index_type run_reps = 20;
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

#if defined(RUN_RAJA_SEQ)
  auto transport3dmc_lam = [=](Index_type i) {
                             TRANSPORT3DMC_BODY
                           };
  auto transport3dmc_lam_setup = [=](VariantID vid, size_t partCt) {
                                    TRANSPORT3DMC_DATA_SETUP(vid, partCt);
                                  };
  auto transport3dmc_lam_reset = [=]() {
                                    TRANSPORT3DMC_RESET
                                  };
#endif

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      TRANSPORT3DMC_DATA_SETUP(vid, iend);
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {
        TRANSPORT3DMC_RESET
        for (Index_type i = ibegin; i < iend; ++i ) {
          TRANSPORT3DMC_BODY
        }

      }
      stopTimer();

      break;
    }

#if defined(RUN_RAJA_SEQ)
    case Lambda_Seq : {

      startTimer();
      transport3dmc_lam_setup(vid, iend);
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {
        transport3dmc_lam_reset();
        for (Index_type i = ibegin; i < iend; ++i ) {
          transport3dmc_lam(i);
        }

      }
      stopTimer();

      break;
    }

    case RAJA_Seq : {

      auto res{getHostResource()};

      startTimer();
      transport3dmc_lam_setup(vid, iend);
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {
        transport3dmc_lam_reset;
        RAJA::forall<RAJA::seq_exec>( res,
          RAJA::RangeSegment(ibegin, iend), transport3dmc_lam);

      }
      stopTimer();

      break;
    }
#endif // RUN_RAJA_SEQ

    default : {
      getCout() << "\n  TRANSPORT3DMC : Unknown variant id = " << vid << std::endl;
    }

  }

}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(TRANSPORT3DMC, Seq, Base_Seq, Lambda_Seq, RAJA_Seq)

} // end namespace apps
} // end namespace rajaperf
