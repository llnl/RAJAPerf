//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_MVT.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_MVT::POLYBENCH_MVT(const RunParams& params)
  : KernelBase(rajaperf::Polybench_MVT, params)
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

void POLYBENCH_MVT::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt( target_size ) + std::sqrt(2)-1;

  setActualProblemSize( m_N * m_N );
  setRunReps( target_reps );

  setItsPerRep( 2 * m_N );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N + // y1
                      1*sizeof(Real_type ) * m_N * m_N + // A

                      1*sizeof(Real_type ) * m_N + // y2
                      1*sizeof(Real_type ) * m_N * m_N ); // A
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_N + // x1

                         1*sizeof(Real_type ) * m_N ); // x2
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * m_N*m_N +
                 2 * m_N*m_N );
}

POLYBENCH_MVT::~POLYBENCH_MVT()
{
}

void POLYBENCH_MVT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_y1, m_N, vid);
  allocAndInitData(m_y2, m_N, vid);
  allocAndInitData(m_A, m_N * m_N, vid);
  allocAndInitDataConst(m_x1, m_N, 0.0, vid);
  allocAndInitDataConst(m_x2, m_N, 0.0, vid);
}

void POLYBENCH_MVT::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x1, m_N, vid);
  addToChecksum(m_x2, m_N, vid);
}

void POLYBENCH_MVT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x1, vid);
  deallocData(m_x2, vid);
  deallocData(m_y1, vid);
  deallocData(m_y2, vid);
  deallocData(m_A, vid);
}

} // end namespace polybench
} // end namespace rajaperf
