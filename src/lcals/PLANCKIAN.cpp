//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "PLANCKIAN.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


PLANCKIAN::PLANCKIAN(const RunParams& params)
  : KernelBase(rajaperf::Lcals_PLANCKIAN, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

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

void PLANCKIAN::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 3*sizeof(Real_type ) * getActualProblemSize() ); // u, v, x
  setBytesWrittenPerRep( 2*sizeof(Real_type ) * getActualProblemSize() ); // y, w
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(4 * getActualProblemSize()); // 1 exp
}

PLANCKIAN::~PLANCKIAN()
{
}

void PLANCKIAN::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_x, getActualProblemSize(), vid);
  allocAndInitData(m_y, getActualProblemSize(), vid);
  allocAndInitData(m_u, getActualProblemSize(), vid);
  allocAndInitData(m_v, getActualProblemSize(), vid);
  allocAndInitDataConst(m_w, getActualProblemSize(), 0.0, vid);
}

void PLANCKIAN::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_w, getActualProblemSize(), vid);
}

void PLANCKIAN::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_u, vid);
  deallocData(m_v, vid);
  deallocData(m_w, vid);
}

} // end namespace lcals
} // end namespace rajaperf
