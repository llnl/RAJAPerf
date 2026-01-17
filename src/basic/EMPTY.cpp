//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "EMPTY.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


EMPTY::EMPTY(const RunParams& params)
  : KernelBase(rajaperf::Basic_EMPTY, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(1000);

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

void EMPTY::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep( 1 );
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 );
}

EMPTY::~EMPTY()
{
}

void EMPTY::setUp(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
}

void EMPTY::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
}

void EMPTY::tearDown(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
}

} // end namespace basic
} // end namespace rajaperf
