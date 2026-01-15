//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-20, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MAT_MAT_SHARED.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf {
namespace basic {

MAT_MAT_SHARED::MAT_MAT_SHARED(const RunParams &params)
    : KernelBase(rajaperf::Basic_MAT_MAT_SHARED, params)
{
  m_N_default = 1000;
  setDefaultProblemSize(m_N_default*m_N_default);
  setDefaultReps(5);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N_to_the_three_halves);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Launch);

  addVariantTunings();
}

void MAT_MAT_SHARED::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt(target_size) + std::sqrt(2)-1;
  const Index_type num_tiles = RAJA_DIVIDE_CEILING_INT(m_N, TL_SZ);

  setActualProblemSize(m_N * m_N);
  setRunReps( target_reps );

  setItsPerRep( num_tiles*num_tiles * TL_SZ*TL_SZ );
  setKernelsPerRep(1);

  setBytesReadPerRep( 2*sizeof(Real_type) * m_N*m_N ); // A, B
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_N*m_N  ); // C
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(2 * TL_SZ * TL_SZ * TL_SZ * num_tiles * num_tiles * num_tiles);
}

MAT_MAT_SHARED::~MAT_MAT_SHARED() {}

void MAT_MAT_SHARED::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  const Index_type NN = m_N * m_N;

  allocAndInitDataConst(m_A, NN, 1.0, vid);
  allocAndInitDataConst(m_B, NN, 1.0, vid);
  allocAndInitDataConst(m_C, NN, 0.0, vid);
}

void MAT_MAT_SHARED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  addToChecksum(m_C, m_N*m_N, vid);
}

void MAT_MAT_SHARED::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_C, vid);
}

} // end namespace basic
} // end namespace rajaperf
