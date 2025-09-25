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

void INTSC_HEXRECT::intscHexRectOMP
    ( Index_type i,
      Index_type )
{
  INTSC_HEXRECT_DATA_SETUP ;

  Int64_type irec = i ;
  int const max_polygon_pts = 10 ;
  Real_type xd_work[ 3*max_polygon_pts+1 ] ;
  Real_ptr my_qx = xd_work ;

  INTSC_HEXRECT_BODY ;
}


void INTSC_HEXRECT::runOpenMPVariant(VariantID vid,
                                     Size_type RAJAPERF_UNUSED_ARG(tune_idx))
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  //  We compute each standard intersection within a single thread
  //  to avoid collisions in vv_out hence only distribute different
  //  standard intersections among threads, iend is getActualProblemSize.
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend = m_nrecords ;


  auto intsc_hexrect_lam =
    [=](Index_type i) {
      intscHexRectOMP ( i, iend ) ;
    };

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        #pragma omp parallel for
        for (Index_type i = ibegin ; i < iend ; ++i ) {
          intscHexRectOMP( i, iend ) ;
        }

      }
      stopTimer();

      break;
    }

    case Lambda_OpenMP : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        #pragma omp parallel for
        for (Index_type i = ibegin ; i < iend ; ++i ) {
          intsc_hexrect_lam(i);
        }

      }
      stopTimer();

      break;
    }

    case RAJA_OpenMP : {

      auto res{getHostResource()};

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::forall<RAJA::omp_parallel_for_exec>( res,
          RAJA::RangeSegment(ibegin, iend), intsc_hexrect_lam);

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n  INTSC_HEXRECT : Unknown OpenMP variant id = " << vid << std::endl;
    }

  }

#else
  RAJA_UNUSED_VAR(vid);
#endif
}

} // end namespace apps
} // end namespace rajaperf
