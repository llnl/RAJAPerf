//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"
#include "FEMSWEEP_DATA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>
#include <cmath>

namespace rajaperf
{
namespace apps
{


FEMSWEEP::FEMSWEEP(const RunParams& params)
  : KernelBase(rajaperf::Apps_FEMSWEEP, params)
{
  m_nx = 15;
  m_ny = 15;
  m_nz = 15;
  m_ne = m_nx * m_ny * m_nz;
  m_na = 72;
  m_ng = 128;

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
  m_Blen = ND * m_ne * m_na;
  m_Alen = ND * ND * m_ne * m_na;
  // 9450 is a property of the mesh. Will need to derive this when mesh generator is available.
  m_Flen = FDS * FDS * 2 * 9450 * m_na;
  m_Sglen = m_ne * m_ng;
  m_M0len = ND * ND * m_ne;
  m_Xlen = ND * m_ne * m_ng * m_na;

  setActualProblemSize( m_Xlen );
  setRunReps( target_reps );

  setItsPerRep(1);
  setKernelsPerRep(1);
  // using total data size instead of writes and reads
  setBytesReadPerRep( 1*sizeof(Real_type) * m_Blen + // Bdat
                      1*sizeof(Real_type) * m_Alen + // Adat
                      1*sizeof(Real_type) * m_Flen + // Fdat
                      1*sizeof(Real_type) * m_Sglen + // Sgdat
                      1*sizeof(Real_type) * m_M0len ); // M0dat
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

  // Some of the constants are properties of the mesh.
  // Will need to derive these when mesh generator is available.
  allocAndCopyHostData(m_nhpaa_r, g_nhpaa_r, m_na       , vid);
  allocAndCopyHostData(m_ohpaa_r, g_ohpaa_r, m_na       , vid);
  allocAndCopyHostData(m_phpaa_r, g_phpaa_r, m_na * 43  , vid);
  allocAndCopyHostData(m_order_r, g_order_r, m_na * m_ne, vid);

  allocAndCopyHostData(m_AngleElem2FaceType, g_AngleElem2FaceType, NLF * m_ne * m_na , vid);
  allocAndCopyHostData(m_elem_to_faces     , g_elem_to_faces     , NLF * m_ne        , vid);
  allocAndCopyHostData(m_F_g2l             , g_F_g2l             , 10800             , vid);
  allocAndCopyHostData(m_idx1              , g_idx1              , 37800             , vid);
  allocAndCopyHostData(m_idx2              , g_idx2              , 37800             , vid);
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
