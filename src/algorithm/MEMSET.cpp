//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MEMSET.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace algorithm
{


MEMSET::MEMSET(const RunParams& params)
  : KernelBase(rajaperf::Algorithm_MEMSET, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(100);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);

  checksum_tolerance = zero_checksum_tolerance;

  setChecksumConsistency(ChecksumConsistency::Consistent);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings( );
}

MEMSET::~MEMSET()
{
}

void MEMSET::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, getActualProblemSize(), -1.234567e89, vid);
  m_val = 0.0;
}

void MEMSET::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid].at(tune_idx) += calcChecksum(m_x, getActualProblemSize(), vid);
}

void MEMSET::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
}

} // end namespace algorithm
} // end namespace rajaperf
