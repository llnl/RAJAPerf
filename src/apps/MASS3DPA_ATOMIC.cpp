//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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
    : KernelBase(rajaperf::Apps_MASS3DPA_ATOMIC, params)
{
  Index_type DOF_default = 1000000;
  setDefaultProblemSize(DOF_default);
  setDefaultReps(50);

  // polynomial order
  m_P = mpa_at::D1D - 1;

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setUsesFeature(Launch);

  addVariantTunings();
}

void MASS3DPA_ATOMIC::setSize(Index_type target_size, Index_type target_reps)
{
  // approximate how many elements we need
  m_NE = std::max(static_cast<Index_type>(target_size / pow(m_P, 3)),
                  Index_type(1));

  // Construct the mesh
  m_Nx = static_cast<int>(std::cbrt(m_NE));
  m_Ny = m_Nx;
  m_Nz = m_Ny;
  m_NE = m_Nx * m_Ny * m_Nz;

  // compute true number of dofs
  m_Tot_Dofs = (m_Nx * m_P + 1) * (m_Ny * m_P + 1) * (m_Nz * m_P + 1);

  setActualProblemSize(m_Tot_Dofs);
  setRunReps( target_reps );

  setItsPerRep(m_NE * mpa_at::D1D * mpa_at::D1D);
  setKernelsPerRep(1);

  setBytesReadPerRep(2 * sizeof(Real_type) * mpa_at::Q1D *
                         mpa_at::D1D + // B, Bt
                     1 * sizeof(Index_type) * mpa_at::D1D * mpa_at::D1D *
                         mpa_at::D1D * m_NE +           // ElemToDoF
                     1 * sizeof(Real_type) * m_Tot_Dofs + // X
                     1 * sizeof(Real_type) * mpa_at::Q1D * mpa_at::Q1D *
                         mpa_at::Q1D * m_NE); // D

  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * mpa_at::D1D*mpa_at::D1D*mpa_at::D1D*m_NE ); // Y

  setBytesAtomicModifyWrittenPerRep(1*sizeof(Real_type) * mpa_at::D1D*mpa_at::D1D*mpa_at::D1D*m_NE ); // Y

  setFLOPsPerRep(
      m_NE *
      (2 * mpa_at::D1D * mpa_at::D1D * mpa_at::D1D * mpa_at::Q1D +
       2 * mpa_at::D1D * mpa_at::D1D * mpa_at::Q1D * mpa_at::Q1D +
       2 * mpa_at::D1D * mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D +
       mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D +
       2 * mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D * mpa_at::D1D +
       2 * mpa_at::Q1D * mpa_at::Q1D * mpa_at::D1D * mpa_at::D1D +
       2 * mpa_at::Q1D * mpa_at::D1D * mpa_at::D1D * mpa_at::D1D +
       mpa_at::D1D * mpa_at::D1D * mpa_at::D1D));
}

MASS3DPA_ATOMIC::~MASS3DPA_ATOMIC() {}

void MASS3DPA_ATOMIC::setUp(VariantID vid,
                            size_t RAJAPERF_UNUSED_ARG(tune_idx)) {

  allocAndInitDataConst(m_B, Index_type(mpa_at::Q1D * mpa_at::D1D),
                        Real_type(1.0), vid);
  allocAndInitDataConst(
      m_D, Index_type(mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D * m_NE),
      Real_type(1.0), vid);
  allocAndInitDataConst(m_X, Index_type(m_Tot_Dofs), Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, Index_type(m_Tot_Dofs), Real_type(0.0), vid);

  // Compute table elem to dof table size
  const int ndof_per_elem = (m_P + 1) * (m_P + 1) * (m_P + 1);
  const int total_size = ndof_per_elem * m_NE;

  auto a_elemToDoF = allocDataForInit(m_ElemToDoF, total_size, vid);
  buildElemToDofTable(m_Nx, m_Ny, m_Nz, m_P, m_ElemToDoF);
}

void MASS3DPA_ATOMIC::updateChecksum(VariantID vid,
                                     size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
   addToChecksum(m_Y, m_Tot_Dofs, vid);
}

void MASS3DPA_ATOMIC::tearDown(VariantID vid,
                               size_t RAJAPERF_UNUSED_ARG(tune_idx)) {
  (void)vid;

  deallocData(m_B, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
  deallocData(m_ElemToDoF, vid);
}

} // end namespace apps
} // end namespace rajaperf
