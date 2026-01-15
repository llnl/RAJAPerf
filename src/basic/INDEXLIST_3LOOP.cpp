//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-22, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INDEXLIST_3LOOP.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


INDEXLIST_3LOOP::INDEXLIST_3LOOP(const RunParams& params)
  : KernelBase(rajaperf::Basic_INDEXLIST_3LOOP, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent);
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Scan);

  addVariantTunings();
}

void INDEXLIST_3LOOP::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( 3 * getActualProblemSize() + 1 );
  setKernelsPerRep(3);
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // x
                      0 +
                      1*sizeof(Index_type) * (getActualProblemSize()+1) ); // counts
  setBytesWrittenPerRep( 1*sizeof(Index_type) * getActualProblemSize() + // counts
                         0 +
                         1*sizeof(Int_type) * (getActualProblemSize()+1) / 2 ); // list (about 50% output)
  setBytesModifyWrittenPerRep( 0 +
                               1*sizeof(Index_type) * (getActualProblemSize()+1) + // counts
                               0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

INDEXLIST_3LOOP::~INDEXLIST_3LOOP()
{
}

void INDEXLIST_3LOOP::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_x, getActualProblemSize(), vid);
  allocAndInitData(m_list, getActualProblemSize(), vid);
  m_len = -1;
}

void INDEXLIST_3LOOP::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_list, getActualProblemSize(), vid);
  addToChecksum(m_len);
}

void INDEXLIST_3LOOP::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_list, vid);
}

} // end namespace basic
} // end namespace rajaperf
