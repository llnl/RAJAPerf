//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GEMVER.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_GEMVER::POLYBENCH_GEMVER(const RunParams& params)
  : KernelBase(rajaperf::Polybench_GEMVER, params)
{
  Index_type n_default = 1000;

  setDefaultProblemSize( n_default * n_default );
  setDefaultReps(20);

  m_n =  std::sqrt( getTargetProblemSize() ) + std::sqrt(2)-1;

  m_alpha = 1.5;
  m_beta = 1.2;


  setActualProblemSize( m_n * m_n );

  setItsPerRep( m_n*m_n +
                m_n +
                m_n +
                m_n );
  setKernelsPerRep(4);
  setBytesReadPerRep( 4*sizeof(Real_type ) * m_n + // u1, v1, u2, v2

                      1*sizeof(Real_type ) * m_n * m_n + // A
                      1*sizeof(Real_type ) * m_n + // y

                      1*sizeof(Real_type ) * m_n + // z

                      1*sizeof(Real_type ) * m_n * m_n + // A
                      2*sizeof(Real_type ) * m_n ); // x
  setBytesWrittenPerRep( 0 +

                         1*sizeof(Real_type ) * m_n + // x

                         0 +

                         1*sizeof(Real_type ) * m_n ); // w
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type ) * m_n * m_n + // A

                               0 +

                               1*sizeof(Real_type ) * m_n + // x

                               0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(4 * m_n*m_n +
                 3 * m_n*m_n +
                 1 * m_n +
                 3 * m_n*m_n );

  checksum_scale_factor = 0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics

  setComplexity(Complexity::N);

  setUsesFeature(Forall);
  setUsesFeature(Kernel);

  addVariantTunings();
}

POLYBENCH_GEMVER::~POLYBENCH_GEMVER()
{
}

void POLYBENCH_GEMVER::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  allocAndInitData(m_A, m_n * m_n, vid);
  allocAndInitData(m_u1, m_n, vid);
  allocAndInitData(m_v1, m_n, vid);
  allocAndInitData(m_u2, m_n, vid);
  allocAndInitData(m_v2, m_n, vid);
  allocAndInitDataConst(m_w, m_n, 0.0, vid);
  allocAndInitData(m_x, m_n, vid);
  allocAndInitData(m_y, m_n, vid);
  allocAndInitData(m_z, m_n, vid);
}

void POLYBENCH_GEMVER::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_w, m_n, checksum_scale_factor , vid);
}

void POLYBENCH_GEMVER::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_A, vid);
  deallocData(m_u1, vid);
  deallocData(m_v1, vid);
  deallocData(m_u2, vid);
  deallocData(m_v2, vid);
  deallocData(m_w, vid);
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
}

} // end namespace basic
} // end namespace rajaperf
