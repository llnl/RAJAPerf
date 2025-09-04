//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

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
__global__ void intsc_hexrect
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type  const nisc_stage,
    Real_ptr vv_int )
{
}



template < size_t block_size >
void INTSC_HEXRECT::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = m_tri_per_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc = 8 * getActualProblemSize() ;
  const Size_type  nisc_stage   = n_subz_intsc ;

  const Size_type  n_szgrp     = ( n_subz_intsc + 7 ) / 8 ;
  const size_t     gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, block_size) ;
  const Index_type iend_fixup  = gsize_fixup * block_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

  auto res{getCudaResource()};

  INTSC_HEXRECT_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (intsc_hexrect<block_size>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          m_dsubz, m_tsubz,
                          n_subz_intsc, m_vv_int ) ;

      RPlaunchCudaKernel( (intsc_hexrect_fixup_vv_64to72<block_size>),
                          gsize_fixup, block_size,
                          shmem, res.get_stream(),
                          m_vv_int, n_subz_intsc, m_vv_out ) ;


    }
    stopTimer();

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexrect_lambda = [=] __device__
          ( Index_type i )
         {
           __shared__ double vv_reduce[16] ;

           long blksize   = blockDim.x ;
           long blk       = blockIdx.x ;
           long ith       = blk*blksize + threadIdx.x ;
           double *vv_out = (double *) vv_int + 8*blk ;
           INTSC_HEXRECT_BODY; };

      auto intsc_hexrect_fixup_lambda = [=] __device__
          ( Index_type i )
         {
           int ith = blockIdx.x*block_size + threadIdx.x;
           FIXUP_VV_BODY ; } ;

      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexrect_lambda)>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend,
                          intsc_hexrect_lambda );

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexrect_fixup_lambda)>),
                          gsize_fixup, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend_fixup,
                          intsc_hexrect_fixup_lambda );


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
            INTSC_HEXRECT_BODY;
          }
      ) ;

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] __device__ (Index_type i)
          {
            int ith = blockIdx.x*block_size + threadIdx.x;
            FIXUP_VV_BODY ;
          }
      ) ;

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXRECT : Unknown Cuda variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXRECT, Cuda)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
