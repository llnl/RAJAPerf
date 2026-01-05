//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "SORT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace algorithm
{


SORT::SORT(const RunParams& params)
  : KernelBase(rajaperf::Algorithm_SORT, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(20);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  // not useful in this case due to O(n*log(n)) algorithm
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // x
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);

  setChecksumConsistency(ChecksumConsistency::Consistent); //  // sort is not stable but values are equal if equivalent

  setComplexity(Complexity::N_logN);

  setUsesFeature(Sort);

  addVariantTunings( );
}

SORT::~SORT()
{
}

void SORT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandValue(m_x, getActualProblemSize()*getRunReps(), vid);
}

void SORT::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_x, getActualProblemSize()*getRunReps(), vid);
}

void SORT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
}

} // end namespace algorithm
} // end namespace rajaperf
