//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "COPY8.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


COPY8::COPY8(const RunParams& params)
  : KernelBase(rajaperf::Basic_COPY8, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent);
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void COPY8::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 8*sizeof(Real_type) * getActualProblemSize() ); // x0, x1, x2, x3, x4, x5, x6, x7
  setBytesWrittenPerRep( 8*sizeof(Real_type) * getActualProblemSize() ); // y0, y1, y2, y3, y4, y5, y6, y7
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

COPY8::~COPY8()
{
}

void COPY8::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_y0, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y1, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y2, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y3, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y4, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y5, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y6, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_y7, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_x0, getActualProblemSize(), vid);
  allocAndInitData(m_x1, getActualProblemSize(), vid);
  allocAndInitData(m_x2, getActualProblemSize(), vid);
  allocAndInitData(m_x3, getActualProblemSize(), vid);
  allocAndInitData(m_x4, getActualProblemSize(), vid);
  allocAndInitData(m_x5, getActualProblemSize(), vid);
  allocAndInitData(m_x6, getActualProblemSize(), vid);
  allocAndInitData(m_x7, getActualProblemSize(), vid);
}

void COPY8::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_y0, getActualProblemSize(), vid);
  addToChecksum(m_y1, getActualProblemSize(), vid);
  addToChecksum(m_y2, getActualProblemSize(), vid);
  addToChecksum(m_y3, getActualProblemSize(), vid);
  addToChecksum(m_y4, getActualProblemSize(), vid);
  addToChecksum(m_y5, getActualProblemSize(), vid);
  addToChecksum(m_y6, getActualProblemSize(), vid);
  addToChecksum(m_y7, getActualProblemSize(), vid);
}

void COPY8::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x0, vid);
  deallocData(m_x1, vid);
  deallocData(m_x2, vid);
  deallocData(m_x3, vid);
  deallocData(m_x4, vid);
  deallocData(m_x5, vid);
  deallocData(m_x6, vid);
  deallocData(m_x7, vid);
  deallocData(m_y0, vid);
  deallocData(m_y1, vid);
  deallocData(m_y2, vid);
  deallocData(m_y3, vid);
  deallocData(m_y4, vid);
  deallocData(m_y5, vid);
  deallocData(m_y6, vid);
  deallocData(m_y7, vid);
}

} // end namespace basic
} // end namespace rajaperf
