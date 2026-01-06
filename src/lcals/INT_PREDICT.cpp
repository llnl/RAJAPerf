//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INT_PREDICT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


INT_PREDICT::INT_PREDICT(const RunParams& params)
  : KernelBase(rajaperf::Lcals_INT_PREDICT, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(400);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 10*sizeof(Real_type ) * getActualProblemSize() ); // px(12), px(11), px(10), px(9), px(8), px(7), px(6), px(4), px(5), px(2)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * getActualProblemSize() ); // px(0)
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(17 * getActualProblemSize());

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

INT_PREDICT::~INT_PREDICT()
{
}

void INT_PREDICT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_array_length = getActualProblemSize() * 13;
  m_offset = getActualProblemSize();

  m_px_initval = 1.0;
  allocAndInitDataConst(m_px, m_array_length, m_px_initval, vid);

  initData(m_dm22, vid);
  initData(m_dm23, vid);
  initData(m_dm24, vid);
  initData(m_dm25, vid);
  initData(m_dm26, vid);
  initData(m_dm27, vid);
  initData(m_dm28, vid);
  initData(m_c0, vid);
}

void INT_PREDICT::updateChecksum(VariantID vid, size_t tune_idx)
{
  Real_ptr px_host = m_px;

  DataSpace ds = getDataSpace(vid);
  DataSpace hds = rajaperf::hostCopyDataSpace(ds);
  if (ds != hds) {
    allocData(hds, px_host, m_array_length);
    copyData(hds, px_host, ds, m_px, m_array_length);
  }

  for (Index_type i = 0; i < getActualProblemSize(); ++i) {
    px_host[i] -= m_px_initval;
  }

  addToChecksum(px_host, getActualProblemSize(), vid);

  if (ds != hds) {
    copyData(ds, m_px, hds, px_host, m_array_length);
    deallocData(hds, px_host);
  }
}

void INT_PREDICT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_px, vid);
}

} // end namespace lcals
} // end namespace rajaperf
