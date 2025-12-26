//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_ADI.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace polybench
{


POLYBENCH_ADI::POLYBENCH_ADI(const RunParams& params)
  : KernelBase(rajaperf::Polybench_ADI, params)
  , m_tsteps(4)
{
  Index_type n_default = 1002;

  setDefaultProblemSize( (n_default-2) * (n_default-2) );
  setDefaultReps(4 * m_tsteps);

  m_n = std::sqrt( getTargetProblemSize() ) + 2 + std::sqrt(2)-1;

  setItsPerRep( 2 * (m_n-2) + (m_n-2) );


  setActualProblemSize( (m_n-2) * (m_n-2) );

  setKernelsPerRep( 2 );
  setBytesReadPerRep( 1*sizeof(Real_type ) * (m_n-2) * (m_n  ) +
                      2*sizeof(Real_type ) * (m_n-2) * (m_n-2) +

                      1*sizeof(Real_type ) * (m_n-2) * (m_n  ) +
                      2*sizeof(Real_type ) * (m_n-2) * (m_n-2) );
  setBytesWrittenPerRep( 2*sizeof(Real_type ) * (m_n-2) * (m_n-1) +
                         1*sizeof(Real_type ) * (m_n-2) * (m_n  ) +

                         2*sizeof(Real_type ) * (m_n-2) * (m_n-1) +
                         1*sizeof(Real_type ) * (m_n-2) * (m_n  ) );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( (13 + 2) * (m_n-2)*(m_n-2) +
                  (13 + 2) * (m_n-2)*(m_n-2) );

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.0000001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N);

  setUsesFeature(Kernel);

  addVariantTunings();
}

POLYBENCH_ADI::~POLYBENCH_ADI()
{
}

void POLYBENCH_ADI::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_U, m_n * m_n, 0.0, vid);
  allocAndInitData(m_V, m_n * m_n, vid);
  allocAndInitData(m_P, m_n * m_n, vid);
  allocAndInitData(m_Q, m_n * m_n, vid);
}

void POLYBENCH_ADI::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_U, m_n * m_n, checksum_scale_factor , vid);
}

void POLYBENCH_ADI::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_U, vid);
  deallocData(m_V, vid);
  deallocData(m_P, vid);
  deallocData(m_Q, vid);
}

} // end namespace polybench
} // end namespace rajaperf
