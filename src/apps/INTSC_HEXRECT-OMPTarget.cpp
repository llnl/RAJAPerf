//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_TARGET_OPENMP)

#include "common/OpenMPTargetDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

  //
  // Define threads per team for target execution (workgroup size)
  //
  const Size_type threads_per_team = 64;


void INTSC_HEXRECT::intscHexHexOMP_Target
    ( Index_type i,
      Index_type iend )  // number of standard intersections
{
  Int64_type nisc_stage = iend * m_tri_per_intsc ;

  for ( Size_type j = 0L ; j < m_tri_per_intsc ; ++j ) {

    Int64_type blksize = threads_per_team ;
    Int64_type ith = i * m_tri_per_intsc + j ;    // which triangle contribution
    Int64_type blk = ith / blksize ;   // which "block" for gpu compatibility

    Real_ptr tsubz = m_tsubz ;
    Real_ptr dsubz = m_dsubz ;

    INTSC_HEXRECT_BODY_SEQ ;

    // Volumes directly to vv_out on the CPU.
    Real_ptr vv_out = m_vv_out + 4L*ipair;

    //   Save results for this triangle, for the subzone pair intersection.
    vv_out[0] += vv_hi + vv_lo ;
    vv_out[1] += vx_hi + vx_lo ;
    vv_out[2] += vy_hi + vy_lo ;
    vv_out[3] += vz_hi + vz_lo ;
  }
}



void INTSC_HEXRECT::runOpenMPTargetVariant(VariantID vid,
                                           Size_type RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = getActualProblemSize() ;

  if ( vid == Base_OpenMPTarget ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

#pragma omp target is_device_ptr          \
  (m_dsubz,m_tsubz,m_vv_out) \
  device( did )
#pragma omp teams distribute parallel for thread_limit(threads_per_team) schedule(static, 1)
      for (Index_type i = ibegin ; i < iend ; ++i ) {
        intscHexHexOMP_Target( i, iend ) ;
      }
    }
    stopTimer();

  } else if ( vid == RAJA_OpenMPTarget ) {

    auto res{getOmpTargetResource()};

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall<RAJA::omp_target_parallel_for_exec<threads_per_team>>( res,
        RAJA::RangeSegment(ibegin, iend), [=](Index_type i) {
          intscHexHexOMP_Target (i, iend) ;
      });
    }
    stopTimer();

  } else {
    getCout() << "\n  INTSC_HEXRECT : Unknown OMP Target variant id = " << vid << std::endl;
  }
}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_TARGET_OPENMP
