//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_3MM.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

#include <algorithm>


namespace rajaperf
{
namespace polybench
{


POLYBENCH_3MM::POLYBENCH_3MM(const RunParams& params)
  : KernelBase(rajaperf::Polybench_3MM, params)
{
  Index_type ni_default = 1000;
  Index_type nj_default = 1000;
  Index_type nk_default = 1010;
  Index_type nl_default = 1000;
  Index_type nm_default = 1200;

  setDefaultProblemSize( std::max( std::max( ni_default*nj_default,
                                             nj_default*nl_default ),
                                  ni_default*nl_default ) );
  setDefaultProblemSize( ni_default * nj_default );
  setDefaultReps(2);

  m_ni = std::sqrt( getTargetProblemSize() ) + std::sqrt(2)-1;
  m_nj = m_ni;
  m_nk = Index_type(double(nk_default)/ni_default*m_ni);
  m_nl = m_ni;
  m_nm = Index_type(double(nm_default)/ni_default*m_ni);


  setActualProblemSize( std::max( std::max( m_ni*m_nj, m_nj*m_nl ),
                                  m_ni*m_nl ) );

  setItsPerRep( m_ni*m_nj + m_nj*m_nl + m_ni*m_nl );
  setKernelsPerRep(3);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_ni * m_nk + // A
                      1*sizeof(Real_type ) * m_nj * m_nk + // B

                      1*sizeof(Real_type ) * m_nj * m_nm + // C
                      1*sizeof(Real_type ) * m_nl * m_nm + // D

                      1*sizeof(Real_type ) * m_ni * m_nj + // E
                      1*sizeof(Real_type ) * m_nj * m_nl ); // F
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_ni * m_nj + // E

                         1*sizeof(Real_type ) * m_nj * m_nl + // F

                         1*sizeof(Real_type ) * m_ni * m_nl ); // G
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * m_ni*m_nj*m_nk +
                 2 * m_nj*m_nl*m_nm +
                 2 * m_ni*m_nj*m_nl );

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.000000001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N_to_the_three_halves);

  setUsesFeature(Kernel);

  addVariantTunings();
}

POLYBENCH_3MM::~POLYBENCH_3MM()
{
}

void POLYBENCH_3MM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  allocAndInitData(m_A, m_ni * m_nk, vid);
  allocAndInitData(m_B, m_nk * m_nj, vid);
  allocAndInitData(m_C, m_nj * m_nm, vid);
  allocAndInitData(m_D, m_nm * m_nl, vid);
  allocAndInitDataConst(m_E, m_ni * m_nj, 0.0, vid);
  allocAndInitDataConst(m_F, m_nj * m_nl, 0.0, vid);
  allocAndInitDataConst(m_G, m_ni * m_nl, 0.0, vid);
}

void POLYBENCH_3MM::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_G, m_ni * m_nl, vid);
}

void POLYBENCH_3MM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_C, vid);
  deallocData(m_D, vid);
  deallocData(m_E, vid);
  deallocData(m_F, vid);
  deallocData(m_G, vid);
}

} // end namespace basic
} // end namespace rajaperf
