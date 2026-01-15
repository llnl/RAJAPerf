//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GEMM.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_GEMM::POLYBENCH_GEMM(const RunParams& params)
  : KernelBase(rajaperf::Polybench_GEMM, params)
{
  m_ni_default = 1000;
  m_nj_default = 1000;
  m_nk_default = 1200;
  setDefaultProblemSize( m_ni_default * m_nj_default );
  setDefaultReps(4);

  m_alpha = 0.62;
  m_beta = 1.002;

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

void POLYBENCH_GEMM::setSize(Index_type target_size, Index_type target_reps)
{
  m_ni = std::sqrt( target_size ) + std::sqrt(2)-1;
  m_nj = m_ni;
  m_nk = Index_type(double(m_nk_default)/m_ni_default*m_ni);

  setActualProblemSize( m_ni * m_nj );
  setRunReps( target_reps );

  setItsPerRep( m_ni * m_nj );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_ni * m_nk + // A
                      1*sizeof(Real_type ) * m_nj * m_nk ); // B
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_ni * m_nj); // C
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((1 +
                  3 * m_nk) * m_ni*m_nj);
}

POLYBENCH_GEMM::~POLYBENCH_GEMM()
{
}

void POLYBENCH_GEMM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_A, m_ni * m_nk, vid);
  allocAndInitData(m_B, m_nk * m_nj, vid);
  allocAndInitDataConst(m_C, m_ni * m_nj, 0.0, vid);
}

void POLYBENCH_GEMM::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_C, m_ni * m_nj, vid);
}

void POLYBENCH_GEMM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_C, vid);
}

} // end namespace polybench
} // end namespace rajaperf
