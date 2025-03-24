//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-24, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"
#include "FEMSWEEP_DATA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


FEMSWEEP::FEMSWEEP(const RunParams& params)
  : KernelBase(rajaperf::Apps_FEMSWEEP, params)
{
  m_ne = params.getFemSweepNumE();
  m_na = params.getFemSweepNumA();
  m_ng = params.getFemSweepNumG();

  setDefaultProblemSize(ND * m_ne * m_ng * m_na);
  setDefaultReps(1);

  m_Blen = ND * m_ne * m_na;
  m_Alen = ND * ND * m_ne * m_na;
  // 9450 is a property of the mesh. Will need to derive this when mesh generator is available.
  m_Flen = FDS * FDS * 2 * 9450 * m_na;
  m_Sglen = m_ne * m_ng;
  m_M0len = ND * ND * m_ne;
  m_Xlen = ND * m_ne * m_ng * m_na;

  setActualProblemSize( m_Xlen );

  setItsPerRep(1);
  setKernelsPerRep(1);
  // using total data size instead of writes and reads
  setBytesReadPerRep( 1*sizeof(Real_type) * m_Blen +
                      1*sizeof(Real_type) * m_Alen +
                      1*sizeof(Real_type) * m_Flen +
                      1*sizeof(Real_type) * m_Sglen +
                      1*sizeof(Real_type) * m_M0len +
                      1*sizeof(Real_type) * m_Xlen );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_Xlen );
  setBytesAtomicModifyWrittenPerRep( 0 );

  // This is an estimate of the upper bound FLOPs.
  setFLOPsPerRep( (ND * ND * (ND-1) * 3 * 2 + // L & U formation
                  ND * (ND-1) * 3 +             // forward substitution
                  ND * (ND-1) * 3) *            // backward substitution
                  m_ne +                            // matrix solve performed per element
                  m_ne * NLF * FDS);                // coupling between sides of faces

  checksum_scale_factor = 1.0;

  setComplexity(Complexity::N_to_the_four);

  setUsesFeature(Launch);
  //setUsesFeature(View);

  setVariantDefined( Base_Seq );
  setVariantDefined( RAJA_Seq );

  setVariantDefined( Base_OpenMP );
  setVariantDefined( RAJA_OpenMP );

  //setVariantDefined( Base_CUDA );
  setVariantDefined( RAJA_CUDA );

  //setVariantDefined( Base_HIP );
  setVariantDefined( RAJA_HIP );
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
  allocData(m_nhpaa_r, m_na       , vid);
  allocData(m_ohpaa_r, m_na       , vid);
  allocData(m_phpaa_r, m_na * 43  , vid);
  allocData(m_order_r, m_na * m_ne, vid);

  allocData(m_AngleElem2FaceType, NLF * m_ne * m_na , vid);
  allocData(m_elem_to_faces     , NLF * m_ne        , vid);
  allocData(m_F_g2l             , 10800             , vid);
  allocData(m_idx1              , 37800             , vid);
  allocData(m_idx2              , 37800             , vid);

  copyDataH2Space(m_nhpaa_r, g_nhpaa_r, m_na        , vid);
  copyDataH2Space(m_ohpaa_r, g_ohpaa_r, m_na        , vid);
  copyDataH2Space(m_phpaa_r, g_phpaa_r, m_na * 43   , vid);
  copyDataH2Space(m_order_r, g_order_r, m_na * m_ne , vid);

  copyDataH2Space(m_AngleElem2FaceType, g_AngleElem2FaceType, NLF * m_ne * m_na , vid);
  copyDataH2Space(m_elem_to_faces     , g_elem_to_faces     , NLF * m_ne        , vid);
  copyDataH2Space(m_F_g2l             , g_F_g2l             , 10800             , vid);
  copyDataH2Space(m_idx1              , g_idx1              , 37800             , vid);
  copyDataH2Space(m_idx2              , g_idx2              , 37800             , vid);
}

void FEMSWEEP::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_Xdat, m_Xlen, checksum_scale_factor , vid);
}

void FEMSWEEP::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

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
