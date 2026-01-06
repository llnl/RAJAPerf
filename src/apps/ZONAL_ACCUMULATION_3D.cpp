//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  Index_type rzmax = std::cbrt(getTargetProblemSize()) + 1 + std::cbrt(3)-1;
  m_domain = new ADomain(rzmax, /* ndims = */ 3);

  m_nodal_array_length = m_domain->nnalls;
  m_zonal_array_length = m_domain->lpz+1;

  setActualProblemSize( m_domain->n_real_zones );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  // touched data size, not actual number of stores and loads
  setBytesReadPerRep( 1*sizeof(Index_type) * getItsPerRep() + // real_zones
                      1*sizeof(Real_type) * m_domain->n_real_nodes ); // x (3d nodal stencil pattern: 8 touches per iterate)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() ); // vol
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(8 * getItsPerRep());

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

ZONAL_ACCUMULATION_3D::~ZONAL_ACCUMULATION_3D()
{
  delete m_domain;
}

void ZONAL_ACCUMULATION_3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_x, m_nodal_array_length, 1.0, vid);
  allocAndInitDataConst(m_vol, m_zonal_array_length, 0.0, vid);

  auto reset_rz = allocAndInitDataConstForInit(m_real_zones, m_domain->n_real_zones,
                                                  static_cast<Index_type>(-1), vid);

  setRealZones_3d(m_real_zones, *m_domain);
}

void ZONAL_ACCUMULATION_3D::updateChecksum(VariantID vid, size_t tune_idx)
{
  addToChecksum(m_vol, m_zonal_array_length, vid);
}

void ZONAL_ACCUMULATION_3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  deallocData(m_x, vid);
  deallocData(m_vol, vid);
  deallocData(m_real_zones, vid);
}

} // end namespace apps
} // end namespace rajaperf
