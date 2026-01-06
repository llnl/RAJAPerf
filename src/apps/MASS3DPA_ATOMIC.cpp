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

  m_DOF_default = 1000000;

  m_P = mpa3d_at::D1D - 1; // polynomial order

  m_NE = m_DOF_default/pow(m_P,3); //approximate how many elements we need

  //How does this work??
  //getTargetProblemSize();
  //m_NE = std::max(static_cast<Index_type>( (getTargetProblemSize() +   , Index_type(1));

  //Construct the mesh
  m_Nx = static_cast<int>(std::cbrt(m_NE));
  m_Ny = m_Nx;
  m_Nz = m_Ny;
  m_NE = m_Nx * m_Ny * m_Nz;

  //compute true number of dofs
  m_Tot_Dofs = (m_Nx * m_P + 1) * (m_Ny * m_P + 1) * (m_Nz * m_P + 1);

  setDefaultProblemSize(m_Tot_Dofs);
  setDefaultReps(50);

  setActualProblemSize(m_Tot_Dofs);

  setItsPerRep(m_NE * mpa3d_at::D1D * mpa3d_at::D1D);
  setKernelsPerRep(1);

  setBytesReadPerRep(2 * sizeof(Real_type) * mpa3d_at::Q1D * mpa3d_at::D1D + // B, Bt
                     1 * sizeof(Index_type) * mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D * m_NE + //ElemToDoF
                     1 * sizeof(Real_type) * m_Tot_Dofs + // X
                     1 * sizeof(Real_type) * mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D * m_NE); // D

  setBytesWrittenPerRep(1 * sizeof(Real_type) * mpa3d_at::D1D * mpa3d_at::D1D *
                        mpa3d_at::D1D * m_NE); // Y

  setBytesAtomicModifyWrittenPerRep(m_Tot_Dofs);

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
  const int total_size = ndof_per_elem * m_NE;

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
