//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"

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
  m_nx = 15;
  m_ny = 15;
  m_nz = 15;
  m_ne = m_nx * m_ny * m_nz;
  m_na = 72;
  m_ng = 128;

  setDefaultProblemSize(ND * m_ne * m_ng * m_na);
  setDefaultReps(1);

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
  setFLOPsPerRep( (ND * ND * (ND-1) * 3 * 2 +     // L & U formation
                  ND * (ND-1) * 3 +               // forward substitution
                  ND * (ND-1) * 3 +               // backward substitution
                  NLF * FDS - m_nx * m_ny * 6) *  // coupling between sides of faces
                  m_ne * m_na * m_ng );           // for all elements, angles, and groups

  // The checksum is inaccurate starting at the 10's digit for: AMD CPU and older clang versions on NVIDIA GPUs.
  checksum_scale_factor = 0.0000000001;

  setComplexity(Complexity::N);

  setUsesFeature(Launch);
  //setUsesFeature(View);

  setVariantDefined( Base_Seq );
  setVariantDefined( RAJA_Seq );

  setVariantDefined( Base_OpenMP );
  setVariantDefined( RAJA_OpenMP );

  setVariantDefined( Base_CUDA );
  setVariantDefined( RAJA_CUDA );

  setVariantDefined( Base_HIP );
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

  // Read mesh connectivity data from file.
  std::ifstream dataFile("FEMSWEEP_DATA.txt");

  if ( !dataFile.is_open() )
  {
    std::cout << "Could not open FEMSWEEP_DATA.txt" << std::endl;
    return 1;
  }

  int lcount = 1;
  std::string line;
  while ( std::getLine(dataFile, line) )
  {
    if ( lcount >= 11 && lcount <= 82 )
    {
      g_nhpaa_r[lcount-11] = std::stoi(line);
    }

    else if ( lcount >= 86 && lcount <= 157 )
    {
      g_ohpaa_r[lcount-86] = std::stoi(line);
    }

    else if ( lcount >= 161 && lcount <= 3256 )
    {
      g_phpaa_r[lcount-161] = std::stoi(line);
    }

    else if ( lcount >= 3261 && lcount <= 246260 )
    {
      g_order_r[lcount-3261] = std::stoi(line);
    }

    else if ( lcount >= 246265 && lcount <= 1704264 )
    {
      g_AngleElem2FaceType[lcount-246265] = std::stoi(line);
    }

    else if ( lcount >= 1704268 && lcount <= 1724517 )
    {
      g_elem_to_faces[lcount-1704268] = std::stoi(line);
    }

    else if ( lcount >= 1724521 && lcount <= 1735320 )
    {
      g_F_g2l[lcount-1724521] = std::stoi(line);
    }

    else if ( lcount >= 1735324 && lcount <= 1773123 )
    {
      g_idx1[lcount-1735324] = std::stoi(line);
    }

    else if ( lcount >= 1773127 && lcount <= 1810926 )
    {
      g_idx2[lcount-1773127] = std::stoi(line);
    }

    lcount++;
  }

  // Some of the constants are properties of the mesh.
  // Will need to derive these when mesh generator is available.
  allocAndCopyHostData(m_nhpaa_r, g_nhpaa_r, m_na       , vid);
  allocAndCopyHostData(m_ohpaa_r, g_ohpaa_r, m_na       , vid);
  allocAndCopyHostData(m_phpaa_r, g_phpaa_r, m_na * m_hplanes  , vid);
  allocAndCopyHostData(m_order_r, g_order_r, m_na * m_ne, vid);

  allocAndCopyHostData(m_AngleElem2FaceType, g_AngleElem2FaceType, NLF * m_ne * m_na , vid);
  allocAndCopyHostData(m_elem_to_faces     , g_elem_to_faces     , NLF * m_ne        , vid);
  allocAndCopyHostData(m_F_g2l             , g_F_g2l             , m_sharedinteriorfaces + m_boundaryfaces             , vid);
  allocAndCopyHostData(m_idx1              , g_idx1              , m_sharedinteriorfaces * 4             , vid);
  allocAndCopyHostData(m_idx2              , g_idx2              , m_sharedinteriorfaces * 4             , vid);
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
