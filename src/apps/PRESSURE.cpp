//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "PRESSURE.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace apps
{


PRESSURE::PRESSURE(const RunParams& params)
  : KernelBase(rajaperf::Apps_PRESSURE, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(700);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( 2 * getActualProblemSize() );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // bvc
                      3*sizeof(Real_type) * getActualProblemSize() ); // bvc, e_old, vnewc
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // compression
                         1*sizeof(Real_type) * getActualProblemSize() ); // p_new
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((2 +
                  1
                  ) * getActualProblemSize());

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

PRESSURE::~PRESSURE()
{
}

void PRESSURE::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_compression, getActualProblemSize(), vid);
  allocAndInitData(m_bvc, getActualProblemSize(), vid);
  allocAndInitDataConst(m_p_new, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_e_old, getActualProblemSize(), vid);
  allocAndInitData(m_vnewc, getActualProblemSize(), vid);

  initData(m_cls, vid);
  initData(m_p_cut, vid);
  initData(m_pmin, vid);
  initData(m_eosvmax, vid);
}

void PRESSURE::updateChecksum(VariantID vid, size_t tune_idx)
{
  addToChecksum(m_p_new, getActualProblemSize(), vid);
}

void PRESSURE::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  deallocData(m_compression, vid);
  deallocData(m_bvc, vid);
  deallocData(m_p_new, vid);
  deallocData(m_e_old, vid);
  deallocData(m_vnewc, vid);
}

} // end namespace apps
} // end namespace rajaperf
