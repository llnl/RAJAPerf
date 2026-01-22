//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_JACOBI_2D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_JACOBI_2D::POLYBENCH_JACOBI_2D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_JACOBI_2D, params)
{
  Index_type N_default = 1002;

  setDefaultProblemSize( (N_default-2)*(N_default-2) );
  setDefaultReps(2000);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_JACOBI_2D::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt( target_size ) + 2 + std::sqrt(2)-1;

  setActualProblemSize( (m_N-2) * (m_N-2) );
  setRunReps( target_reps );

  setItsPerRep( 2 * (m_N-2) * (m_N-2) );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * (m_N * m_N - 4) + // A (5 point stencil)

                      1*sizeof(Real_type ) * (m_N * m_N - 4) ); // B (5 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-2) * (m_N-2) + // B

                         1*sizeof(Real_type ) * (m_N-2) * (m_N-2) ); // A
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 5 * (m_N-2)*(m_N-2) +
                  5 * (m_N-2)*(m_N-2) );
}

POLYBENCH_JACOBI_2D::~POLYBENCH_JACOBI_2D()
{
}

void POLYBENCH_JACOBI_2D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_Ainit, m_N*m_N, vid);
  allocAndInitData(m_Binit, m_N*m_N, vid);
  allocData(m_A, m_N*m_N, vid);
  allocData(m_B, m_N*m_N, vid);
}

void POLYBENCH_JACOBI_2D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_A, m_N*m_N, vid);
  addToChecksum(m_B, m_N*m_N, vid);
}

void POLYBENCH_JACOBI_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_Ainit, vid);
  deallocData(m_Binit, vid);
}

} // end namespace polybench
} // end namespace rajaperf
