//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "IF_QUAD.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


IF_QUAD::IF_QUAD(const RunParams& params)
  : KernelBase(rajaperf::Basic_IF_QUAD, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(180);

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

void IF_QUAD::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 3*sizeof(Real_type) * getActualProblemSize() ); // b, a, c
  setBytesWrittenPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // x2, x1
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  // estimate conditional true half of the time, 1 sqrt
  setFLOPsPerRep(4 * getActualProblemSize() +
                 7 * getActualProblemSize() / 2);
}

IF_QUAD::~IF_QUAD()
{
}

void IF_QUAD::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_a, getActualProblemSize(), vid);
  allocAndInitData(m_b, getActualProblemSize(), vid);
  allocAndInitData(m_c, getActualProblemSize(), vid);
  allocAndInitDataConst(m_x1, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_x2, getActualProblemSize(), 0.0, vid);
}

void IF_QUAD::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x1, getActualProblemSize(), vid);
  addToChecksum(m_x2, getActualProblemSize(), vid);
}

void IF_QUAD::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_a, vid);
  deallocData(m_b, vid);
  deallocData(m_c, vid);
  deallocData(m_x1, vid);
  deallocData(m_x2, vid);
}

} // end namespace basic
} // end namespace rajaperf
