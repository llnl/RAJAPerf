//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "REDUCE3_INT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <limits>

namespace rajaperf
{
namespace basic
{


REDUCE3_INT::REDUCE3_INT(const RunParams& params)
  : KernelBase(rajaperf::Basic_REDUCE3_INT, params)
{
  setDefaultProblemSize(1000000);
//setDefaultReps(5000);
// Set reps to low value until we resolve RAJA omp-target
// reduction performance issues
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Inconsistent);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Reduction);

  addVariantTunings();
}

void REDUCE3_INT::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 1*sizeof(Int_type) * getActualProblemSize() ); // vec
  setBytesReadPerRep( 1*sizeof(Int_type) * getActualProblemSize() ); // vec
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

REDUCE3_INT::~REDUCE3_INT()
{
}

void REDUCE3_INT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_vec, getActualProblemSize(), vid);

  m_vsum = 0;
  m_vsum_init = 0;
  m_vmin = std::numeric_limits<Int_type>::max();
  m_vmin_init = std::numeric_limits<Int_type>::max();
  m_vmax = std::numeric_limits<Int_type>::min();
  m_vmax_init = std::numeric_limits<Int_type>::min();
}

void REDUCE3_INT::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_vsum);
  addToChecksum(m_vmin);
  addToChecksum(m_vmax);
}

void REDUCE3_INT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_vec, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void REDUCE3_INT::setCountedAttributes()
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

    REDUCE3_INT_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      Int_type vsum = m_vsum_init;
      Int_type vmin = m_vmin_init;
      Int_type vmax = m_vmax_init;
      );

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(REDUCE3_INT_BODY);
      }

      RAJAPERF_COUNTERS_PAR_SYNC();

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      m_vsum = vsum;
      m_vmin = vmin;
      m_vmax = vmax;
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
