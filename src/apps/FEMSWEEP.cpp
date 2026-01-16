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
  mesh_file = params.getFemsweepMeshFile();

  // Read problem size from file.
  std::ifstream dataFile(mesh_file);

  if ( !dataFile.is_open() )
  {
    std::cout << "Could not open " << mesh_file << " in constructor." << std::endl;
  }

  std::string line;
  while ( std::getline(dataFile, line) )
  {

    if ( line == std::string("m_nx") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_nx = std::stoi(line);
      }
      else
      {
        std::cout << "Unable to initialize m_nx properly in constructor. Please check the " << mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_ny") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_ny = std::stoi(line);
      }
      else
      {
        std::cout << "Unable to initialize m_ny properly in constructor. Please check the " << mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_nz") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_nz = std::stoi(line);
      }
      else
      {
        std::cout << "Unable to initialize m_nz properly in constructor. Please check the " << mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_na") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_na = std::stoi(line);
      }
      else
      {
        std::cout << "Unable to initialize m_na properly in constructor. Please check the " << mesh_file << " file." << std::endl;
      }
    }

    else if ( line == std::string("m_ng") )
    {
      // Read next line for value.
      if ( std::getline(dataFile, line) )
      {
        m_ng = std::stoi(line);
      }
      else
      {
        std::cout << "Unable to initialize m_ng properly in constructor. Please check the " << mesh_file << " file." << std::endl;
      }
    }

  }

  m_ne = m_nx * m_ny * m_nz;

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

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  // The checksum is inaccurate starting at the 10's digit for: AMD CPU and older clang versions on NVIDIA GPUs.
  setChecksumTolerance(ChecksumTolerance::loose);

  setComplexity(Complexity::N);

  setUsesFeature(Launch);
  //setUsesFeature(View);

  addVariantTunings();
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
  std::ifstream dataFile(mesh_file);

  if ( !dataFile.is_open() )
  {
    std::cout << "Could not open " << mesh_file << " in setUp." << std::endl;
  }

  std::string line;
  while ( std::getline(dataFile, line) )
  {

    if ( line == std::string("m_nhpaa_r") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_na )
        {
          std::cout << "Size of m_nhpaa_r in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_nhpaa_r = allocDataForInit(m_nhpaa_r, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_nhpaa_r[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_nhpaa_r." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_nhpaa_r." << std::endl;
      }
    }

    else if ( line == std::string("m_ohpaa_r") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_na )
        {
          std::cout << "Size of m_ohpaa_r in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_ohpaa_r = allocDataForInit(m_ohpaa_r, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_ohpaa_r[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_ohpaa_r." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_ohpaa_r." << std::endl;
      }
    }

    else if ( line == std::string("m_phpaa_r") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_na * m_hplanes )
        {
          std::cout << "Size of m_phpaa_r in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_phpaa_r = allocDataForInit(m_phpaa_r, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_phpaa_r[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_phpaa_r." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_phpaa_r." << std::endl;
      }
    }

    else if ( line == std::string("m_order_r") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_na * m_ne )
        {
          std::cout << "Size of m_order_r in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_order_r, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_order_r[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_order_r." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_order_r." << std::endl;
      }
    }

    else if ( line == std::string("m_AngleElem2FaceType") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != NLF * m_ne * m_na )
        {
          std::cout << "Size of m_AngleElem2FaceType in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_AngleElem2FaceType, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_AngleElem2FaceType[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_AngleElem2FaceType." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_AngleElem2FaceType." << std::endl;
      }
    }

    else if ( line == std::string("m_elem_to_faces") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != NLF * m_ne )
        {
          std::cout << "Size of m_elem_to_faces in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_elem_to_faces, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_elem_to_faces[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_elem_to_faces." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_elem_to_faces." << std::endl;
      }
    }

    else if ( line == std::string("m_F_g2l") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != (m_sharedinteriorfaces + m_boundaryfaces) )
        {
          std::cout << "Size of m_F_g2l in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_F_g2l, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_F_g2l[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_F_g2l." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_F_g2l." << std::endl;
      }
    }

    else if ( line == std::string("m_idx1") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_sharedinteriorfaces * 4 )
        {
          std::cout << "Size of m_idx1 in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_idx1, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_idx1[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_idx1." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_idx1." << std::endl;
      }
    }

    else if ( line == std::string("m_idx2") )
    {
      // Read next line for array size.
      if ( std::getline(dataFile, line) )
      {
        int sizetemp = std::stoi(line);
        // Check size for sanity.
        if ( sizetemp != m_sharedinteriorfaces * 4 )
        {
          std::cout << "Size of m_idx2 in " << mesh_file << " does not match." << std::endl;
        }
        auto temp_order_r = allocDataForInit(m_idx2, sizetemp, vid); 
        // Read rest of entries for array.
        for ( int lcount = 0; lcount < sizetemp; ++lcount )
        {
          if ( std::getline(dataFile, line) )
          {
            m_idx2[lcount] = std::stoi(line);
          }
          else
          {
            std::cout << "Invalid entry in " << mesh_file << " for m_idx2." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "Invalid size entry in " << mesh_file << " for m_idx2." << std::endl;
      }
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
