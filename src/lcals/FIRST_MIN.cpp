//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  setActualProblemSize( getTargetProblemSize() );

  m_N = getActualProblemSize();

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N ); // x
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);

  setChecksumConsistency(ChecksumConsistency::Consistent); // The loc returned is always the first of equivalent mins
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);
  setUsesFeature(Reduction);

  addVariantTunings();
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

void FIRST_MIN::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += static_cast<Checksum_type>(m_minloc);
}

void FIRST_MIN::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
}

} // end namespace lcals
} // end namespace rajaperf
