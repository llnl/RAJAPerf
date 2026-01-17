//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HYDRO_1D.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


HYDRO_1D::HYDRO_1D(const RunParams& params)
  : KernelBase(rajaperf::Lcals_HYDRO_1D, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(1000);

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

void HYDRO_1D::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  m_array_length = getActualProblemSize() + 12;

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * getActualProblemSize() + // y
                      1*sizeof(Real_type ) * (getActualProblemSize()+1) ); // z (each iterate accesses the range [i+10, i+11])
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * getActualProblemSize() ); // x
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(5 * getActualProblemSize());
}

HYDRO_1D::~HYDRO_1D()
{
}

void HYDRO_1D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_array_length, 0.0, vid);
  allocAndInitData(m_y, m_array_length, vid);
  allocAndInitData(m_z, m_array_length, vid);

  initData(m_q, vid);
  initData(m_r, vid);
  initData(m_t, vid);
}

void HYDRO_1D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x, getActualProblemSize(), vid);
}

void HYDRO_1D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
}

} // end namespace lcals
} // end namespace rajaperf
