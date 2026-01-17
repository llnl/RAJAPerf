//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIRST_MIN.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


FIRST_MIN::FIRST_MIN(const RunParams& params)
  : KernelBase(rajaperf::Lcals_FIRST_MIN, params)
{
  setDefaultProblemSize(1000000);
//setDefaultReps(1000);
// Set reps to low value until we resolve RAJA omp-target
// reduction performance issues
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent); // The loc returned is always the first of equivalent mins
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Reduction);

  addVariantTunings();
}

void FIRST_MIN::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  m_N = getActualProblemSize();

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N ); // x
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

FIRST_MIN::~FIRST_MIN()
{
}

void FIRST_MIN::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  auto reset_x = allocAndInitDataConstForInit(m_x, m_N, 0.0, vid);

  m_x[ m_N / 2 ] = -1.0e+10;
  m_xmin_init = m_x[0];

  m_initloc = 0;
  m_minloc = -1;
}

void FIRST_MIN::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_minloc);
}

void FIRST_MIN::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
}

} // end namespace lcals
} // end namespace rajaperf
