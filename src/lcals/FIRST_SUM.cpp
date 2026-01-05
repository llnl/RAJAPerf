//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIRST_SUM.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


FIRST_SUM::FIRST_SUM(const RunParams& params)
  : KernelBase(rajaperf::Lcals_FIRST_SUM, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(2000);

  setActualProblemSize( std::max(getTargetProblemSize(), Index_type(2)) );

  m_N = getActualProblemSize();

  setItsPerRep( m_N-1 );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N ); // y (each iterate accesses the range [i-1, i])
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-1) ); // x
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(1 * (m_N-1));

  setChecksumConsistency(ChecksumConsistency::Consistent); // assumes FP ops get the same answer across platforms

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

FIRST_SUM::~FIRST_SUM()
{
}

void FIRST_SUM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_N, 0.0, vid);
  allocAndInitData(m_y, m_N, vid);
}

void FIRST_SUM::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_x, getActualProblemSize(), vid);
}

void FIRST_SUM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
  deallocData(m_y, vid);
}

} // end namespace lcals
} // end namespace rajaperf
