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

void INTSC_HEXHEX::intscHexHexSeq
    ( Index_type i,
      Index_type nisc_stage )   // is iend, for compatibility with the macro
{
  // for compatibility with gpu code
  Int64_type blksize = default_gpu_block_size ;

  Int64_type ith = i ;
  Int64_type blk = ith / blksize ;   // which "block" for gpu compatibility

  if ( i == 0 ) {   // initialize the accumulation
    Index_type n_std_intsc = m_nthreads / m_tri_per_intsc ;
    Index_type vv_len = 32L * n_std_intsc ;
    for ( Index_type k = 0 ; k < vv_len ; ++k ) {
      m_vv_out[k] = 0.0 ;
    }
  }

  Real_ptr tsubz = m_tsubz ;
  Real_ptr dsubz = m_dsubz ;

  INTSC_HEXHEX_BODY_SEQ ;

  // Volumes directly to vv_out on the CPU.
  Real_ptr vv_out = m_vv_out + 4L*ipair;

  //   Save results for this triangle, for the subzone pair intersection.
  vv_out[0] += vv_hi + vv_lo ;
  vv_out[1] += vx_hi + vx_lo ;
  vv_out[2] += vy_hi + vy_lo ;
  vv_out[3] += vz_hi + vz_lo ;
}


void INTSC_HEXHEX::runSeqVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend = m_nthreads ;

#if defined(RUN_RAJA_SEQ)
  auto intsc_hexhex_lam = [=](Index_type i) {
                     intscHexHexSeq ( i, iend ) ;
                   };
#endif

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type i = ibegin ; i < iend ; ++i ) {
          intscHexHexSeq ( i, iend ) ;
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
          intsc_hexhex_lam( i );
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
          RAJA::RangeSegment(ibegin, iend), intsc_hexhex_lam);

      }
      stopTimer();

      break;
    }
#endif // RUN_RAJA_SEQ

    default : {
      getCout() << "\n  INTSC_HEXHEX : Unknown variant id = " << vid << std::endl;
    }

  }

}

} // end namespace apps
} // end namespace rajaperf
