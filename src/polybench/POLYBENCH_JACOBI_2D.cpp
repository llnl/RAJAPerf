//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_JACOBI_2D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_JACOBI_2D::POLYBENCH_JACOBI_2D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_JACOBI_2D, params)
{
  Index_type N_default = 1002;

  setDefaultProblemSize( (N_default-2)*(N_default-2) );
  setDefaultReps(2000);

  m_N = std::sqrt( getTargetProblemSize() ) + 2 + std::sqrt(2)-1;


  setActualProblemSize( (m_N-2) * (m_N-2) );

  setItsPerRep( 2 * (m_N-2) * (m_N-2) );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * (m_N * m_N - 4) + // A (5 point stencil)

                      1*sizeof(Real_type ) * (m_N * m_N - 4) ); // B (5 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-2) * (m_N-2) + // B

                         1*sizeof(Real_type ) * (m_N-2) * (m_N-2) ); // A
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 5 * (m_N-2)*(m_N-2) +
                  5 * (m_N-2)*(m_N-2) );

  checksum_scale_factor = 0.0001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setComplexity(Complexity::N);

  setUsesFeature(Kernel);

  addVariantTunings();
}

POLYBENCH_JACOBI_2D::~POLYBENCH_JACOBI_2D()
{
}

void POLYBENCH_JACOBI_2D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  allocAndInitData(m_Ainit, m_N*m_N, vid);
  allocAndInitData(m_Binit, m_N*m_N, vid);
  allocData(m_A, m_N*m_N, vid);
  allocData(m_B, m_N*m_N, vid);
}

void POLYBENCH_JACOBI_2D::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_A, m_N*m_N, checksum_scale_factor , vid);
  checksum[vid][tune_idx] += calcChecksum(m_B, m_N*m_N, checksum_scale_factor , vid);
}

void POLYBENCH_JACOBI_2D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_Ainit, vid);
  deallocData(m_Binit, vid);
}

} // end namespace polybench
} // end namespace rajaperf
