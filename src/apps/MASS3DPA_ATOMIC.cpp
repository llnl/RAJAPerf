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

void buildElemToDofTable(Index_type Nx, Index_type Ny, Index_type Nz,
                         Index_type p,
                         Index_ptr elemToDof);


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

  setChecksumConsistency(ChecksumConsistency::Inconsistent);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(3);

  setUsesFeature(Launch);
  setUsesFeature(Atomic);

  addVariantTunings();
}

void MASS3DPA_ATOMIC::setSize(Index_type target_size, Index_type target_reps)
{
  // approximate how many elements we need
  m_NE = std::max(static_cast<Index_type>(target_size / (m_P*m_P*m_P)),
                  Index_type(1));

  // Construct the mesh
  m_Nx = static_cast<Index_type>(std::cbrt(m_NE));
  m_Ny = m_Nx;
  m_Nz = m_Ny;
  m_NE = m_Nx * m_Ny * m_Nz;

  // compute true number of dofs
  m_Tot_Dofs = (m_Nx * m_P + 1) * (m_Ny * m_P + 1) * (m_Nz * m_P + 1);

  const Index_type ndof_per_elem = (m_P + 1) * (m_P + 1) * (m_P + 1);

  setActualProblemSize(m_Tot_Dofs);
  setRunReps( target_reps );

  setItsPerRep(m_NE * mpa_at::D1D*mpa_at::D1D);
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * mpa_at::Q1D*mpa_at::D1D + // B
                           1*sizeof(Real_type) * mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D*m_NE + // D
                           2*sizeof(Real_type) * m_Tot_Dofs + // X, Y
                           1*sizeof(Index_type) * ndof_per_elem*m_NE ); // ElemToDoF
  setBytesReadPerRep( 1*sizeof(Real_type) * mpa_at::Q1D*mpa_at::D1D + // B
                      1*sizeof(Index_type) * mpa_at::D1D*mpa_at::D1D*mpa_at::D1D*m_NE + // ElemToDoF
                      1*sizeof(Real_type) * m_Tot_Dofs + // X
                      1*sizeof(Real_type) * mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D*m_NE ); // D
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep(1*sizeof(Real_type) * m_Tot_Dofs ); // Y

  setFLOPsPerRep(
      m_NE *
      (2 * mpa_at::D1D*mpa_at::D1D*mpa_at::D1D*mpa_at::Q1D +
       2 * mpa_at::D1D*mpa_at::D1D*mpa_at::Q1D*mpa_at::Q1D +
       2 * mpa_at::D1D*mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D +
       mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D +
       2 * mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D*mpa_at::D1D +
       2 * mpa_at::Q1D*mpa_at::Q1D*mpa_at::D1D*mpa_at::D1D +
       2 * mpa_at::Q1D*mpa_at::D1D*mpa_at::D1D*mpa_at::D1D +
       mpa_at::D1D*mpa_at::D1D*mpa_at::D1D));
}

MASS3DPA_ATOMIC::~MASS3DPA_ATOMIC() {}

void MASS3DPA_ATOMIC::setUp(VariantID vid,
                            size_t RAJAPERF_UNUSED_ARG(tune_idx)) {

  allocAndInitDataConst(m_B, mpa_at::Q1D*mpa_at::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, mpa_at::Q1D*mpa_at::Q1D*mpa_at::Q1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X, m_Tot_Dofs, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, m_Tot_Dofs, Real_type(0.0), vid);

  const int ndof_per_elem = (m_P + 1) * (m_P + 1) * (m_P + 1);

  auto a_elemToDoF = allocDataForInit(m_ElemToDoF, ndof_per_elem*m_NE, vid);

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

/**
 * Build element-to-DOF connectivity for a structured 3D hex mesh
 * with arbitrary polynomial order p and 1 DOF per node.
 *
 * Inputs:
 *   Nx, Ny, Nz    : number of elements in x, y, z directions
 *   p             : polynomial order (>=1)
 *
 * Outputs:
 *   elem_to_dofs  : size = num_elems
 *                   each entry is a vector of size (p+1)^3
 *                   containing the global DOF indices of that element
 *                   must be preallocated
 *
 * Element numbering:
 *   elem_id = ex + Nx * (ey + Ny * ez)
 */
void buildElemToDofTable(Index_type Nx, Index_type Ny, Index_type Nz,
                         Index_type p,
                         Index_ptr elemToDof)
{
  const Index_type num_nodes_x = Nx * p + 1;
  const Index_type num_nodes_y = Ny * p + 1;

  const Index_type ndof_per_elem = (p + 1) * (p + 1) * (p + 1);

  // Loop over elements
  for (Index_type ez = 0; ez < Nz; ++ez) {
    for (Index_type ey = 0; ey < Ny; ++ey) {
      for (Index_type ex = 0; ex < Nx; ++ex) {
        // Global element index (row in elemToDof)
        Index_type e = ex + Nx * (ey + Ny * ez);

        // Pointer to start of this element's DOF list
        Index_ptr row = elemToDof + e * ndof_per_elem;

        Index_type local = 0;

        // Loop over local nodes of the element
        for (Index_type kz = 0; kz <= p; ++kz) {
          Index_type iz = ez * p + kz;
          for (Index_type ky = 0; ky <= p; ++ky) {
            Index_type iy = ey * p + ky;
            for (Index_type kx = 0; kx <= p; ++kx) {
              Index_type ix = ex * p + kx;

              Index_type nodeID = ix + num_nodes_x * (iy + num_nodes_y * iz);

              // Scalar DOF per node, so dofID == nodeID
              Index_type dofID = nodeID;

              row[local++] = dofID;
            }
          }
        }
      }
    }
  }
}

} // end namespace apps
} // end namespace rajaperf
