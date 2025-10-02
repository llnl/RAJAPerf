//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


MASSVEC3DPA::MASSVEC3DPA(const RunParams& params)
  : KernelBase(rajaperf::Apps_MASSVEC3DPA, params)
{
  m_NE_default = 8000;

  setDefaultProblemSize(m_NE_default*MVPA_Q1D*MVPA_Q1D*MVPA_Q1D);
  setDefaultReps(50);

  m_NE = std::max((getTargetProblemSize() + (MVPA_Q1D*MVPA_Q1D*MVPA_Q1D)/2) / (MVPA_Q1D*MVPA_Q1D*MVPA_Q1D), Index_type(1));

  setActualProblemSize( m_NE*MVPA_Q1D*MVPA_Q1D*MVPA_Q1D );

  setItsPerRep(getActualProblemSize());
  setKernelsPerRep(1);

  setBytesReadPerRep( 1*sizeof(Real_type) * MVPA_Q1D*MVPA_D1D + // B
                      1*sizeof(Real_type) * MVPA_D1D*MVPA_D1D*MVPA_D1D*MVDIM*m_NE + // X
                      1*sizeof(Real_type) * MVPA_Q1D*MVPA_Q1D*MVPA_Q1D*m_NE ); // D
  setBytesWrittenPerRep( 1*sizeof(Real_type) * MVPA_D1D*MVPA_D1D*MVPA_D1D*MVDIM*m_NE ); // Y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (2 * MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_Q1D +
                         2 * MVPA_D1D * MVPA_D1D * MVPA_Q1D * MVPA_Q1D +
                         2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D + MVPA_Q1D * MVPA_Q1D * MVPA_Q1D +
                         2 * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D * MVPA_D1D +
                         2 * MVPA_Q1D * MVPA_Q1D * MVPA_D1D * MVPA_D1D +
                         2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_D1D + MVPA_D1D * MVPA_D1D * MVPA_D1D));

  setComplexity(Complexity::N);

  setUsesFeature(Launch);

  setVariantDefined( Base_Seq );
  setVariantDefined( RAJA_Seq );

  setVariantDefined( Base_OpenMP );
  setVariantDefined( RAJA_OpenMP );

  setVariantDefined( Base_CUDA );
  setVariantDefined( RAJA_CUDA );

  setVariantDefined( Base_HIP );
  setVariantDefined( RAJA_HIP );

  setVariantDefined( Base_SYCL );
  setVariantDefined( RAJA_SYCL );

}

MASSVEC3DPA::~MASSVEC3DPA()
{
}

void MASSVEC3DPA::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B, int(MVPA_Q1D*MVPA_D1D), Real_type(1.0), vid);
  allocAndInitDataConst(m_Bt,int(MVPA_Q1D*MVPA_D1D), Real_type(1.0), vid);
  allocAndInitDataConst(m_D, int(MVPA_Q1D*MVPA_Q1D*MVPA_Q1D*m_NE), Real_type(1.0), vid);
  allocAndInitDataConst(m_X, int(MVPA_D1D*MVPA_D1D*MVPA_D1D*MVDIM*m_NE), Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, int(MVPA_D1D*MVPA_D1D*MVPA_D1D*MVDIM*m_NE), Real_type(0.0), vid);
}

void MASSVEC3DPA::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_Y, MVPA_D1D*MVPA_D1D*MVPA_D1D*m_NE, vid);
}

void MASSVEC3DPA::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}

} // end namespace apps
} // end namespace rajaperf
