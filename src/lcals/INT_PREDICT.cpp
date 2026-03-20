//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INT_PREDICT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


INT_PREDICT::INT_PREDICT(const RunParams& params)
  : KernelBase(rajaperf::Lcals_INT_PREDICT, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(400);

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

void INT_PREDICT::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 13*sizeof(Real_type) * getActualProblemSize() ); // px
  setBytesReadPerRep( 10*sizeof(Real_type) * getActualProblemSize() ); // px(12), px(11), px(10), px(9), px(8), px(7), px(6), px(4), px(5), px(2)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // px(0)
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(17 * getActualProblemSize());
}

INT_PREDICT::~INT_PREDICT()
{
}

void INT_PREDICT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_px_initval = 1.0;
  allocAndInitDataConst(m_px, 13*getActualProblemSize(), m_px_initval, vid);

  initData(m_dm22, vid);
  initData(m_dm23, vid);
  initData(m_dm24, vid);
  initData(m_dm25, vid);
  initData(m_dm26, vid);
  initData(m_dm27, vid);
  initData(m_dm28, vid);
  initData(m_c0, vid);
}

void INT_PREDICT::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_px, getActualProblemSize(), vid);
}

void INT_PREDICT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_px, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void INT_PREDICT::setCountedAttributes()
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

    INT_PREDICT_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(INT_PREDICT_BODY);
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
