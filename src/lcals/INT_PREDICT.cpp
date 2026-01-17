//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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

void INT_PREDICT::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 10*sizeof(Real_type ) * getActualProblemSize() ); // px(12), px(11), px(10), px(9), px(8), px(7), px(6), px(4), px(5), px(2)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * getActualProblemSize() ); // px(0)
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(17 * getActualProblemSize());
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

void INT_PREDICT::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
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
  deallocData(m_px, vid);
}

} // end namespace lcals
} // end namespace rajaperf
