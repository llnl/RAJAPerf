//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MATVEC_3D_STENCIL.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


MATVEC_3D_STENCIL::MATVEC_3D_STENCIL(const RunParams& params)
  : KernelBase(rajaperf::Apps_MATVEC_3D_STENCIL, params)
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

void MATVEC_3D_STENCIL::setSize(Index_type target_size, Index_type target_reps)
{
  Index_type rzmax = std::cbrt(target_size) + 1 + std::cbrt(3)-1;
  m_domain.reset(new ADomain(rzmax, /* ndims = */ 3));

  m_zonal_array_length = m_domain->lpz+1;

  setActualProblemSize( m_domain->n_real_zones );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  const size_t ilen = m_domain->imax - m_domain->imin;
  const size_t jlen = m_domain->jmax - m_domain->jmin;
  const size_t klen = m_domain->kmax - m_domain->kmin;
  auto get_size_extra = [&](size_t iextra, size_t jextra, size_t kextra) {
    return (ilen + iextra) * (jlen + jextra) * (klen + kextra);
  };
  auto get_size_matrix = [&](size_t ishift, size_t jshift, size_t kshift) {
    // get the used size of matrix coefficient allocations
    return get_size_extra(0,0,0) +                   // real zones
          (get_size_extra(0,0,0) - (ilen - ishift) * // plus some extra from the
                                   (jlen - jshift) * // edges based on the shift
                                   (klen - kshift));
  };

  const size_t b_accessed = get_size_extra(0, 0, 0);
  const size_t x_accessed = get_size_extra(2, 2, 2) ;
  const size_t m_accessed = get_size_matrix(0, 0, 0) +
                            get_size_matrix(1, 0, 0) +
                            get_size_matrix(1, 1, 0) +
                            get_size_matrix(0, 1, 0) +
                            get_size_matrix(1, 1, 0) +
                            get_size_matrix(1, 1, 1) +
                            get_size_matrix(0, 1, 1) +
                            get_size_matrix(1, 1, 1) +
                            get_size_matrix(1, 0, 1) +
                            get_size_matrix(0, 0, 1) +
                            get_size_matrix(1, 0, 1) +
                            get_size_matrix(1, 1, 1) +
                            get_size_matrix(0, 1, 1) +
                            get_size_matrix(1, 1, 1) ;
  setBytesReadPerRep( 1*sizeof(Index_type) * getItsPerRep() + // real_zones
                      1*sizeof(Real_type) * x_accessed + // x
                      1*sizeof(Real_type) * m_accessed ); // m
  setBytesWrittenPerRep( 1*sizeof(Real_type) * b_accessed ); // b
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  const size_t multiplies = 27;
  const size_t adds = 26;
  setFLOPsPerRep((multiplies + adds) * getItsPerRep());
}

MATVEC_3D_STENCIL::~MATVEC_3D_STENCIL()
{
}

void MATVEC_3D_STENCIL::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_b, m_zonal_array_length, 0.0, vid);
  allocAndInitData(m_x, m_zonal_array_length, vid);

  allocAndInitData(m_matrix.dbl, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dbc, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dbr, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dcl, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dcc, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dcr, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dfl, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dfc, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.dfr, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.cbl, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.cbc, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.cbr, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.ccl, m_zonal_array_length, vid);
  allocAndInitData(m_matrix.ccc, m_zonal_array_length, vid);
  m_matrix.ccr = m_matrix.ccl + 1                               ;
  m_matrix.cfl = m_matrix.cbr - 1 + m_domain->jp                ;
  m_matrix.cfc = m_matrix.cbc     + m_domain->jp                ;
  m_matrix.cfr = m_matrix.cbl + 1 + m_domain->jp                ;
  m_matrix.ubl = m_matrix.dfr - 1 - m_domain->jp + m_domain->kp ;
  m_matrix.ubc = m_matrix.dfc     - m_domain->jp + m_domain->kp ;
  m_matrix.ubr = m_matrix.dfl + 1 - m_domain->jp + m_domain->kp ;
  m_matrix.ucl = m_matrix.dcr - 1                + m_domain->kp ;
  m_matrix.ucc = m_matrix.dcc                    + m_domain->kp ;
  m_matrix.ucr = m_matrix.dcl + 1                + m_domain->kp ;
  m_matrix.ufl = m_matrix.dbr - 1 + m_domain->jp + m_domain->kp ;
  m_matrix.ufc = m_matrix.dbc     + m_domain->jp + m_domain->kp ;
  m_matrix.ufr = m_matrix.dbl + 1 + m_domain->jp + m_domain->kp ;

  auto reset_rz = allocAndInitDataConstForInit(m_real_zones, m_domain->n_real_zones,
                      static_cast<Index_type>(-1), vid);

  setRealZones_3d(m_real_zones, *m_domain);
}

void MATVEC_3D_STENCIL::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_b, m_zonal_array_length, vid);
}

void MATVEC_3D_STENCIL::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_b, vid);
  deallocData(m_x, vid);

  deallocData(m_matrix.dbl, vid);
  deallocData(m_matrix.dbc, vid);
  deallocData(m_matrix.dbr, vid);
  deallocData(m_matrix.dcl, vid);
  deallocData(m_matrix.dcc, vid);
  deallocData(m_matrix.dcr, vid);
  deallocData(m_matrix.dfl, vid);
  deallocData(m_matrix.dfc, vid);
  deallocData(m_matrix.dfr, vid);
  deallocData(m_matrix.cbl, vid);
  deallocData(m_matrix.cbc, vid);
  deallocData(m_matrix.cbr, vid);
  deallocData(m_matrix.ccl, vid);
  deallocData(m_matrix.ccc, vid);

  deallocData(m_real_zones, vid);
}

} // end namespace apps
} // end namespace rajaperf
