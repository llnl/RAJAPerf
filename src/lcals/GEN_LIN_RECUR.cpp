//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "GEN_LIN_RECUR.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


GEN_LIN_RECUR::GEN_LIN_RECUR(const RunParams& params)
  : KernelBase(rajaperf::Lcals_GEN_LIN_RECUR, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(500);

  setActualProblemSize( getTargetProblemSize() );

  m_N = getActualProblemSize();

  setItsPerRep( 2 * m_N );
  setKernelsPerRep(2);
  setBytesReadPerRep( 2*sizeof(Real_type ) * m_N + // sa, sb
                      2*sizeof(Real_type ) * m_N ); // sa, sb
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_N + // b5
                         1*sizeof(Real_type ) * m_N ); // b5
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type ) * m_N + // stb5
                               1*sizeof(Real_type ) * m_N ); // stb5
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((3 +
                  3 ) * m_N);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.01 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

GEN_LIN_RECUR::~GEN_LIN_RECUR()
{
}

void GEN_LIN_RECUR::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_kb5i = 0;

  allocAndInitDataConst(m_b5, m_N, 0.0, vid);
  allocAndInitData(m_stb5, m_N, vid);
  allocAndInitData(m_sa, m_N, vid);
  allocAndInitData(m_sb, m_N, vid);
}

void GEN_LIN_RECUR::updateChecksum(VariantID vid, size_t tune_idx)
{
  addToChecksum(m_b5, getActualProblemSize(), vid);
}

void GEN_LIN_RECUR::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_b5, vid);
  deallocData(m_stb5, vid);
  deallocData(m_sa, vid);
  deallocData(m_sb, vid);
}

} // end namespace lcals
} // end namespace rajaperf
