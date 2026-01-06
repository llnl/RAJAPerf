//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // in1, in2
  setBytesWrittenPerRep( 3*sizeof(Real_type) * getActualProblemSize()  ); // out1, out2, out3
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(3 * getActualProblemSize());

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::tight);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
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

void MULADDSUB::updateChecksum(VariantID vid, size_t tune_idx)
{
  addToChecksum(m_out1, getActualProblemSize(), vid);
  addToChecksum(m_out2, getActualProblemSize(), vid);
  addToChecksum(m_out3, getActualProblemSize(), vid);
}

void MULADDSUB::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_out1, vid);
  deallocData(m_out2, vid);
  deallocData(m_out3, vid);
  deallocData(m_in1, vid);
  deallocData(m_in2, vid);
}

} // end namespace basic
} // end namespace rajaperf
