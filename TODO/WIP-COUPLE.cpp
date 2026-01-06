//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "WIP-COUPLE.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{


COUPLE::COUPLE(const RunParams& params)
  : KernelBase(rajaperf::Apps_COUPLE, params)
{
  setDefaultProblemSize(100*100*100);  // See rzmax in ADomain struct
  setDefaultReps(50);

  Index_type rzmax = std::cbrt(getTargetProblemSize())+1;
  m_domain = new ADomain(rzmax, /* ndims = */ 3);

  m_imin = m_domain->imin;
  m_imax = m_domain->imax;
  m_jmin = m_domain->jmin;
  m_jmax = m_domain->jmax;
  m_kmin = m_domain->kmin;
  m_kmax = m_domain->kmax;

  setActualProblemSize( m_domain->n_real_zones );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesPerRep( (3*sizeof(Complex_type) + 5*sizeof(Complex_type)) * m_domain->n_real_zones );
  setFLOPsPerRep(0);

  setUsesFeature(Forall);

  addVariantTunings();
}

COUPLE::~COUPLE()
{
  delete m_domain;
}

void COUPLE::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  Index_type max_loop_index = m_domain->lrn;

  allocAndInitData(m_t0, max_loop_index, vid);
  allocAndInitData(m_t1, max_loop_index, vid);
  allocAndInitData(m_t2, max_loop_index, vid);
  allocAndInitData(m_denac, max_loop_index, vid);
  allocAndInitData(m_denlw, max_loop_index, vid);

  m_clight = 3.e+10;
  m_csound = 3.09e+7;
  m_omega0 = 0.9;
  m_omegar = 0.9;
  m_dt = 0.208;
  m_c10 = 0.25 * (m_clight / m_csound);
  m_fratio = sqrt(m_omegar / m_omega0);
  m_r_fratio = 1.0/m_fratio;
  m_c20 = 0.25 * (m_clight / m_csound) * m_r_fratio;
  m_ireal = Complex_type(0.0, 1.0);
}

void COUPLE::runSeqVariant(VariantID vid)
{
  const Index_type run_reps = getRunReps();

  COUPLE_DATA_SETUP;

  switch ( vid ) {

    case Base_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        for (Index_type k = kmin ; k < kmax ; ++k ) {
          COUPLE_BODY;
        }

      }
      stopTimer();

      break;
    }

#if defined(RUN_RAJA_SEQ)
    case RAJA_Seq : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::forall<RAJA::seq_exec>(
          RAJA::RangeSegment(kmin, kmax), [=](Index_type k) {
          COUPLE_BODY;
        });

      }
      stopTimer();

      break;
    }
#endif

    default : {
      getCout() << "\n  COUPLE : Unknown variant id = " << vid << std::endl;
    }

  }
}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(COUPLE, Seq, Base_Seq, RAJA_Seq)

void COUPLE::runOpenMPVariant(VariantID vid)
{
  const Index_type run_reps = getRunReps();

  COUPLE_DATA_SETUP;

  switch ( vid ) {

#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
    case Base_OpenMP : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        #pragma omp parallel for
        for (Index_type k = kmin ; k < kmax ; ++k ) {
          COUPLE_BODY;
        }

      }
      stopTimer();
      break;
    }

    case RAJA_OpenMP : {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::forall<RAJA::omp_parallel_for_exec>(
          RAJA::RangeSegment(kmin, kmax), [=](Index_type k) {
          COUPLE_BODY;
        });

      }
      stopTimer();

      break;
    }
#endif

    default : {
      getCout() << "\n  COUPLE : Unknown variant id = " << vid << std::endl;
    }

  }
}

RAJAPERF_DEFAULT_TUNING_DEFINE_BOILERPLATE(COUPLE, OpenMP, Base_OpenMP, RAJA_OpenMP)


void COUPLE::updateChecksum(VariantID vid, size_t tune_idx)
{
  Index_type max_loop_index = m_domain->lrn;

  addToChecksum(m_t0, max_loop_index, vid);
  addToChecksum(m_t1, max_loop_index, vid);
  addToChecksum(m_t2, max_loop_index, vid);
}

void COUPLE::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  deallocData(m_t0, vid);
  deallocData(m_t1, vid);
  deallocData(m_t2, vid);
  deallocData(m_denac, vid);
  deallocData(m_denlw, vid);
}

} // end namespace apps
} // end namespace rajaperf
