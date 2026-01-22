//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DIFF_PREDICT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"
namespace rajaperf
{
namespace lcals
{


DIFF_PREDICT::DIFF_PREDICT(const RunParams& params)
  : KernelBase(rajaperf::Lcals_DIFF_PREDICT, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(200);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void DIFF_PREDICT::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );

  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // cx(4)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // px(13)
  setBytesModifyWrittenPerRep( 9*sizeof(Real_type) * getActualProblemSize() ); // px(4), px(5), px(6), px(7), px(8), px(9), px(10), px(11), px(12)
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(9 * getActualProblemSize());
}

DIFF_PREDICT::~DIFF_PREDICT()
{
}

void DIFF_PREDICT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_array_length = getActualProblemSize() * 14;
  m_offset = getActualProblemSize();

  allocAndInitDataConst(m_px, m_array_length, 0.0, vid);
  allocAndInitData(m_cx, m_array_length, vid);
}

void DIFF_PREDICT::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_px, m_array_length, vid);
}

void DIFF_PREDICT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_px, vid);
  deallocData(m_cx, vid);
}

} // end namespace lcals
} // end namespace rajaperf
