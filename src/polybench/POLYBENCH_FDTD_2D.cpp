//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_FDTD_2D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>

namespace rajaperf
{
namespace polybench
{


POLYBENCH_FDTD_2D::POLYBENCH_FDTD_2D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_FDTD_2D, params)
  , m_tsteps(40)
{
  Index_type nx_default = 1000;
  Index_type ny_default = 1000;

  setDefaultProblemSize( std::max( (nx_default-1) * ny_default,
                                    nx_default * (ny_default-1) ) );
  setDefaultReps(8 * m_tsteps);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(2);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_FDTD_2D::setSize(Index_type target_size, Index_type target_reps)
{
  m_nx = std::sqrt( target_size ) + 1 + std::sqrt(2)-1;
  m_ny = m_nx;

  setActualProblemSize( std::max( (m_nx-1)*m_ny, m_nx*(m_ny-1) ) );
  setRunReps( target_reps );

  setItsPerRep( m_ny +
                (m_nx-1)*m_ny +
                m_nx*(m_ny-1) +
                (m_nx-1)*(m_ny-1) );
  setKernelsPerRep(4);
  setBytesReadPerRep( 1*sizeof(Real_type ) + // fict

                      1*sizeof(Real_type ) * m_nx * m_ny + // hz

                      1*sizeof(Real_type ) * m_nx * m_ny + // hz

                      1*sizeof(Real_type ) * (m_nx-1) * m_ny + // ex
                      1*sizeof(Real_type ) * m_nx * (m_ny-1) ); // ey
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_ny + // ey

                         0 +

                         0 +

                         0 ); // hz
  setBytesModifyWrittenPerRep( 0 +

                               1*sizeof(Real_type ) * (m_nx-1) * m_ny + // ey

                               1*sizeof(Real_type ) * m_nx * (m_ny-1) + // ex

                               1*sizeof(Real_type ) * (m_nx-1) * (m_ny-1) ); // hz
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 * m_ny +
                  3 * (m_nx-1)*m_ny +
                  3 * m_nx*(m_ny-1) +
                  5 * (m_nx-1)*(m_ny-1) );
}

POLYBENCH_FDTD_2D::~POLYBENCH_FDTD_2D()
{
}

void POLYBENCH_FDTD_2D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_hz, m_nx * m_ny, 0.0, vid);
  allocAndInitData(m_ex, m_nx * m_ny, vid);
  allocAndInitData(m_ey, m_nx * m_ny, vid);
  allocAndInitData(m_fict, m_tsteps, vid);
}

void POLYBENCH_FDTD_2D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_hz, m_nx * m_ny, vid);
}

void POLYBENCH_FDTD_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_fict, vid);
  deallocData(m_ex, vid);
  deallocData(m_ey, vid);
  deallocData(m_hz, vid);
}

} // end namespace polybench
} // end namespace rajaperf
