//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_JACOBI_1D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_JACOBI_1D::POLYBENCH_JACOBI_1D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_JACOBI_1D, params)
{
  Index_type N_default = 1000002;

  setDefaultProblemSize( N_default-2 );
  setDefaultReps(1600);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
#if defined(RAJA_ENABLE_TARGET_OPENMP)
  // TODO: base omp target variant result is off
  setChecksumTolerance(ChecksumTolerance::loose);
#else
  setChecksumTolerance(ChecksumTolerance::normal);
#endif

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void POLYBENCH_JACOBI_1D::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = target_size + 2;

  setActualProblemSize( m_N-2 );
  setRunReps( target_reps );

  setItsPerRep( 2 * getActualProblemSize() );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N + // A (3 point stencil)

                      1*sizeof(Real_type ) * m_N ); // B (3 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-2) + // B

                         1*sizeof(Real_type ) * (m_N-2) ); // A
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 3 * (m_N-2) +
                  3 * (m_N-2) );
}

POLYBENCH_JACOBI_1D::~POLYBENCH_JACOBI_1D()
{
}

void POLYBENCH_JACOBI_1D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_Ainit, m_N, vid);
  allocAndInitData(m_Binit, m_N, vid);
  allocData(m_A, m_N, vid);
  allocData(m_B, m_N, vid);
}

void POLYBENCH_JACOBI_1D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_A, m_N, vid);
  addToChecksum(m_B, m_N, vid);
}

void POLYBENCH_JACOBI_1D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_Ainit, vid);
  deallocData(m_Binit, vid);
}

} // end namespace polybench
} // end namespace rajaperf
