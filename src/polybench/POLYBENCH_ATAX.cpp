//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_ATAX.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_ATAX::POLYBENCH_ATAX(const RunParams& params)
  : KernelBase(rajaperf::Polybench_ATAX, params)
{
  Index_type N_default = 1000;
  setDefaultProblemSize( N_default * N_default );
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_ATAX::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt( target_size ) + std::sqrt(2)-1;

  setActualProblemSize( m_N * m_N );
  setRunReps( target_reps );

  setItsPerRep( 2 * m_N + m_N );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N +       // x
                      1*sizeof(Real_type ) * m_N * m_N + // A

                      1*sizeof(Real_type ) * m_N +        // tmp
                      1*sizeof(Real_type ) * m_N * m_N ); // A
  setBytesWrittenPerRep( 2*sizeof(Real_type ) * m_N + // y, tmp

                         0);
  setBytesModifyWrittenPerRep( 0 +

                               1*sizeof(Real_type ) * m_N ); // y
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * m_N*m_N +
                 2 * m_N*m_N );
}

POLYBENCH_ATAX::~POLYBENCH_ATAX()
{
}

void POLYBENCH_ATAX::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_tmp, m_N, vid);
  allocAndInitData(m_x, m_N, vid);
  allocAndInitData(m_A, m_N * m_N, vid);
  allocAndInitDataConst(m_y, m_N, 0.0, vid);
}

void POLYBENCH_ATAX::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_y, m_N, vid);
}

void POLYBENCH_ATAX::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_tmp, vid);
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_A, vid);
}

} // end namespace polybench
} // end namespace rajaperf
