//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "EDGE3D.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


EDGE3D::EDGE3D(const RunParams& params)
  : KernelBase(rajaperf::Apps_EDGE3D, params)
{
  setDefaultProblemSize(100*100*100);  // See rzmax in ADomain struct
  setDefaultReps(10);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(3);

  setUsesFeature(Forall);

  addVariantTunings();
}

void EDGE3D::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::cbrt(target_size) + 1 + std::cbrt(3)-1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 3));

  m_array_length = m_domain->nnalls;
  size_t number_of_elements = m_domain->lpz+1 - m_domain->fpz;

  setActualProblemSize( m_domain->n_real_zones );
  setRunReps( target_reps );

  setItsPerRep( number_of_elements );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  setBytesReadPerRep( 3*sizeof(Real_type) * (getItsPerRep() + 1+m_domain->jp+m_domain->kp) ); // x, y, z (3d nodal stencil pattern: 8 touches per iterate)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() ); // sum
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr size_t flops_k_loop = 15
                                  + 6*flops_Jxx()
                                  + flops_jacobian_inv()
                                  + flops_transform_basis(EB) // flops for transform_edge_basis()
                                  + flops_transform_basis(EB) + 9 // flops for transform_curl_edge_basis()
                                  + 2*flops_inner_product<12, 12>(true);

  constexpr size_t flops_j_loop = flops_k_loop*NQ_1D + 3*flops_Jxx() + 6;
  constexpr size_t flops_i_loop = flops_j_loop*NQ_1D + 1;

  constexpr size_t flops_per_element = flops_i_loop*NQ_1D + 9*flops_Jxx() + flops_compute_detj();

  setFLOPsPerRep(number_of_elements * flops_per_element);
}

EDGE3D::~EDGE3D()
{
}

void EDGE3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  auto reset_x = allocAndInitDataConstForInit(m_x, m_array_length, Real_type(0.0), vid);
  auto reset_y = allocAndInitDataConstForInit(m_y, m_array_length, Real_type(0.0), vid);
  auto reset_z = allocAndInitDataConstForInit(m_z, m_array_length, Real_type(0.0), vid);

  Real_type dx = 0.3;
  Real_type dy = 0.2;
  Real_type dz = 0.1;
  setMeshPositions_3d(m_x, dx, m_y, dy, m_z, dz, *m_domain);

  allocAndInitDataConst(m_sum, m_array_length, Real_type(0.0), vid);
}

void EDGE3D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_sum, m_array_length, vid);
}

void EDGE3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);

  deallocData(m_sum, vid);
}

} // end namespace apps
} // end namespace rajaperf
