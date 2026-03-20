//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HYDRO_2D.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace lcals
{


HYDRO_2D::HYDRO_2D(const RunParams& params)
  : KernelBase(rajaperf::Lcals_HYDRO_2D, params)
{
  m_jn = 1000;
  m_kn = 1000;

  m_s = 0.0041;
  m_t = 0.0037;

  setDefaultProblemSize(m_kn * m_jn);
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void HYDRO_2D::setSize(Index_type target_size, Index_type target_reps)
{
  m_jn = m_kn = std::sqrt(target_size) + std::sqrt(2)-1;

  setActualProblemSize( m_kn*m_jn );
  setRunReps( target_reps );

  setItsPerRep( 3 * (m_kn-2)*(m_jn-2) );
  setKernelsPerRep(3);

  setBytesAllocatedPerRep( 11*sizeof(Real_type) * m_kn*m_jn ); // zrout, zzout, za, zb, zm, zp, zq, zr, zu, zv, zz
  setBytesReadPerRep( 2*sizeof(Real_type) * ((m_kn-1)*(m_jn-1) - 1) + // zp, zq (4 point stencil)
                      2*sizeof(Real_type) * ((m_kn-1)*(m_jn-1) - 1) + // zr, zm (3 point stencil)

                      2*sizeof(Real_type) * (m_kn-2)*(m_jn-1) +       // za, zb (2 point stencil)
                      2*sizeof(Real_type) * ((m_kn)*(m_jn) - 4) +     // zz, zr (5 point stencil)

                      4*sizeof(Real_type) * (m_kn-2)*(m_jn-2) );      // zr, zu, zz, zv
  setBytesWrittenPerRep( 2*sizeof(Real_type) * (m_kn-2)*(m_jn-2) +  // za, zb

                         0 +

                         2*sizeof(Real_type) * (m_kn-2)*(m_jn-2) ); // zrout, zzout
  setBytesModifyWrittenPerRep( 0 +

                               2*sizeof(Real_type) * (m_kn-2)*(m_jn-2) + // zu, zv

                               0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((14 +
                  26 +
                  4  ) * (m_jn-2)*(m_kn-2));
}

HYDRO_2D::~HYDRO_2D()
{
}

void HYDRO_2D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_zrout, m_kn*m_jn, 0.0, vid);
  allocAndInitDataConst(m_zzout, m_kn*m_jn, 0.0, vid);
  allocAndInitData(m_za, m_kn*m_jn, vid);
  allocAndInitData(m_zb, m_kn*m_jn, vid);
  allocAndInitData(m_zm, m_kn*m_jn, vid);
  allocAndInitData(m_zp, m_kn*m_jn, vid);
  allocAndInitData(m_zq, m_kn*m_jn, vid);
  allocAndInitData(m_zr, m_kn*m_jn, vid);
  allocAndInitData(m_zu, m_kn*m_jn, vid);
  allocAndInitData(m_zv, m_kn*m_jn, vid);
  allocAndInitData(m_zz, m_kn*m_jn, vid);
}

void HYDRO_2D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_zzout, m_kn*m_jn, vid);
  addToChecksum(m_zrout, m_kn*m_jn, vid);
}

void HYDRO_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_zrout, vid);
  deallocData(m_zzout, vid);
  deallocData(m_za, vid);
  deallocData(m_zb, vid);
  deallocData(m_zm, vid);
  deallocData(m_zp, vid);
  deallocData(m_zq, vid);
  deallocData(m_zr, vid);
  deallocData(m_zu, vid);
  deallocData(m_zv, vid);
  deallocData(m_zz, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void HYDRO_2D::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    const Index_type kbeg = 1;
    const Index_type kend = m_kn - 1;
    const Index_type jbeg = 1;
    const Index_type jend = m_jn - 1;

    HYDRO_2D_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type k = kbeg; k < kend; ++k )) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = jbeg; j < jend; ++j )) {
          RAJAPERF_COUNTERS_LOOP_BODY(HYDRO_2D_BODY1);
        }
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type k = kbeg; k < kend; ++k )) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = jbeg; j < jend; ++j )) {
          RAJAPERF_COUNTERS_LOOP_BODY(HYDRO_2D_BODY2);
        }
      }

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type k = kbeg; k < kend; ++k )) {
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = jbeg; j < jend; ++j )) {
          RAJAPERF_COUNTERS_LOOP_BODY(HYDRO_2D_BODY3);
        }
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
