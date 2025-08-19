//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


INTSC_HEXHEX::INTSC_HEXHEX(const RunParams& params)
  : KernelBase(rajaperf::Apps_INTSC_HEXHEX, params)
{
  constexpr size_t number_of_intsc = 100*100*100 ;
  setDefaultProblemSize(number_of_intsc);
  setDefaultReps(10);

  setActualProblemSize( getDefaultProblemSize() );

  setItsPerRep( number_of_intsc );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  setBytesReadPerRep( 48*sizeof(Real_type) * getItsPerRep() );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr size_t flops_per_tri = 700 ;
  constexpr size_t tri_per_intsc = 576 ;
  constexpr size_t flops_per_intsc = flops_per_tri * tri_per_intsc ;

  setFLOPsPerRep(number_of_intsc * flops_per_intsc);

  checksum_scale_factor = 0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  setVariantDefined( Base_Seq );
  setVariantDefined( Lambda_Seq );
  setVariantDefined( RAJA_Seq );

  setVariantDefined( Base_OpenMP );
  setVariantDefined( Lambda_OpenMP );
  setVariantDefined( RAJA_OpenMP );

  setVariantDefined( Base_OpenMPTarget );
  setVariantDefined( RAJA_OpenMPTarget );

  setVariantDefined( Base_CUDA );
  setVariantDefined( Lambda_CUDA );
  setVariantDefined( RAJA_CUDA );

  setVariantDefined( Base_HIP );
  setVariantDefined( Lambda_HIP );
  setVariantDefined( RAJA_HIP );

  setVariantDefined( Base_SYCL );
  setVariantDefined( RAJA_SYCL );
}

INTSC_HEXHEX::~INTSC_HEXHEX()
{
}

void INTSC_HEXHEX::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  //  auto reset_x = allocAndInitDataConstForInit(m_x, m_array_length, Real_type(0.0), vid);
}

void INTSC_HEXHEX::updateChecksum(VariantID vid, size_t tune_idx)
{
  // checksum[vid][tune_idx] += calcChecksum(m_sum, m_array_length, m_checksum_scale_factor, vid  );
}

void INTSC_HEXHEX::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  // deallocData(m_x, vid);
}

} // end namespace apps
} // end namespace rajaperf
