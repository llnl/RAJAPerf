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

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * m_N ); // A, B
  setBytesReadPerRep( 1*sizeof(Real_type) * m_N + // A (3 point stencil)

                      1*sizeof(Real_type) * m_N ); // B (3 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * (m_N-2) + // B

                         1*sizeof(Real_type) * (m_N-2) ); // A
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
  allocAndInitData(m_A, m_N, vid);
  allocAndInitData(m_B, m_N, vid);
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
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void POLYBENCH_JACOBI_1D::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    POLYBENCH_JACOBI_1D_DATA_SETUP;

    const Index_type iend = N-1;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 1; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_JACOBI_1D_BODY1);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 1; i < iend; ++i)) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_JACOBI_1D_BODY2);
      }

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, 0);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace polybench
} // end namespace rajaperf
