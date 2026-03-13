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
  setDefaultProblemSize(params.getFemsweepX() * params.getFemsweepY() * params.getFemsweepZ() * // mesh elements
                        params.getFemsweepGroups() *  // energy groups
                        8 * params.getFemsweepPolar() * params.getFemsweepAzim()  // angles
                       );
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

void FEMSWEEP::setSize(Index_type target_size, Index_type target_reps)
{
  // Set basic mesh parameters

  // Always use user or default angles and groups.
  m_na = 8 * this->run_params.getFemsweepPolar() * this->run_params.getFemsweepAzim();
  m_ng = this->run_params.getFemsweepGroups();

  if (!this->run_params.isFemsweepMeshDims())
  {
    // Adapt mesh size to runtime memory requirements.
    Index_type remainder = target_size / (m_na * m_ng);

    // Cube root of a number less than 4 will always round down, so set these to 1 regardless.
    if ( remainder < 4 )
    {
      m_nx = 1;
      m_ny = 1;
      m_nz = 1;
    }
    else
    {
      Index_type rounded_cube = std::round( std::cbrt(remainder) );
      m_nx = rounded_cube;
      m_ny = rounded_cube;
      m_nz = rounded_cube;
    }

  }
  else  // Using user or default parameters to set mesh size.
  {
    m_nx = this->run_params.getFemsweepX();
    m_ny = this->run_params.getFemsweepY();
    m_nz = this->run_params.getFemsweepZ();
  }
  
  m_ne = m_nx * m_ny * m_nz;

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
                           1*sizeof(Index_type) * m_na * m_hplanes + // phpaa_r,
                           1*sizeof(Index_type) * m_na * m_ne + // order_r,
                           1*sizeof(Index_type) * NLF * m_ne * m_na + // AngleElem2FaceType
                           1*sizeof(Index_type) * NLF * m_ne + // elem_to_faces
                           1*sizeof(Index_type) * (m_sharedinteriorfaces + m_boundaryfaces) + // F_g2l
                           1*sizeof(Index_type) * m_sharedinteriorfaces * 4 + // idx1
                           1*sizeof(Index_type) * m_sharedinteriorfaces * 4 );// idx2
  // using total data size instead of writes and reads
  setBytesReadPerRep( m_ng*2*sizeof(Real_type) * m_Blen + // Bdat
                      2*sizeof(Real_type) * m_Alen + // Adat
                      m_ng*NLF*sizeof(Real_type) * m_Flen + // Fdat
                      m_na*sizeof(Real_type) * m_Sglen + // Sgdat
                      m_ng*m_na*sizeof(Real_type) * m_M0len + // M0dat
                      1*sizeof(Real_type) * m_sharedinteriorfaces * m_na * m_ng * ND * NLF * FDS * FDS + // Xdat
                      m_ng*sizeof(Index_type) * m_na + // nhpaa_r,
                      m_ng*sizeof(Index_type) * m_na + // ohpaa_r,
                      m_ng*sizeof(Index_type) * m_na * m_hplanes + // phpaa_r,
                      m_ng*sizeof(Index_type) * m_na * m_ne + // order_r,
                      m_ng*sizeof(Index_type) * NLF * m_ne * m_na + // AngleElem2FaceType
                      m_ng*m_na*sizeof(Index_type) * NLF * m_ne + // elem_to_faces
                      m_ng*m_na*sizeof(Index_type) * (m_sharedinteriorfaces + m_boundaryfaces) + // F_g2l
                      m_ng*m_na*sizeof(Index_type) * m_sharedinteriorfaces * 4 + // idx1
                      m_ng*m_na*sizeof(Index_type) * m_sharedinteriorfaces * 4 );// idx2
  setBytesWrittenPerRep( ND*sizeof(Real_type) * m_Xlen );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  // This is an estimate of the upper bound FLOPs.
  setFLOPsPerRep( m_ne * m_na * m_ng *                        // for all elements, angles, and groups
                  (ND * ND * ND + 7 * ND * ND / 2 - ND / 2 - 1) + // LU (naive)
                  m_sharedinteriorfaces * m_na * m_ng *       // for all shared faces, angles, and groups
                  (FDS +                                      // B
                  FDS * FDS * 3));                            // coupling between sides of faces
}

FEMSWEEP::~FEMSWEEP()
{
}

void FEMSWEEP::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  // Create mesh connectivity arrays
  m_angularquadrature = new AngularQuadratureLite(this->run_params.getFemsweepPolar(), this->run_params.getFemsweepAzim());
  m_meshgen = new MeshGenerator(*m_angularquadrature, m_nx, m_ny, m_nz, m_ng);
  m_meshgen->Setup();

  allocAndInitDataRandValue (m_Bdat     , m_Blen      , vid);
  allocAndInitDataRandValue (m_Adat     , m_Alen      , vid);
  allocAndInitDataRandValue (m_Fdat     , m_Flen      , vid);
  allocAndInitDataRandValue (m_Sgdat    , m_Sglen     , vid);
  allocAndInitDataRandValue (m_M0dat    , m_M0len     , vid);
  allocAndInitDataRandValue (m_Xdat     , m_Xlen      , vid);

  allocAndCopyHostData(m_nhpaa_r, m_meshgen->m_nhyperplanes_all_angles.Data(), m_meshgen->m_nhyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_ohpaa_r, m_meshgen->m_ohyperplanes_all_angles.Data(), m_meshgen->m_ohyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_phpaa_r, m_meshgen->m_phyperplanes_all_angles.Data(), m_meshgen->m_phyperplanes_all_angles.Size(), vid);

  allocAndCopyHostData(m_order_r, m_meshgen->md_ordered_elements_all_angles.Data(), m_meshgen->md_ordered_elements_all_angles.Size(), vid);

  allocAndCopyHostData(m_AngleElem2FaceType, m_meshgen->md_angle_elem_to_face_types.Data(), m_meshgen->md_angle_elem_to_face_types.Size(), vid);

  allocAndCopyHostData(m_elem_to_faces, m_meshgen->md_elem_to_faces.Data(), m_meshgen->md_elem_to_faces.Size(), vid);

  allocAndCopyHostData(m_F_g2l, m_meshgen->global_to_local_face.Data(), m_meshgen->global_to_local_face.Size(), vid);

  allocAndCopyHostData(m_idx1, m_meshgen->d_indices1.Data(), m_meshgen->d_indices1.Size(), vid);

  allocAndCopyHostData(m_idx2, m_meshgen->d_indices2.Data(), m_meshgen->d_indices2.Size(), vid);

  delete m_angularquadrature;
  m_angularquadrature = nullptr;
  delete m_meshgen;
  m_meshgen = nullptr;

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
