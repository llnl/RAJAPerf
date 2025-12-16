//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep( 1 );
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 );

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
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
