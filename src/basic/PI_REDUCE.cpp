//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "PI_REDUCE.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


PI_REDUCE::PI_REDUCE(const RunParams& params)
  : KernelBase(rajaperf::Basic_PI_REDUCE, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Inconsistent);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Reduction);

  addVariantTunings();
}

void PI_REDUCE::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(6 * getActualProblemSize() + 1);
}

PI_REDUCE::~PI_REDUCE()
{
}

void PI_REDUCE::setUp(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_dx = 1.0 / double(getActualProblemSize());
  m_pi_init = 0.0;
  m_pi = 0.0;
}

void PI_REDUCE::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_pi);
}

void PI_REDUCE::tearDown(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

}

} // end namespace basic
} // end namespace rajaperf
