//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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
  m_ni_default = 1000;
  m_nj_default = 1000;
  m_nk_default = 1010;
  m_nl_default = 1000;
  m_nm_default = 1200;

  setDefaultProblemSize( std::max( std::max( m_ni_default*m_nj_default,
                                             m_nj_default*m_nl_default ),
                                  m_ni_default*m_nl_default ) );
  setDefaultProblemSize( m_ni_default * m_nj_default );
  setDefaultReps(2);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N_to_the_three_halves);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_3MM::setSize(Index_type target_size, Index_type target_reps)
{
  m_ni = std::sqrt( target_size ) + std::sqrt(2)-1;
  m_nj = m_ni;
  m_nk = Index_type(double(m_nk_default)/m_ni_default*m_ni);
  m_nl = m_ni;
  m_nm = Index_type(double(m_nm_default)/m_ni_default*m_ni);

  setActualProblemSize( std::max( std::max( m_ni*m_nj, m_nj*m_nl ),
                                  m_ni*m_nl ) );
  setRunReps( target_reps );

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
}

POLYBENCH_3MM::~POLYBENCH_3MM()
{
}

void POLYBENCH_3MM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_A, m_ni * m_nk, vid);
  allocAndInitData(m_B, m_nk * m_nj, vid);
  allocAndInitData(m_C, m_nj * m_nm, vid);
  allocAndInitData(m_D, m_nm * m_nl, vid);
  allocAndInitDataConst(m_E, m_ni * m_nj, 0.0, vid);
  allocAndInitDataConst(m_F, m_nj * m_nl, 0.0, vid);
  allocAndInitDataConst(m_G, m_ni * m_nl, 0.0, vid);
}

void POLYBENCH_3MM::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_G, m_ni * m_nl, vid);
}

void POLYBENCH_3MM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
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
