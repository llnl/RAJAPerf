//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf {
namespace apps {

MASSVEC3DPA::MASSVEC3DPA(const RunParams &params)
    : KernelBase(rajaperf::Apps_MASSVEC3DPA, params) {

  const Index_type NE_initial = 64000;

  setDefaultProblemSize(NE_initial * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D);
  setDefaultReps(50);

  m_NE =
      std::max((getTargetProblemSize() + (MVPA_Q1D * MVPA_Q1D * MVPA_Q1D) / 2) /
                   (MVPA_Q1D * MVPA_Q1D * MVPA_Q1D),
               Index_type(1));

  setActualProblemSize(m_NE * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D);

  setItsPerRep(getActualProblemSize());
  setKernelsPerRep(1);

  setBytesReadPerRep(2 * sizeof(Real_type) * MVPA_Q1D * MVPA_D1D + // B, Bt
                     3 * sizeof(Real_type) * MVPA_D1D * MVPA_D1D * MVPA_D1D *
                         MVPA_DIM * m_NE + // X (3 components)
                     1 * sizeof(Real_type) * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D *
                         m_NE); // D
  setBytesWrittenPerRep(3 * sizeof(Real_type) * MVPA_D1D * MVPA_D1D * MVPA_D1D *
                        MVPA_DIM * m_NE); // Y (3 components)
  setBytesAtomicModifyWrittenPerRep(0);

  //3 for the dimension loop
  setFLOPsPerRep(m_NE * MVPA_DIM *
                 (2 * MVPA_D1D * MVPA_Q1D * MVPA_D1D * MVPA_D1D +
                  2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_D1D +
                  2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D +
                  MVPA_Q1D * MVPA_Q1D * MVPA_Q1D +
                  2 * MVPA_Q1D * MVPA_D1D * MVPA_Q1D * MVPA_Q1D +
                  2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_Q1D +
                  2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_D1D));

  setComplexity(Complexity::N);

  setUsesFeature(Launch);

  setVariantDefined(Base_Seq);
  setVariantDefined(RAJA_Seq);

  setVariantDefined(Base_OpenMP);
  setVariantDefined(RAJA_OpenMP);

  setVariantDefined(Base_CUDA);
  setVariantDefined(RAJA_CUDA);

  setVariantDefined(Base_HIP);
  setVariantDefined(RAJA_HIP);

  setVariantDefined(Base_SYCL);
  setVariantDefined(RAJA_SYCL);
}

MASSVEC3DPA::~MASSVEC3DPA() {}

void MASSVEC3DPA::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx)) {

  allocAndInitDataConst(m_B, MVPA_Q1D * MVPA_D1D, 1.0, vid);
  allocAndInitDataConst(m_Bt, MVPA_Q1D * MVPA_D1D, 1.0, vid);
  allocAndInitDataConst(m_D, MVPA_Q1D * MVPA_Q1D * MVPA_Q1D * m_NE, 1.0, vid);

  allocAndInitDataConst(m_X, MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_DIM * m_NE, 1.0, vid);

  allocAndInitDataConst(m_Y, MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_DIM * m_NE, 0.0, vid);

}

void MASSVEC3DPA::updateChecksum(VariantID vid, size_t tune_idx) {
  checksum[vid][tune_idx] +=
      calcChecksum(m_Y, MVPA_DIM * MVPA_D1D * MVPA_D1D * MVPA_D1D * m_NE, vid);
}

void MASSVEC3DPA::tearDown(VariantID vid,
                           size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  (void)vid;

  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}

} // end namespace apps
} // end namespace rajaperf
