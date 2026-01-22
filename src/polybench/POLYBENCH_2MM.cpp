//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_2MM.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

#include <algorithm>


namespace rajaperf
{
namespace polybench
{

POLYBENCH_2MM::POLYBENCH_2MM(const RunParams& params)
  : KernelBase(rajaperf::Polybench_2MM, params)
{
  m_ni_default = 1000;
  m_nj_default = 1000;
  m_nk_default = 1120;
  m_nl_default = 1000;

  setDefaultProblemSize( std::max( m_ni_default*m_nj_default,
                                   m_ni_default*m_nl_default ) );
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

void POLYBENCH_2MM::setSize(Index_type target_size, Index_type target_reps)
{
  m_ni = std::sqrt( target_size ) + std::sqrt(2)-1;
  m_nj = m_ni;
  m_nk = Index_type(double(m_nk_default)/m_ni_default*m_ni);
  m_nl = m_ni;

  m_alpha = 1.5;
  m_beta = 1.2;

  setActualProblemSize( std::max( m_ni*m_nj, m_ni*m_nl ) );
  setRunReps( target_reps );

  setItsPerRep( m_ni*m_nj + m_ni*m_nl );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_ni * m_nk + // A
                      1*sizeof(Real_type ) * m_nj * m_nk + // B

                      1*sizeof(Real_type ) * m_ni * m_nj + // tmp
                      1*sizeof(Real_type ) * m_nj * m_nl ); // C
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_ni * m_nj + // tmp

                         1*sizeof(Real_type ) * m_ni * m_nl ); // D
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(3 * m_ni*m_nj*m_nk +
                 2 * m_ni*m_nj*m_nl );
}

POLYBENCH_2MM::~POLYBENCH_2MM()
{
}

void POLYBENCH_2MM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_tmp, m_ni * m_nj, vid);
  allocAndInitData(m_A, m_ni * m_nk, vid);
  allocAndInitData(m_B, m_nk * m_nj, vid);
  allocAndInitData(m_C, m_nj * m_nl, vid);
  allocAndInitDataConst(m_D, m_ni * m_nl, 0.0, vid);
}

void POLYBENCH_2MM::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_D, m_ni * m_nl, vid);
}

void POLYBENCH_2MM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_tmp, vid);
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_C, vid);
  deallocData(m_D, vid);
}

} // end namespace polybench
} // end namespace rajaperf
