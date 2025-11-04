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

  setActualProblemSize(m_N*m_N);
  setRunReps( target_reps );

  setItsPerRep( num_tiles*num_tiles * TL_SZ*TL_SZ );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 3*sizeof(Real_type) * m_N*m_N ); // A, B, C
  setBytesReadPerRep( 2*sizeof(Real_type) * m_N*m_N ); // A, B
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_N*m_N  ); // C
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(2 * TL_SZ * TL_SZ * TL_SZ * num_tiles * num_tiles * num_tiles);
}

MAT_MAT_SHARED::~MAT_MAT_SHARED() {}

void MAT_MAT_SHARED::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_A, m_N*m_N, 1.0, vid);
  allocAndInitDataConst(m_B, m_N*m_N, 1.0, vid);
  allocAndInitDataConst(m_C, m_N*m_N, 0.0, vid);
}

void MAT_MAT_SHARED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_C, m_N*m_N, vid);
}

void MAT_MAT_SHARED::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_C, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void MAT_MAT_SHARED::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    const Index_type N = m_N;

    MAT_MAT_SHARED_DATA_SETUP;
    const Index_type Nx = RAJA_DIVIDE_CEILING_INT(N, TL_SZ);
    const Index_type Ny = RAJA_DIVIDE_CEILING_INT(N, TL_SZ);
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type by = 0; by < Ny; ++by)) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type bx = 0; bx < Nx; ++bx)) {
          RAJAPERF_COUNTERS_TEAM_CONTEXT();

          //Work around for when compiling with CLANG and HIP
          //See notes in MAT_MAT_SHARED.hpp
          RAJAPERF_COUNTERS_LOOP_BODY(MAT_MAT_SHARED_BODY_0_CLANG_HIP_CPU(TL_SZ));

          RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type ty = 0; ty < TL_SZ; ++ty)) {
            RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type tx = 0; tx < TL_SZ; ++tx)) {
              RAJAPERF_COUNTERS_LOOP_BODY(MAT_MAT_SHARED_BODY_1(TL_SZ));
            }
          }

          RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type k = 0; k < (TL_SZ + N - 1) / TL_SZ; ++k)) {

            RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type ty = 0; ty < TL_SZ; ++ty)) {
              RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type tx = 0; tx < TL_SZ; ++tx)) {
                RAJAPERF_COUNTERS_LOOP_BODY(MAT_MAT_SHARED_BODY_2(TL_SZ));
              }
            }

            RAJAPERF_COUNTERS_TEAM_SYNC();

            RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type ty = 0; ty < TL_SZ; ++ty)) {
              RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type tx = 0; tx < TL_SZ; ++tx)) {
                RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type n = 0; n < TL_SZ; ++n)) {
                  RAJAPERF_COUNTERS_LOOP_BODY(MAT_MAT_SHARED_BODY_3(TL_SZ));
                }
              }
            }

            RAJAPERF_COUNTERS_TEAM_SYNC();

          } // Sequential loop

          RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type ty = 0; ty < TL_SZ; ++ty)) {
            RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type tx = 0; tx < TL_SZ; ++tx)) {
              RAJAPERF_COUNTERS_LOOP_BODY(MAT_MAT_SHARED_BODY_4(TL_SZ));
            }
          }
        }
      }

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace basic
} // end namespace rajaperf
