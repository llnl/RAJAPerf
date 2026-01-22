//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GESUMMV.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_GESUMMV::POLYBENCH_GESUMMV(const RunParams& params)
  : KernelBase(rajaperf::Polybench_GESUMMV, params)
{
  Index_type N_default = 1000;
  setDefaultProblemSize( N_default * N_default );
  setDefaultReps(120);

  m_alpha = 0.62;
  m_beta = 1.002;

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

void POLYBENCH_GESUMMV::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt( target_size ) + std::sqrt(2)-1;

  setActualProblemSize( m_N * m_N );
  setRunReps( target_reps );

  setItsPerRep( m_N );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N + // x
                      2*sizeof(Real_type ) * m_N * m_N ); // A, B
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_N ); // y
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((4 * m_N +
                  3 ) * m_N  );
}

POLYBENCH_GESUMMV::~POLYBENCH_GESUMMV()
{
}

void POLYBENCH_GESUMMV::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_x, m_N, vid);
  allocAndInitDataConst(m_y, m_N, 0.0, vid);
  allocAndInitData(m_A, m_N * m_N, vid);
  allocAndInitData(m_B, m_N * m_N, vid);
}

void POLYBENCH_GESUMMV::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_y, m_N, vid);
}

void POLYBENCH_GESUMMV::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_A, vid);
  deallocData(m_B, vid);
}

} // end namespace polybench
} // end namespace rajaperf
