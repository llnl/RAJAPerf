//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_ADI.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace polybench
{


POLYBENCH_ADI::POLYBENCH_ADI(const RunParams& params)
  : KernelBase(rajaperf::Polybench_ADI, params)
  , m_tsteps(4)
{
  Index_type n_default = 1002;
  setDefaultProblemSize( (n_default-2) * (n_default-2) );
  setDefaultReps(4 * m_tsteps);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_ADI::setSize(Index_type target_size, Index_type target_reps)
{
  m_n = std::sqrt( target_size ) + 2 + std::sqrt(2)-1;

  setActualProblemSize( (m_n-2) * (m_n-2) );
  setRunReps( target_reps );

  setItsPerRep( 2 * (m_n-2) + (m_n-2) );
  setKernelsPerRep( 2 );

  setBytesAllocatedPerRep( 4*sizeof(Real_type) * m_n * m_n ); // u, v, p, q
  setBytesReadPerRep( 1*sizeof(Real_type) * (m_n-2) * (m_n  ) + // u

                      1*sizeof(Real_type) * (m_n-2) * (m_n  ) ); // v
  setBytesWrittenPerRep( 2*sizeof(Real_type) * (m_n-2) * (    1) + // p, q
                         1*sizeof(Real_type) * (m_n-2) * (m_n  ) + // v

                         2*sizeof(Real_type) * (m_n-2) * (    1) + // p, q
                         1*sizeof(Real_type) * (m_n-2) * (m_n  ) ); // u
  setBytesModifyWrittenPerRep( 2*sizeof(Real_type) * (m_n-2) * (m_n-2) + // p, q

                               2*sizeof(Real_type) * (m_n-2) * (m_n-2) ); // p, q
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( (13 + 2) * (m_n-2)*(m_n-2) +
                  (13 + 2) * (m_n-2)*(m_n-2) );
}

POLYBENCH_ADI::~POLYBENCH_ADI()
{
}

void POLYBENCH_ADI::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_U, m_n * m_n, 0.0, vid);
  allocAndInitData(m_V, m_n * m_n, vid);
  allocAndInitData(m_P, m_n * m_n, vid);
  allocAndInitData(m_Q, m_n * m_n, vid);
}

void POLYBENCH_ADI::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_U, m_n * m_n, vid);
}

void POLYBENCH_ADI::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_U, vid);
  deallocData(m_V, vid);
  deallocData(m_P, vid);
  deallocData(m_Q, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void POLYBENCH_ADI::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    POLYBENCH_ADI_DATA_SETUP
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 1; i < n-1; ++i)) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY2);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 1; j < n-1; ++j)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY3);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY4);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type k = n-2; k >= 1; --k)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY5);
        }
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 1; i < n-1; ++i)) {
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY6);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type j = 1; j < n-1; ++j)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY7);
        }
        RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY8);
        RAJAPERF_COUNTERS_SEQ_LOOP(for (Index_type k = n-2; k >= 1; --k)) {
          RAJAPERF_COUNTERS_LOOP_BODY(POLYBENCH_ADI_OPT_BODY9);
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
