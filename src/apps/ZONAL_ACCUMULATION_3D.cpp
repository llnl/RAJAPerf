//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "ZONAL_ACCUMULATION_3D.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


ZONAL_ACCUMULATION_3D::ZONAL_ACCUMULATION_3D(const RunParams& params)
  : KernelBase(rajaperf::Apps_ZONAL_ACCUMULATION_3D, params)
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

void ZONAL_ACCUMULATION_3D::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::cbrt(target_size) + 1 + std::cbrt(3)-1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 3));

  m_nodal_array_length = m_domain->nnalls;
  m_zonal_array_length = m_domain->lpz+1;

  setActualProblemSize( m_domain->n_real_zones );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  // touched data size, not actual number of stores and loads
  setBytesReadPerRep( 1*sizeof(Index_type) * getItsPerRep() + // real_zones
                      1*sizeof(Real_type) * m_domain->n_real_nodes ); // x (3d nodal stencil pattern: 8 touches per iterate)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() ); // vol
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(8 * getItsPerRep());
}

ZONAL_ACCUMULATION_3D::~ZONAL_ACCUMULATION_3D()
{
}

void ZONAL_ACCUMULATION_3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_nodal_array_length, 1.0, vid);
  allocAndInitDataConst(m_vol, m_zonal_array_length, 0.0, vid);

  auto reset_rz = allocAndInitDataConstForInit(m_real_zones, m_domain->n_real_zones,
                                                  static_cast<Index_type>(-1), vid);

  setRealZones_3d(m_real_zones, *m_domain);
}

void ZONAL_ACCUMULATION_3D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_vol, m_zonal_array_length, vid);
}

void ZONAL_ACCUMULATION_3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_vol, vid);
  deallocData(m_real_zones, vid);
}

} // end namespace apps
} // end namespace rajaperf
