//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "TRIDIAG_ELIM.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


TRIDIAG_ELIM::TRIDIAG_ELIM(const RunParams& params)
  : KernelBase(rajaperf::Lcals_TRIDIAG_ELIM, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(1000);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void TRIDIAG_ELIM::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( std::max(target_size, Index_type(2)) );
  setRunReps( target_reps );

  m_N = getActualProblemSize();

  setItsPerRep( m_N-1 );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 4*sizeof(Real_type) * m_N ); // xout, xin, y, z
  setBytesReadPerRep( 3*sizeof(Real_type) * (m_N-1) ); // z, y, xin
  setBytesWrittenPerRep( 1*sizeof(Real_type) * (m_N-1) ); // xout
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * (m_N-1));
}

TRIDIAG_ELIM::~TRIDIAG_ELIM()
{
}

void TRIDIAG_ELIM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_xout, m_N, 0.0, vid);
  allocAndInitData(m_xin, m_N, vid);
  allocAndInitData(m_y, m_N, vid);
  allocAndInitData(m_z, m_N, vid);
}

void TRIDIAG_ELIM::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_xout, m_N, vid);
}

void TRIDIAG_ELIM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_xout, vid);
  deallocData(m_xin, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void TRIDIAG_ELIM::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    const Index_type ibegin = 1;
    const Index_type iend = m_N;

    TRIDIAG_ELIM_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(TRIDIAG_ELIM_BODY);
      }

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace lcals
} // end namespace rajaperf
