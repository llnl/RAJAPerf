//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIRST_DIFF.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


FIRST_DIFF::FIRST_DIFF(const RunParams& params)
  : KernelBase(rajaperf::Lcals_FIRST_DIFF, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(2000);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::tight);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void FIRST_DIFF::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  m_N = getActualProblemSize()+1;

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type) * m_N ); // y (each iterate accesses the range [i, i+1])
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // x
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(1 * getActualProblemSize());
}

FIRST_DIFF::~FIRST_DIFF()
{
}

void FIRST_DIFF::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_N, 0.0, vid);
  allocAndInitDataRandValue(m_y, m_N, vid);
}

void FIRST_DIFF::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x, getActualProblemSize(), vid);
}

void FIRST_DIFF::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
}

} // end namespace lcals
} // end namespace rajaperf
