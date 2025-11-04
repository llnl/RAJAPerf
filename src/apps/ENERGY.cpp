//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "ENERGY.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace apps
{


ENERGY::ENERGY(const RunParams& params)
  : KernelBase(rajaperf::Apps_ENERGY, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(130);

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

void ENERGY::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );


  setActualProblemSize( target_size );

  setItsPerRep( 6 * getActualProblemSize() );
  setKernelsPerRep(6);

  setBytesAllocatedPerRep( 15*sizeof(Real_type) * getActualProblemSize() ); // e_new, e_old, delvc, p_new, p_old, q_new, q_old, work, compHalfStep, pHalfStep, bvc, pbvc, ql_old, qq_old, vnewc
  // some branches are never taken due to the nature of the initialization of delvc
  // the additional ops that would be done if those branches were taken are noted in the comments
  setBytesReadPerRep((5*sizeof(Real_type) + // e_old, delvc, p_old, q_old, work
                      1*sizeof(Real_type) + // delvc (+7 : compHalfStep, pbvc, e_new, bvc, pHalfStep, ql_old, qq_old)
                      5*sizeof(Real_type) + // delvc, p_old, q_old, pHalfStep, q_new
                      1*sizeof(Real_type) + // work
                      6*sizeof(Real_type) + // delvc p_old, q_old, pHalfStep, q_new, p_new (+5 : pbvc, vnewc, bvc, ql_old, qq_old )
                      1*sizeof(Real_type)   // delvc (+7 : pbvc, e_new, vnewc, bvc, p_new, ql_old, qq_old )
                      ) * getActualProblemSize() );
  setBytesWrittenPerRep((1*sizeof(Real_type) + // e_new
                         1*sizeof(Real_type) + // q_new
                         0*sizeof(Real_type) +
                         0*sizeof(Real_type) +
                         0*sizeof(Real_type) +
                         0*sizeof(Real_type)   // (+1 : q_new )
                         ) * getActualProblemSize() );
  setBytesModifyWrittenPerRep( (0*sizeof(Real_type) +
                                0*sizeof(Real_type) +
                                1*sizeof(Real_type) + // e_new
                                1*sizeof(Real_type) + // e_new
                                1*sizeof(Real_type) + // e_new
                                0*sizeof(Real_type)
                                ) * getActualProblemSize() );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((6  +
                  11 + // 1 sqrt
                  8  +
                  2  +
                  19 + // 1 sqrt
                  9    // 1 sqrt
                  ) * getActualProblemSize());
}

ENERGY::~ENERGY()
{
}

void ENERGY::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_e_new, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_e_old, getActualProblemSize(), vid);
  allocAndInitData(m_delvc, getActualProblemSize(), vid);
  allocAndInitData(m_p_new, getActualProblemSize(), vid);
  allocAndInitData(m_p_old, getActualProblemSize(), vid);
  allocAndInitDataConst(m_q_new, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_q_old, getActualProblemSize(), vid);
  allocAndInitData(m_work, getActualProblemSize(), vid);
  allocAndInitData(m_compHalfStep, getActualProblemSize(), vid);
  allocAndInitData(m_pHalfStep, getActualProblemSize(), vid);
  allocAndInitData(m_bvc, getActualProblemSize(), vid);
  allocAndInitData(m_pbvc, getActualProblemSize(), vid);
  allocAndInitData(m_ql_old, getActualProblemSize(), vid);
  allocAndInitData(m_qq_old, getActualProblemSize(), vid);
  allocAndInitData(m_vnewc, getActualProblemSize(), vid);

  initData(m_rho0, vid);
  initData(m_e_cut, vid);
  initData(m_emin, vid);
  initData(m_q_cut, vid);
}

void ENERGY::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_e_new, getActualProblemSize(), vid);
  addToChecksum(m_q_new, getActualProblemSize(), vid);
}

void ENERGY::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_e_new, vid);
  deallocData(m_e_old, vid);
  deallocData(m_delvc, vid);
  deallocData(m_p_new, vid);
  deallocData(m_p_old, vid);
  deallocData(m_q_new, vid);
  deallocData(m_q_old, vid);
  deallocData(m_work, vid);
  deallocData(m_compHalfStep, vid);
  deallocData(m_pHalfStep, vid);
  deallocData(m_bvc, vid);
  deallocData(m_pbvc, vid);
  deallocData(m_ql_old, vid);
  deallocData(m_qq_old, vid);
  deallocData(m_vnewc, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void ENERGY::setCountedAttributes()
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

    ENERGY_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY1);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY2);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY3);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY4);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY5);
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(ENERGY_BODY6);
      }

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace apps
} // end namespace rajaperf
