//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-22, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INDEXLIST_3LOOP.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


INDEXLIST_3LOOP::INDEXLIST_3LOOP(const RunParams& params)
  : KernelBase(rajaperf::Basic_INDEXLIST_3LOOP, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent);
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Scan);

  addVariantTunings();
}

void INDEXLIST_3LOOP::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( 3 * getActualProblemSize() + 1 );
  setKernelsPerRep(3);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // x
                           1*sizeof(Int_type) * getActualProblemSize() + // list
                           1*sizeof(Index_type) * (getActualProblemSize()+1) ); // counts
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // x
                      0 +
                      1*sizeof(Index_type) * (getActualProblemSize()+1) ); // counts
  setBytesWrittenPerRep( 1*sizeof(Index_type) * getActualProblemSize() + // counts
                         0 +
                         1*sizeof(Int_type) * (getActualProblemSize()+1) / 2 ); // list (about 50% output)
  setBytesModifyWrittenPerRep( 0 +
                               1*sizeof(Index_type) * (getActualProblemSize()+1) + // counts
                               0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

INDEXLIST_3LOOP::~INDEXLIST_3LOOP()
{
}

void INDEXLIST_3LOOP::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_x, getActualProblemSize(), vid);
  allocAndInitData(m_list, getActualProblemSize(), vid);
  m_len = -1;
}

void INDEXLIST_3LOOP::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_list, getActualProblemSize(), vid);
  addToChecksum(m_len);
}

void INDEXLIST_3LOOP::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_list, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void INDEXLIST_3LOOP::setCountedAttributes()
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

    INDEXLIST_3LOOP_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_CODE_WRAPPER(
    INDEXLIST_3LOOP_COUNTS_SETUP(DataSpace::Host);
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(counts[i] = (INDEXLIST_3LOOP_CONDITIONAL) ? 1 : 0);
      }

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      Index_type count = 0;
      );

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend+1; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        Index_type inc = counts[i];
        counts[i] = count;
        count += inc;
        );
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(INDEXLIST_3LOOP_MAKE_LIST);
      }

      RAJAPERF_COUNTERS_PAR_SYNC();

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      m_len = counts[iend];
      );

    }

    RAJAPERF_COUNTERS_CODE_WRAPPER(
    INDEXLIST_3LOOP_COUNTS_TEARDOWN(DataSpace::Host);
    );

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace basic
} // end namespace rajaperf
