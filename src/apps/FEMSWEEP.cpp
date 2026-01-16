//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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
  m_mesh_file = params.getFemsweepMeshFile();

  // Read problem size from file.
  std::ifstream dataFile(m_mesh_file);

  if ( !dataFile.is_open() )
  {
    std::cout << "Could not open " << m_mesh_file << " in constructor." << std::endl;
  }

  std::string line;
  while ( std::getline(dataFile, line) )
  {

    if ( line == std::string("m_nx") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_nx = std::stol(line);
      }
      else
      {
        std::cout << "Unable to initialize m_nx properly in constructor. Please check the " << m_mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_ny") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_ny = std::stol(line);
      }
      else
      {
        std::cout << "Unable to initialize m_ny properly in constructor. Please check the " << m_mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_nz") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_nz = std::stol(line);
      }
      else
      {
        std::cout << "Unable to initialize m_nz properly in constructor. Please check the " << m_mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_na") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_na = std::stol(line);
      }
      else
      {
        std::cout << "Unable to initialize m_na properly in constructor. Please check the " << m_mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_ng") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_ng = std::stol(line);
      }
      else
      {
        std::cout << "Unable to initialize m_ng properly in constructor. Please check the " << m_mesh_file << " file." << std::endl;
      }
    }

  }

  m_ne = m_nx * m_ny * m_nz;

  setDefaultProblemSize(ND * m_ne * m_ng * m_na);
  setDefaultReps(1);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  // The checksum is inaccurate starting at the 10's digit for: AMD CPU and older clang versions on NVIDIA GPUs.
  setChecksumTolerance(ChecksumTolerance::loose);

  setComplexity(Complexity::N);

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

template < typename T >
void FEMSWEEP::readIndexArray(VariantID vid, std::ifstream & file, T*& arr, Index_type expectedsize, std::string arrname)
{
  std::string line;

  // Read next line for array size.
  if ( std::getline(file, line) )
  {
    long sizetemp = std::stol(line);
    // Check size for sanity.
    if ( sizetemp != expectedsize )
    {
      std::cout << "Size of " << arrname << " in " << m_mesh_file << " does not match." << std::endl;
    }
    auto temp_arr = allocDataForInit(arr, sizetemp, vid); 
    // Read rest of entries for array.
    for ( int lcount = 0; lcount < sizetemp; ++lcount )
    {
      if ( std::getline(file, line) )
      {
        arr[lcount] = std::stol(line);
      }
      else
      {
        std::cout << "Invalid entry in " << m_mesh_file << " for " << arrname << "." << std::endl;
      }
    }
  }
  else
  {
    std::cout << "Invalid size entry in " << m_mesh_file << " for " << arrname << "." << std::endl;
  }
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
  std::ifstream dataFile(m_mesh_file);

  if ( !dataFile.is_open() )
  {
    std::cout << "Could not open " << m_mesh_file << " in setUp." << std::endl;
  }

  std::string line;
  while ( std::getline(dataFile, line) )
  {

    if ( line == "m_nhpaa_r" )
    {
      readIndexArray(vid, dataFile, m_nhpaa_r, m_na, "m_nhpaa_r");
    }

    else if ( line == std::string("m_ohpaa_r") )
    {
      readIndexArray(vid, dataFile, m_ohpaa_r, m_na, "m_ohpaa_r");
    }

    else if ( line == std::string("m_phpaa_r") )
    {
      readIndexArray(vid, dataFile, m_phpaa_r, m_na * m_hplanes, "m_phpaa_r");
    }

    else if ( line == std::string("m_order_r") )
    {
      readIndexArray(vid, dataFile, m_order_r, m_na * m_ne, "m_order_r");
    }

    else if ( line == std::string("m_AngleElem2FaceType") )
    {
      readIndexArray(vid, dataFile, m_AngleElem2FaceType, NLF * m_na * m_ne, "m_AngleElem2FaceType");
    }

    else if ( line == std::string("m_elem_to_faces") )
    {
      readIndexArray(vid, dataFile, m_elem_to_faces, NLF * m_ne, "m_elem_to_faces");
    }

    else if ( line == std::string("m_F_g2l") )
    {
      readIndexArray(vid, dataFile, m_F_g2l, m_sharedinteriorfaces + m_boundaryfaces, "m_F_g2l");
    }

    else if ( line == std::string("m_idx1") )
    {
      readIndexArray(vid, dataFile, m_idx1, m_sharedinteriorfaces * 4, "m_idx1");
    }

    else if ( line == std::string("m_idx2") )
    {
      readIndexArray(vid, dataFile, m_idx2, m_sharedinteriorfaces * 4, "m_idx2");
    }

  }

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
