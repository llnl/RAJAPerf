//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "PI_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


PI_ATOMIC::PI_ATOMIC(const RunParams& params)
  : KernelBase(rajaperf::Basic_PI_ATOMIC, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Inconsistent);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Atomic);

  addVariantTunings();
}

void PI_ATOMIC::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) ); // pi
  setBytesReadPerRep( 0 );
  setBytesWrittenPerRep( 0  );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 1*sizeof(Real_type) ); // pi
  setFLOPsPerRep(6 * getActualProblemSize() + 1);
}

PI_ATOMIC::~PI_ATOMIC()
{
}

void PI_ATOMIC::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_dx = 1.0 / double(getActualProblemSize());
  m_pi_init = 0.0;
  m_pi_final = -static_cast<int>(vid);
}

void PI_ATOMIC::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_pi_final);
}

void PI_ATOMIC::tearDown(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void PI_ATOMIC::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    const Index_type ibegin = 0;
    const Index_type iend = getActualProblemSize();

    PI_ATOMIC_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      *pi = m_pi_init;
      );

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        Real_type x = (Real_type(i) + 0.5) * dx;
        RAJAPERF_ATOMIC_ADD_COUNTING(*pi, dx / (1.0 + x * x));
        );
      }

      RAJAPERF_COUNTERS_PAR_SYNC();

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      m_pi_final = *pi * 4.0;
      );

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace basic
} // end namespace rajaperf
