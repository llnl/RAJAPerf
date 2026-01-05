//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DPA_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf {
namespace apps {

MASS3DPA_ATOMIC::MASS3DPA_ATOMIC(const RunParams &params)
    : KernelBase(rajaperf::Apps_MASS3DPA_ATOMIC, params) {
  m_NE_default = 8000;

  m_Nx = 2;
  m_Ny = 1;
  m_Nz = 1;
  m_P = mpa3d_at::D1D - 1; // polynomial order

  m_NE = m_Nx * m_Ny * m_Nz;


  m_Tot_Dofs = (m_Nx * m_P + 1) * (m_Ny * m_P + 1) * (m_Nz * m_P + 1);

  setDefaultProblemSize(m_Tot_Dofs);
  setDefaultReps(50);

  m_NE = m_Nx * m_Ny * m_Nz; // std::max((getTargetProblemSize() +
                             // (m_Tot_Dofs)/2) / (m_Tot_Dofs), Index_type(1));

  setActualProblemSize(m_Tot_Dofs);

  setItsPerRep(m_NE * mpa3d_at::Q1D * mpa3d_at::Q1D);
  setKernelsPerRep(1);

  // Need to also account for the indirection array
  setBytesReadPerRep(2 * sizeof(Real_type) * mpa3d_at::Q1D *
                         mpa3d_at::D1D + // B, Bt
                     2 * sizeof(Real_type) * mpa3d_at::D1D * mpa3d_at::D1D *
                         mpa3d_at::D1D * m_NE + // X, Y
                     1 * sizeof(Real_type) * mpa3d_at::Q1D * mpa3d_at::Q1D *
                         mpa3d_at::Q1D * m_NE); // D
  setBytesWrittenPerRep(1 * sizeof(Real_type) * mpa3d_at::D1D * mpa3d_at::D1D *
                        mpa3d_at::D1D * m_NE); // Y
  setBytesAtomicModifyWrittenPerRep(0);

  setFLOPsPerRep(
      m_NE *
      (2 * mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::Q1D +
       2 * mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::Q1D * mpa3d_at::Q1D +
       2 * mpa3d_at::D1D * mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D +
       mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D +
       2 * mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::D1D +
       2 * mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::D1D * mpa3d_at::D1D +
       2 * mpa3d_at::Q1D * mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D +
       mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);

  setComplexity(Complexity::N);

  setUsesFeature(Launch);

  addVariantTunings();
}

MASS3DPA_ATOMIC::~MASS3DPA_ATOMIC() { }

void MASS3DPA_ATOMIC::setUp(VariantID vid,
                            size_t RAJAPERF_UNUSED_ARG(tune_idx)) {

  allocAndInitDataConst(m_B, Index_type(mpa3d_at::Q1D * mpa3d_at::D1D),
                        Real_type(1.0), vid);
  allocAndInitDataConst(m_Bt, Index_type(mpa3d_at::Q1D * mpa3d_at::D1D),
                        Real_type(1.0), vid);
  allocAndInitDataConst(
      m_D, Index_type(mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D * m_NE),
      Real_type(1.0), vid);
  allocAndInitDataConst(m_X, Index_type(m_Tot_Dofs), Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, Index_type(m_Tot_Dofs), Real_type(0.0), vid);

  // Compute table elem to dof table size
  const int ndof_per_elem = (m_P + 1) * (m_P + 1) * (m_P + 1);
  const int total_size = ndof_per_elem * ndof_per_elem;

  auto a_elemToDoF = allocDataForInit(m_ElemToDoF, total_size, vid);
  buildElemToDofTable(m_Nx, m_Ny, m_Nz, m_P, m_ElemToDoF);  
  
}

void MASS3DPA_ATOMIC::updateChecksum(VariantID vid, size_t tune_idx) {
  checksum[vid][tune_idx] += calcChecksum(m_Y, m_Tot_Dofs, vid);
}

void MASS3DPA_ATOMIC::tearDown(VariantID vid,
                               size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  (void)vid;

  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
  deallocData(m_ElemToDoF, vid);
}

} // end namespace apps
} // end namespace rajaperf
