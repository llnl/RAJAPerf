//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 3*sizeof(Real_type ) * getActualProblemSize() ); // u, v, x
  setBytesWrittenPerRep( 2*sizeof(Real_type ) * getActualProblemSize() ); // y, w
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(4 * getActualProblemSize()); // 1 exp

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
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

void PLANCKIAN::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_w, getActualProblemSize(), vid);
}

void PLANCKIAN::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_u, vid);
  deallocData(m_v, vid);
  deallocData(m_w, vid);
}

} // end namespace lcals
} // end namespace rajaperf
