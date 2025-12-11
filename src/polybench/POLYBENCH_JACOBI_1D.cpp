//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_JACOBI_1D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_JACOBI_1D::POLYBENCH_JACOBI_1D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_JACOBI_1D, params)
{
  Index_type N_default = 1000002;

  setDefaultProblemSize( N_default-2 );
  setDefaultReps(1600);

  m_N = getTargetProblemSize() + 2;


  setActualProblemSize( m_N-2 );

  setItsPerRep( 2 * getActualProblemSize() );
  setKernelsPerRep(2);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N +
                      1*sizeof(Real_type ) * m_N );
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-2) +
                         1*sizeof(Real_type ) * (m_N-2) );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 3 * (m_N-2) +
                  3 * (m_N-2) );

  checksum_scale_factor = 0.0001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  addVariantTunings();
}

POLYBENCH_JACOBI_1D::~POLYBENCH_JACOBI_1D()
{
}

void POLYBENCH_JACOBI_1D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  allocAndInitData(m_Ainit, m_N, vid);
  allocAndInitData(m_Binit, m_N, vid);
  allocData(m_A, m_N, vid);
  allocData(m_B, m_N, vid);
}

void POLYBENCH_JACOBI_1D::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_A, m_N, checksum_scale_factor , vid);
  checksum[vid][tune_idx] += calcChecksum(m_B, m_N, checksum_scale_factor , vid);
}

void POLYBENCH_JACOBI_1D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_Ainit, vid);
  deallocData(m_Binit, vid);
}

} // end namespace polybench
} // end namespace rajaperf
