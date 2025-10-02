//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

void INTSC_HEXHEX::intscHexHexOMP
    ( Index_type i,
      Index_type iend )  // number of standard intersections
{
  Index_type nisc_stage = iend * m_tri_per_intsc ;

  //  A "standard intersection" is eight subzones.
  Index_type nsubzones_per_std_intsc = 8 ;
  Index_type ipair0 = i * nsubzones_per_std_intsc ;

  // Initialize the accumulation.
  for ( Index_type j=0 ; j < 4L*nsubzones_per_std_intsc ; ++j ) {
    m_vv_out[ 4L*ipair0 + j ] = 0.0 ;
  }

  Index_type const tri_per_intsc = (Index_type) m_tri_per_intsc ;

  for ( Index_type j = 0 ; j < tri_per_intsc ; ++j ) {

    // for compatibility with gpu code
    Index_type blksize = default_gpu_block_size ;
    Index_type ith = i * m_tri_per_intsc + j ;    // which triangle contribution
    Index_type blk = ith / blksize ;   // which "block" for gpu compatibility

    INTSC_HEXHEX_DATA_SETUP ;

    INTSC_HEXHEX_BODY_SEQ ;

    // Volumes directly to vv_out on the CPU.
    Real_ptr vv_out = m_vv_out + 4L*ipair;

    //   Save results for this triangle, for the subzone pair intersection.
    vv_out[0] += vv_hi + vv_lo ;
    vv_out[1] += vx_hi + vx_lo ;
    vv_out[2] += vy_hi + vy_lo ;
    vv_out[3] += vz_hi + vz_lo ;
  }
}


void INTSC_HEXHEX::runOpenMPVariant(VariantID vid,
                                    Size_type RAJAPERF_UNUSED_ARG(tune_idx))
{
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)

  //  We compute each standard intersection within a single thread
  //  to avoid collisions in vv_out hence only distribute different
  //  standard intersections among threads, iend is getActualProblemSize.
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend = getActualProblemSize() ;


  auto intsc_hexhex_lam =
    [=](Index_type i) {
      intscHexHexOMP ( i, iend ) ;
    };

  switch ( vid ) {

    case Base_OpenMP : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        #pragma omp parallel for
        for (Index_type i = ibegin ; i < iend ; ++i ) {
          intscHexHexOMP( i, iend ) ;
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
          intsc_hexhex_lam(i);
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
          RAJA::RangeSegment(ibegin, iend), intsc_hexhex_lam);

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n  INTSC_HEXHEX : Unknown OpenMP variant id = " << vid << std::endl;
    }

  }

#else
  RAJA_UNUSED_VAR(vid);
#endif
}

} // end namespace apps
} // end namespace rajaperf
