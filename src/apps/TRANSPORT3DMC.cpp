//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "TRANSPORT3DMC.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace apps
{


TRANSPORT3DMC::TRANSPORT3DMC(const RunParams& params)
  : KernelBase(rajaperf::Apps_TRANSPORT3DMC, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(1000);

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

void TRANSPORT3DMC::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // in, out
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // in
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // out
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 );
}

TRANSPORT3DMC::~TRANSPORT3DMC()
{
}

void TRANSPORT3DMC::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_in, getActualProblemSize(), vid);
  allocAndInitDataConst(m_out, getActualProblemSize(), 0.0, vid);
}

void TRANSPORT3DMC::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_out, getActualProblemSize(), vid);
}

void TRANSPORT3DMC::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_in, vid);
  deallocData(m_out, vid);
}

} // end namespace apps
} // end namespace rajaperf
