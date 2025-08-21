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

#define WARPSIZE 32

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void intsc_hexhex
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type  const nisc_stage,
    Real_ptr vv_int )
{
  __shared__ double vv_reduce[16] ;

  long blksize = block_size ;        // blocksize = 64  must <= nth_per_isc
  long blk     = blockIdx.x ;
  long ith     = blk*blksize + threadIdx.x ;   // which thread with offset

  double *vv_out = (double *) vv_int + 8*blk ;

  INTSC_HEXHEX_BODY;
}


template < size_t block_size >
void INTSC_HEXHEX::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = m_tri_per_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc = 8 * getActualProblemSize() ;
  const Size_type  nisc_stage   = n_subz_intsc ;

  auto res{getCudaResource()};

  INTSC_HEXHEX_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (intsc_hexhex<block_size>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          m_dsubz, m_tsubz,
                          n_subz_intsc, m_vv_int ) ;

    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      auto intsc_hexhex_lambda = [=] __device__
          ( Index_type i )
         {
           __shared__ double vv_reduce[16] ;

           long blksize   = blockDim.x ;
           long blk       = blockIdx.x ;
           long ith       = blk*blksize + threadIdx.x ;
           double *vv_out = (double *) vv_int + 8*blk ;
           INTSC_HEXHEX_BODY; };

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                                              decltype(intsc_hexhex_lambda)>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend,
                          intsc_hexhex_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i)
          {
            __shared__ double vv_reduce[16] ;

            long blksize   = blockDim.x ;
            long blk       = blockIdx.x ;
            long ith       = blk*blksize + threadIdx.x ;
            double *vv_out = (double *) vv_int + 8*blk ;
            INTSC_HEXHEX_BODY;
          }
      ) ;

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
