//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "IF_QUAD.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


IF_QUAD::IF_QUAD(const RunParams& params)
  : KernelBase(rajaperf::Basic_IF_QUAD, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(180);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 3*sizeof(Real_type) * getActualProblemSize() ); // b, a, c
  setBytesWrittenPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // x2, x1
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  // estimate conditional true half of the time, 1 sqrt
  setFLOPsPerRep(4 * getActualProblemSize() +
                 7 * getActualProblemSize() / 2);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.0001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

IF_QUAD::~IF_QUAD()
{
}

void IF_QUAD::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_a, getActualProblemSize(), vid);
  allocAndInitData(m_b, getActualProblemSize(), vid);
  allocAndInitData(m_c, getActualProblemSize(), vid);
  allocAndInitDataConst(m_x1, getActualProblemSize(), 0.0, vid);
  allocAndInitDataConst(m_x2, getActualProblemSize(), 0.0, vid);
}

void IF_QUAD::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_x1, getActualProblemSize(), vid);
  checksum[vid][tune_idx] += calcChecksum(m_x2, getActualProblemSize(), vid);
}

void IF_QUAD::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_a, vid);
  deallocData(m_b, vid);
  deallocData(m_c, vid);
  deallocData(m_x1, vid);
  deallocData(m_x2, vid);
}

} // end namespace basic
} // end namespace rajaperf
