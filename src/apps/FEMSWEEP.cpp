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
  m_nd = params.getFemSweepNumD();
  m_nfds = params.getFemSweepNumFDS();
  m_na = params.getFemSweepNumA();
  m_ng = params.getFemSweepNumG();

  setDefaultProblemSize(m_nd * m_ne * m_ng * m_na);
  setDefaultReps(1);

  m_Blen = m_nd * m_ne * m_na;
  m_Alen = m_nd * m_nd * m_ne * m_na;
  m_Flen = m_nfds * m_nfds * 2 * 9450 * m_na;
  m_Sglen = m_ne * m_ng;
  m_M0len = m_nd * m_nd * m_ne;
  m_Xlen = m_nd * m_ne * m_ng * m_na;
  m_Xfinallen = m_nd * m_ne * m_ng * m_na;

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
  setFLOPsPerRep(2 * m_ne * m_ng * m_na * m_nd);

  checksum_scale_factor = 1.0;  //0.001 *
              //( static_cast<Checksum_type>(getDefaultProblemSize()) /
              //                             getActualProblemSize() );

  setComplexity(Complexity::N);

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
  allocAndInitDataConst (m_Fdat     , m_Flen      , Real_type(0.0), vid);
  //allocAndInitDataRandValue (m_Fdat     , m_Flen      , vid);
  allocAndInitDataRandValue (m_Sgdat    , m_Sglen     , vid);
  allocAndInitDataRandValue (m_M0dat    , m_M0len     , vid);
  allocAndInitDataRandValue (m_Xdat     , m_Xlen      , vid);

  allocData(m_nhpaa_r, 72     , vid);
  allocData(m_ohpaa_r, 72     , vid);
  allocData(m_phpaa_r, 3096   , vid);
  allocData(m_order_r, 243000 , vid);

  allocData(m_AngleElem2FaceType, 1458000 , vid);
  allocData(m_elem_to_faces     , 20250   , vid);
  allocData(m_F_g2l             , 10800   , vid);
  allocData(m_idx1              , 37800   , vid);
  allocData(m_idx2              , 37800   , vid);

  copyDataH2Space(m_nhpaa_r, g_nhpaa_r, 72    , vid);
  copyDataH2Space(m_ohpaa_r, g_ohpaa_r, 72    , vid);
  copyDataH2Space(m_phpaa_r, g_phpaa_r, 3096  , vid);
  copyDataH2Space(m_order_r, g_order_r, 243000, vid);

  copyDataH2Space(m_AngleElem2FaceType, g_AngleElem2FaceType, 1458000 , vid);
  copyDataH2Space(m_elem_to_faces     , g_elem_to_faces     , 20250   , vid);
  copyDataH2Space(m_F_g2l             , g_F_g2l             , 10800   , vid);
  copyDataH2Space(m_idx1              , g_idx1              , 37800   , vid);
  copyDataH2Space(m_idx2              , g_idx2              , 37800   , vid);
}

void FEMSWEEP::updateChecksum(VariantID vid, size_t tune_idx)
{
  //copyDataSameSpace(m_Xfinaldat, m_Xdat, m_Xfinallen, vid);
  checksum[vid][tune_idx] += calcChecksum(m_Xdat, m_Xlen, checksum_scale_factor , vid);
  //checksum[vid][tune_idx] += calcChecksum(m_Xfinaldat, m_Xfinallen, checksum_scale_factor , vid);
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
  //deallocData(m_Xfinaldat, vid);

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
