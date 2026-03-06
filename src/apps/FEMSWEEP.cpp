//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"
#include "FEMSWEEPMeshGen.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

namespace rajaperf
{
namespace apps
{


FEMSWEEP::FEMSWEEP(const RunParams& params)
  : KernelBase(rajaperf::Apps_FEMSWEEP, params)
{
  // Set basic mesh parameters
  m_na = 8 * params.getFemsweepPolar() * params.getFemsweepAzim();
  m_ng = params.getFemsweepGroups();
  m_nx = params.getFemsweepX();
  m_ny = params.getFemsweepY();
  m_nz = params.getFemsweepZ();
  m_ne = m_nx * m_ny * m_nz;

  // Create mesh connectivity arrays
  m_angularquadrature = new AngularQuadratureLite(params.getFemsweepPolar(), params.getFemsweepAzim());
  m_meshgen = new MeshGenerator(*m_angularquadrature, m_nx, m_ny, m_nz, m_ng);
  m_meshgen->Setup();

  setDefaultProblemSize(ND * m_ne * m_ng * m_na);
  setDefaultReps(1);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  // The checksum is inaccurate starting at the 10's digit for: AMD CPU and older clang versions on NVIDIA GPUs.
  setChecksumTolerance(ChecksumTolerance::loose);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(3);

  setUsesFeature(Launch);
  //setUsesFeature(View);

  addVariantTunings();
}

void FEMSWEEP::setSize(Index_type RAJAPERF_UNUSED_ARG(target_size), Index_type target_reps)
{
  m_sharedinteriorfaces = (m_nx - 1) * m_ny * m_nz +
                          m_nx * (m_ny - 1) * m_nz +
                          m_nx * m_ny * (m_nz - 1);
  m_boundaryfaces = 2 * m_nx * m_ny + 2 * m_ny * m_nz + 2 * m_nx * m_nz;
  m_hplanes = m_nx + m_ny + m_nz - 2;

  m_Blen = ND * m_ne * m_na;
  m_Alen = ND * ND * m_ne * m_na;
  m_Flen = FDS * FDS * 2 * m_sharedinteriorfaces * m_na;
  m_Sglen = m_ne * m_ng;
  m_M0len = ND * ND * m_ne;
  m_Xlen = ND * m_ne * m_ng * m_na;

  setActualProblemSize( m_Xlen );
  setRunReps( target_reps );

  setItsPerRep(1);
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * m_Blen + // Bdat
                           1*sizeof(Real_type) * m_Alen + // Adat
                           1*sizeof(Real_type) * m_Flen + // Fdat
                           1*sizeof(Real_type) * m_Sglen + // Sgdat
                           1*sizeof(Real_type) * m_M0len + // M0dat
                           1*sizeof(Real_type) * m_Xlen + // Xdat
                           1*sizeof(Index_type) * m_na + // nhpaa_r,
                           1*sizeof(Index_type) * m_na + // ohpaa_r,
                           1*sizeof(Index_type) * m_na * 43 + // phpaa_r,
                           1*sizeof(Index_type) * m_na * m_ne + // order_r,
                           1*sizeof(Index_type) * NLF * m_ne * m_na + // AngleElem2FaceType
                           1*sizeof(Index_type) * NLF * m_ne + // elem_to_faces
                           1*sizeof(Index_type) * 10800 + // F_g2l
                           1*sizeof(Index_type) * 37800 + // idx1
                           1*sizeof(Index_type) * 37800 );// idx2
  // using total data size instead of writes and reads
  setBytesReadPerRep( 1*sizeof(Real_type) * m_Blen + // Bdat
                      1*sizeof(Real_type) * m_Alen + // Adat
                      1*sizeof(Real_type) * m_Flen + // Fdat
                      1*sizeof(Real_type) * m_Sglen + // Sgdat
                      1*sizeof(Real_type) * m_M0len + // M0dat
                      1*sizeof(Index_type) * m_na + // nhpaa_r,
                      1*sizeof(Index_type) * m_na + // ohpaa_r,
                      1*sizeof(Index_type) * m_na * 43 + // phpaa_r,
                      1*sizeof(Index_type) * m_na * m_ne + // order_r,
                      1*sizeof(Index_type) * NLF * m_ne * m_na + // AngleElem2FaceType
                      1*sizeof(Index_type) * NLF * m_ne + // elem_to_faces
                      1*sizeof(Index_type) * 10800 + // F_g2l
                      1*sizeof(Index_type) * 37800 + // idx1
                      1*sizeof(Index_type) * 37800 );// idx2
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * m_Xlen ); // Xdat
  setBytesAtomicModifyWrittenPerRep( 0 );

