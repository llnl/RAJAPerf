//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HYDRO_1D.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace lcals
{


HYDRO_1D::HYDRO_1D(const RunParams& params)
  : KernelBase(rajaperf::Lcals_HYDRO_1D, params)
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

void HYDRO_1D::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * getActualProblemSize() + // x, y
                           1*sizeof(Real_type) * (getActualProblemSize()+1) ); // z
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() + // y
                      1*sizeof(Real_type) * (getActualProblemSize()+1) ); // z (each iterate accesses the range [i+10, i+11])
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // x
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(5 * getActualProblemSize());
}

HYDRO_1D::~HYDRO_1D()
{
}

void HYDRO_1D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_y, getActualProblemSize(), vid);
  allocAndInitData(m_z, getActualProblemSize()+1, vid);

  initData(m_q, vid);
  initData(m_r, vid);
  initData(m_t, vid);
}

void HYDRO_1D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_x, getActualProblemSize(), vid);
}

void HYDRO_1D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void HYDRO_1D::setCountedAttributes()
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

    HYDRO_1D_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = ibegin; i < iend; ++i )) {
        RAJAPERF_COUNTERS_LOOP_BODY(HYDRO_1D_BODY);
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
