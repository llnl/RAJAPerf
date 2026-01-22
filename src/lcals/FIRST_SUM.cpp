//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIRST_SUM.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


FIRST_SUM::FIRST_SUM(const RunParams& params)
  : KernelBase(rajaperf::Lcals_FIRST_SUM, params)
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

void FIRST_SUM::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( std::max(target_size, Index_type(2)) );
  setRunReps( target_reps );

  m_N = getActualProblemSize();

  setItsPerRep( m_N-1 );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N ); // y (each iterate accesses the range [i-1, i])
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-1) ); // x
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(1 * (m_N-1));
}

FIRST_SUM::~FIRST_SUM()
{
}

void FIRST_SUM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_N, 0.0, vid);
  allocAndInitData(m_y, m_N, vid);
}

void FIRST_SUM::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x, getActualProblemSize(), vid);
}

void FIRST_SUM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
}

} // end namespace lcals
} // end namespace rajaperf
