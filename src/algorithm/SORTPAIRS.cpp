//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "SORTPAIRS.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace algorithm
{


SORTPAIRS::SORTPAIRS(const RunParams& params)
  : KernelBase(rajaperf::Algorithm_SORTPAIRS, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(20);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Inconsistent); // sort is not stable and could depend on scheduling
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N_logN);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Sort);

  addVariantTunings( );
}

void SORTPAIRS::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  // not useful in this case due to O(n*log(n)) algorithm
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // x, i
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

SORTPAIRS::~SORTPAIRS()
{
}

void SORTPAIRS::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandValue(m_x, getActualProblemSize()*getRunReps(), vid);
  allocAndInitDataRandValue(m_i, getActualProblemSize()*getRunReps(), vid);
}

void SORTPAIRS::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x, getActualProblemSize()*getRunReps(), vid);
  addToChecksum(m_i, getActualProblemSize()*getRunReps(), vid);
}

void SORTPAIRS::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_i, vid);
}

} // end namespace algorithm
} // end namespace rajaperf
