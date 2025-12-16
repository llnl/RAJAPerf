//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_GESUMMV.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"


namespace rajaperf
{
namespace polybench
{


POLYBENCH_GESUMMV::POLYBENCH_GESUMMV(const RunParams& params)
  : KernelBase(rajaperf::Polybench_GESUMMV, params)
{
  Index_type N_default = 1000;

  setDefaultProblemSize( N_default * N_default );
  setDefaultReps(120);

  m_N = std::sqrt( getTargetProblemSize() ) + std::sqrt(2)-1;

  m_alpha = 0.62;
  m_beta = 1.002;


  setActualProblemSize( m_N * m_N );

  setItsPerRep( m_N );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type ) * m_N + // x
                      2*sizeof(Real_type ) * m_N * m_N ); // A, B
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * m_N ); // y
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((4 * m_N +
                  3 ) * m_N  );

  setComplexity(Complexity::N);

  setUsesFeature(Kernel);

  addVariantTunings();
}

POLYBENCH_GESUMMV::~POLYBENCH_GESUMMV()
{
}

void POLYBENCH_GESUMMV::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  allocAndInitData(m_x, m_N, vid);
  allocAndInitDataConst(m_y, m_N, 0.0, vid);
  allocAndInitData(m_A, m_N * m_N, vid);
  allocAndInitData(m_B, m_N * m_N, vid);
}

void POLYBENCH_GESUMMV::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_y, m_N, vid);
}

void POLYBENCH_GESUMMV::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
  deallocData(m_y, vid);
  deallocData(m_A, vid);
  deallocData(m_B, vid);
}

} // end namespace polybench
} // end namespace rajaperf
