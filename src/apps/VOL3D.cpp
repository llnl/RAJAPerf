//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "VOL3D.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


VOL3D::VOL3D(const RunParams& params)
  : KernelBase(rajaperf::Apps_VOL3D, params)
{
  setDefaultProblemSize(100*100*100);  // See rzmax in ADomain struct
  setDefaultReps(100);

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

void VOL3D::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::cbrt(target_size) + 1 + std::cbrt(3)-1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 3));

  m_array_length = m_domain->nnalls;

  setActualProblemSize( m_domain->n_real_zones );
  setRunReps( target_reps );

  setItsPerRep( m_domain->lpz+1 - m_domain->fpz );
  setKernelsPerRep(1);
  // touched data size, not actual number of stores and loads
  setBytesReadPerRep( 3*sizeof(Real_type) * (getItsPerRep() + 1+m_domain->jp+m_domain->kp) ); // x, y, z (3d nodal stencil pattern: 8 touches per iterate)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() ); // vol
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(72 * (m_domain->lpz+1 - m_domain->fpz));
}

VOL3D::~VOL3D()
{
}

void VOL3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  auto reset_x = allocAndInitDataConstForInit(m_x, m_array_length, 0.0, vid);
  auto reset_y = allocAndInitDataConstForInit(m_y, m_array_length, 0.0, vid);
  auto reset_z = allocAndInitDataConstForInit(m_z, m_array_length, 0.0, vid);

  Real_type dx = 0.3;
  Real_type dy = 0.2;
  Real_type dz = 0.1;
  setMeshPositions_3d(m_x, dx, m_y, dy, m_z, dz, *m_domain);

  allocAndInitDataConst(m_vol, m_array_length, 0.0, vid);

  m_vnormq = 0.083333333333333333; /* vnormq = 1/12 */
}

void VOL3D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_vol, m_array_length, vid);
}

void VOL3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_z, vid);
  deallocData(m_vol, vid);
}

} // end namespace apps
} // end namespace rajaperf
