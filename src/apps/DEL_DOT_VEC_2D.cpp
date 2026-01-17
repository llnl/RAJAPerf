//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DEL_DOT_VEC_2D.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


DEL_DOT_VEC_2D::DEL_DOT_VEC_2D(const RunParams& params)
  : KernelBase(rajaperf::Apps_DEL_DOT_VEC_2D, params)
{
  setDefaultProblemSize(1000*1000);  // See rzmax in ADomain struct
  setDefaultReps(100);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(2);

  setUsesFeature(Forall);

  addVariantTunings();
}

void DEL_DOT_VEC_2D::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::sqrt(target_size) + 1 + std::sqrt(2)-1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 2));

  m_array_length = m_domain->nnalls;

  setActualProblemSize(m_domain->n_real_zones);
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Index_type) * getItsPerRep() + // real_zones
                      4*sizeof(Real_type) * m_domain->n_real_nodes ); // x, y, fx, fy (2d nodal stencil pattern: 4 touches per iterate)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() ); // div
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(54 * m_domain->n_real_zones);


}

DEL_DOT_VEC_2D::~DEL_DOT_VEC_2D()
{
}

void DEL_DOT_VEC_2D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  auto reset_x = allocAndInitDataConstForInit(m_x, m_array_length, 0.0, vid);
  auto reset_y = allocAndInitDataConstForInit(m_y, m_array_length, 0.0, vid);
  auto reset_rz = allocAndInitDataConstForInit(m_real_zones, m_domain->n_real_zones,
                      static_cast<Index_type>(-1), vid);

  Real_type dx = 0.2;
  Real_type dy = 0.1;
  setMeshPositions_2d(m_x, dx, m_y, dy, *m_domain);
  setRealZones_2d(m_real_zones, *m_domain);

  allocAndInitData(m_xdot, m_array_length, vid);
  allocAndInitData(m_ydot, m_array_length, vid);

  allocAndInitDataConst(m_div, m_array_length, 0.0, vid);

  m_ptiny = 1.0e-20;
  m_half = 0.5;
}

void DEL_DOT_VEC_2D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_div, m_array_length, vid);
}

void DEL_DOT_VEC_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_real_zones, vid);
  deallocData(m_xdot, vid);
  deallocData(m_ydot, vid);
  deallocData(m_div, vid);
}

} // end namespace apps
} // end namespace rajaperf
