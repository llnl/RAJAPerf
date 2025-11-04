//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_FLOYD_WARSHALL.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_FLOYD_WARSHALL::POLYBENCH_FLOYD_WARSHALL(const RunParams& params)
  : KernelBase(rajaperf::Polybench_FLOYD_WARSHALL, params)
{
  Index_type N_default = 1000;
  setDefaultProblemSize( N_default * N_default );
  setDefaultReps(8);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::tight);

  setComplexity(Complexity::N_to_the_three_halves);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_FLOYD_WARSHALL::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::sqrt( target_size ) + std::sqrt(2)-1;

  setActualProblemSize( m_N * m_N );
  setRunReps( target_reps );

  setItsPerRep( m_N * m_N*m_N );
  setKernelsPerRep(m_N);

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * m_N*m_N ); // pin, pout
  setBytesReadPerRep( m_N * 1*sizeof(Real_type) * m_N*m_N ); // pin
  setBytesWrittenPerRep( m_N * 1*sizeof(Real_type) * m_N*m_N ); // pout
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( m_N*m_N*m_N * 3 / 2 ); // conditional is true about half of the time
}

POLYBENCH_FLOYD_WARSHALL::~POLYBENCH_FLOYD_WARSHALL()
{
}

void POLYBENCH_FLOYD_WARSHALL::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_pin, m_N*m_N, vid);
  allocAndInitDataConst(m_pout, m_N*m_N, 0.0, vid);
}

void POLYBENCH_FLOYD_WARSHALL::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_pout, m_N*m_N, vid);
}

void POLYBENCH_FLOYD_WARSHALL::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_pin, vid);
  deallocData(m_pout, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void POLYBENCH_FLOYD_WARSHALL::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    POLYBENCH_FLOYD_WARSHALL_DATA_SETUP
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type k = 0; k < N; ++k)) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < N; ++i)) {
          RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = 0; j < N; ++j)) {
            RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_FLOYD_WARSHALL_BODY);
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

} // end namespace polybench
} // end namespace rajaperf