  // This is an estimate of the upper bound FLOPs.
  setFLOPsPerRep( (ND * ND * (ND-1) * 3 * 2 +     // L & U formation
                  ND * (ND-1) * 3 +               // forward substitution
                  ND * (ND-1) * 3 +               // backward substitution
                  NLF * FDS - m_nx * m_ny * 6) *  // coupling between sides of faces
                  m_ne * m_na * m_ng );           // for all elements, angles, and groups
}

FEMSWEEP::~FEMSWEEP()
{
}

void FEMSWEEP::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandValue (m_Bdat     , m_Blen      , vid);
  allocAndInitDataRandValue (m_Adat     , m_Alen      , vid);
  allocAndInitDataRandValue (m_Fdat     , m_Flen      , vid);
  allocAndInitDataRandValue (m_Sgdat    , m_Sglen     , vid);
  allocAndInitDataRandValue (m_M0dat    , m_M0len     , vid);
  allocAndInitDataRandValue (m_Xdat     , m_Xlen      , vid);

  allocAndCopyHostData(m_nhpaa_r, m_meshgen->m_nhyperplanes_all_angles.Data(), m_meshgen->m_nhyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_ohpaa_r, m_meshgen->m_ohyperplanes_all_angles.Data(), m_meshgen->m_ohyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_phpaa_r, m_meshgen->m_phyperplanes_all_angles.Data(), m_meshgen->m_phyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_order_r, m_meshgen->md_ordered_elements_all_angles.HostRead(), m_meshgen->md_ordered_elements_all_angles.Size(), vid);

  allocAndCopyHostData(m_AngleElem2FaceType, m_meshgen->md_angle_elem_to_face_types.HostRead(), m_meshgen->md_angle_elem_to_face_types.Size(), vid);

  allocAndCopyHostData(m_elem_to_faces, m_meshgen->md_elem_to_faces.HostRead(), m_meshgen->md_elem_to_faces.Size(), vid);

  allocAndCopyHostData(m_F_g2l, m_meshgen->global_to_local_face.Data(), m_meshgen->global_to_local_face.Size(), vid);

  allocAndCopyHostData(m_idx1, m_meshgen->d_indices1.Data(), m_meshgen->d_indices1.Size(), vid);

  allocAndCopyHostData(m_idx2, m_meshgen->d_indices2.Data(), m_meshgen->d_indices2.Size(), vid);

}

void FEMSWEEP::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Xdat, m_Xlen, vid);
}

void FEMSWEEP::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_Bdat, vid);
  deallocData(m_Adat, vid);
  deallocData(m_Fdat, vid);
  deallocData(m_Sgdat, vid);
  deallocData(m_M0dat, vid);
  deallocData(m_Xdat, vid);

  deallocData(m_nhpaa_r, vid);
  deallocData(m_ohpaa_r, vid);
  deallocData(m_phpaa_r, vid);
  deallocData(m_order_r, vid);

  deallocData(m_AngleElem2FaceType, vid);
  deallocData(m_elem_to_faces     , vid);
  deallocData(m_F_g2l             , vid);
  deallocData(m_idx1              , vid);
  deallocData(m_idx2              , vid);
}

} // end namespace apps
} // end namespace rajaperf
