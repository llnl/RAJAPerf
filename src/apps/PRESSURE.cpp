//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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

void PRESSURE::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

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

void PRESSURE::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_p_new, getActualProblemSize(), vid);
}

void PRESSURE::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_compression, vid);
  deallocData(m_bvc, vid);
  deallocData(m_p_new, vid);
  deallocData(m_e_old, vid);
  deallocData(m_vnewc, vid);
}

} // end namespace apps
} // end namespace rajaperf
