//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
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

  m_nx = std::sqrt( getTargetProblemSize() ) + 1 + std::sqrt(2)-1;
  m_ny = m_nx;


  setActualProblemSize( std::max( (m_nx-1)*m_ny, m_nx*(m_ny-1) ) );

  setItsPerRep( m_ny +
                (m_nx-1)*m_ny +
                m_nx*(m_ny-1) +
                (m_nx-1)*(m_ny-1) );
  setKernelsPerRep(4);
  setBytesReadPerRep( 1*sizeof(Real_type ) +

                      1*sizeof(Real_type ) * (m_nx-1) * m_ny +
                      1*sizeof(Real_type ) * m_nx * m_ny +

                      1*sizeof(Real_type ) * m_nx * (m_ny-1) +
                      1*sizeof(Real_type ) * m_nx * m_ny +

                      1*sizeof(Real_type ) * (m_nx-1) * (m_ny-1) +
                      1*sizeof(Real_type ) * (m_nx-1) * m_ny +
                      1*sizeof(Real_type ) * m_nx * (m_ny-1) );
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_ny +

                         1*sizeof(Real_type ) * (m_nx-1) * m_ny +

                         1*sizeof(Real_type ) * m_nx * (m_ny-1) +

                         1*sizeof(Real_type ) * (m_nx-1) * (m_ny-1) );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 * m_ny +
                  3 * (m_nx-1)*m_ny +
                  3 * m_nx*(m_ny-1) +
                  5 * (m_nx-1)*(m_ny-1) );

  checksum_scale_factor = 0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setComplexity(Complexity::N);

  setUsesFeature(Kernel);

  addVariantTunings();
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

void POLYBENCH_FDTD_2D::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_hz, m_nx * m_ny, checksum_scale_factor, vid);
}

void POLYBENCH_FDTD_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_fict, vid);
  deallocData(m_ex, vid);
  deallocData(m_ey, vid);
  deallocData(m_hz, vid);
}

} // end namespace polybench
} // end namespace rajaperf
