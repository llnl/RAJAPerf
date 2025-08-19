//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void intsc_hexhex()
{
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  INTSC_HEXHEX_BODY;
}


template < size_t block_size >
void INTSC_HEXHEX::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend = getDefaultProblemSize() ;

  auto res{getCudaResource()};

  INTSC_HEXHEX_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (intsc_hexhex<block_size>),
                          grid_size, block_size,
                          shmem, res.get_stream() ) ;

    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      auto intsc_hexhex_lambda = [=] __device__ (Index_type i)
                                 { INTSC_HEXHEX_BODY; };

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                                              decltype(intsc_hexhex_lambda)>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          0, iend,
                          intsc_hexhex_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i) {
        INTSC_HEXHEX_BODY;
      });

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXHEX : Unknown Cuda variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXHEX, Cuda)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
