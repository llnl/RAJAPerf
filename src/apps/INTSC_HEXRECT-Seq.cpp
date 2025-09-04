//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"

#include <iostream>



namespace rajaperf
{
namespace apps
{

void INTSC_HEXRECT::intscHexRectSeq
    ( Index_type i, Index_type nrecords )
{
  INTSC_HEXRECT_DATA_SETUP ;

  long irec = i ;
  int const max_polygon_pts = 10 ;
  double xd_work[ 3*max_polygon_pts+1 ] ;
  double *my_qx = xd_work ;

  INTSC_HEXRECT_BODY ;

}


void INTSC_HEXRECT::runSeqVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend = m_nrecords ;

#if defined(RUN_RAJA_SEQ)
  auto intsc_hexrect_lam = [=](Index_type i) {
                     intscHexRectSeq ( i, iend ) ;
                   };
#endif

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type i = ibegin ; i < iend ; ++i ) {
          intscHexRectSeq ( i, iend ) ;
        }

      }
      stopTimer();

      break;
    }

#if defined(RUN_RAJA_SEQ)
    case Lambda_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type i = ibegin ; i < iend; ++i ) {
          intsc_hexrect_lam( i );
        }

      }
      stopTimer();

      break;
    }

    case RAJA_Seq : {

      auto res{getHostResource()};

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::forall<RAJA::seq_exec>( res,
          RAJA::RangeSegment(ibegin, iend), intsc_hexrect_lam);

      }
      stopTimer();

      break;
    }
#endif // RUN_RAJA_SEQ

    default : {
      getCout() << "\n  INTSC_HEXRECT : Unknown variant id = " << vid << std::endl;
    }

  }

}

} // end namespace apps
} // end namespace rajaperf
