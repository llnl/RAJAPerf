//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

void COUPLE::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::cbrt(target_size)+1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 3));

  m_imin = m_domain->imin;
  m_imax = m_domain->imax;
  m_jmin = m_domain->jmin;
  m_jmax = m_domain->jmax;
  m_kmin = m_domain->kmin;
  m_kmax = m_domain->kmax;

  setActualProblemSize( m_domain->n_real_zones );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesPerRep( (3*sizeof(Complex_type) + 5*sizeof(Complex_type)) * m_domain->n_real_zones );
  setFLOPsPerRep(0);
}

COUPLE::~COUPLE()
{
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


void COUPLE::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  Index_type max_loop_index = m_domain->lrn;

  addToChecksum(m_t0, max_loop_index, vid);
  addToChecksum(m_t1, max_loop_index, vid);
  addToChecksum(m_t2, max_loop_index, vid);
}

void COUPLE::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_t0, vid);
  deallocData(m_t1, vid);
  deallocData(m_t2, vid);
  deallocData(m_denac, vid);
  deallocData(m_denlw, vid);
}

} // end namespace apps
} // end namespace rajaperf
