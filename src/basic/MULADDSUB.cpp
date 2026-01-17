//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MULADDSUB.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


MULADDSUB::MULADDSUB(const RunParams& params)
  : KernelBase(rajaperf::Basic_MULADDSUB, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(350);

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

void MULADDSUB::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // in1, in2
  setBytesWrittenPerRep( 3*sizeof(Real_type) * getActualProblemSize()  ); // out1, out2, out3
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(3 * getActualProblemSize());
}

MULADDSUB::~MULADDSUB()
{
}

void MULADDSUB::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_out1, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_out2, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_out3, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_in1, getActualProblemSize(), vid);
  allocAndInitData(m_in2, getActualProblemSize(), vid);
}

void MULADDSUB::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_out1, getActualProblemSize(), vid);
  addToChecksum(m_out2, getActualProblemSize(), vid);
  addToChecksum(m_out3, getActualProblemSize(), vid);
}

void MULADDSUB::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_out1, vid);
  deallocData(m_out2, vid);
  deallocData(m_out3, vid);
  deallocData(m_in1, vid);
  deallocData(m_in2, vid);
}

} // end namespace basic
} // end namespace rajaperf
