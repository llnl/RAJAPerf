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

  setActualProblemSize( m_N*m_N );
  setRunReps( target_reps );

  setItsPerRep( 2 * m_N );
  setKernelsPerRep(2);

  setBytesAllocatedPerRep( 4*sizeof(Real_type) * m_N +      // y1, y2, x1, x2
                           1*sizeof(Real_type) * m_N*m_N ); // A
  setBytesReadPerRep( 1*sizeof(Real_type) * m_N +      // y1
                      1*sizeof(Real_type) * m_N*m_N +  // A

                      1*sizeof(Real_type) * m_N +      // y2
                      1*sizeof(Real_type) * m_N*m_N ); // A
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_N +  // x1

                         1*sizeof(Real_type) * m_N ); // x2
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
  allocAndInitData(m_A, m_N*m_N, vid);
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


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void POLYBENCH_MVT::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    POLYBENCH_MVT_DATA_SETUP
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < N; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY1);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 0; j < N; ++j )) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY2);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY3);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < N; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY4);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 0; j < N; ++j )) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY5);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_MVT_BODY6);
      }

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace polybench
} // end namespace rajaperf
