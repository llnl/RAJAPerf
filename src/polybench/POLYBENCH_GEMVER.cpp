//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GEMVER.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_GEMVER::POLYBENCH_GEMVER(const RunParams& params)
  : KernelBase(rajaperf::Polybench_GEMVER, params)
{
  Index_type n_default = 1000;
  setDefaultProblemSize( n_default * n_default );
  setDefaultReps(20);

  m_alpha = 1.5;
  m_beta = 1.2;

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Forall);
  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_GEMVER::setSize(Index_type target_size, Index_type target_reps)
{
  m_n =  std::sqrt( target_size ) + std::sqrt(2)-1;

  setActualProblemSize( m_n*m_n );
  setRunReps( target_reps );

  setItsPerRep( m_n*m_n +
                m_n +
                m_n +
                m_n );
  setKernelsPerRep(4);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * m_n*m_n + // A
                           8*sizeof(Real_type) * m_n );    // u1, v1, u2, v2, w, x, y, z
  setBytesReadPerRep( 4*sizeof(Real_type) * m_n +     // u1, v1, u2, v2

                      1*sizeof(Real_type) * m_n*m_n + // A
                      1*sizeof(Real_type) * m_n +     // y

                      1*sizeof(Real_type) * m_n +     // z

                      1*sizeof(Real_type) * m_n*m_n + // A
                      2*sizeof(Real_type) * m_n );    // x
  setBytesWrittenPerRep( 0 +

                         1*sizeof(Real_type) * m_n +  // x

                         0 +

                         1*sizeof(Real_type) * m_n ); // w
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * m_n*m_n + // A

                               0 +

                               1*sizeof(Real_type) * m_n +     // x

                               0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(4 * m_n*m_n +
                 3 * m_n*m_n +
                 1 * m_n +
                 3 * m_n*m_n );
}

POLYBENCH_GEMVER::~POLYBENCH_GEMVER()
{
}

void POLYBENCH_GEMVER::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_A, m_n*m_n, vid);
  allocAndInitData(m_u1, m_n, vid);
  allocAndInitData(m_v1, m_n, vid);
  allocAndInitData(m_u2, m_n, vid);
  allocAndInitData(m_v2, m_n, vid);
  allocAndInitDataConst(m_w, m_n, 0.0, vid);
  allocAndInitData(m_x, m_n, vid);
  allocAndInitData(m_y, m_n, vid);
  allocAndInitData(m_z, m_n, vid);
}

void POLYBENCH_GEMVER::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_w, m_n, vid);
}

void POLYBENCH_GEMVER::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_u1, vid);
  deallocData(m_v1, vid);
  deallocData(m_u2, vid);
  deallocData(m_v2, vid);
  deallocData(m_w, vid);
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void POLYBENCH_GEMVER::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    POLYBENCH_GEMVER_DATA_SETUP
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < n; i++ )) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = 0; j < n; j++)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY1);
        }
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < n; i++ )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY2);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 0; j < n; j++)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY3);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY4);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < n; i++ )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY5);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < n; i++ )) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY6);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 0; j < n; j++)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY7);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_GEMVER_BODY8);
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
