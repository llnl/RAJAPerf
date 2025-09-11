//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

#define WARPSIZE 64
#define __shfl_xor_sync(mask,val,n) __shfl_xor(val,n)

namespace rajaperf
{
namespace apps
{

template < size_t block_size >
__launch_bounds__(block_size,3)
__global__ void intsc_hexhex_hip
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
__global__ void intsc_hexhex_hip_fixup_vv_64to72
    ( Real_ptr const vv_int,   // [8*intsc blks] blocked volumes, moments
      Size_type const n_szpairs,  // number of subzone pairs
      Real_ptr vv_pair )       // [4*n_szpairs] output voluments, moments
{
  int ith = blockIdx.x*block_size + threadIdx.x;
  FIXUP_VV_BODY ;
}


template < size_t block_size >
void INTSC_HEXHEX::runHipVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend     = m_tri_per_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc = 8 * getActualProblemSize() ;
  const Size_type  nisc_stage   = n_subz_intsc ;

  const Size_type  n_szgrp     = ( n_subz_intsc + 7 ) / 8 ;
  const size_t     gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, block_size) ;
  const Index_type iend_fixup  = gsize_fixup * block_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

  auto res{getHipResource()};

  INTSC_HEXHEX_DATA_SETUP;

  //  Insert a warmup call to the kernel in order to remove the
  //  time of initialization that affects the first call to the kernel.
  //   The warmup calls send n_subz_intsc=0 and hence return immediately.
  bool const do_warmup = true ;
  if ( do_warmup ) {
    const size_t grid_size1 = 1 ;
    constexpr size_t shmem = 0;

    RPlaunchHipKernel( (intsc_hexhex_hip<block_size>),
                       grid_size1, block_size,
                       shmem, res.get_stream(),
                       m_dsubz, m_tsubz,
                       0UL, m_vv_int ) ;
    RPlaunchHipKernel( (intsc_hexhex_hip_fixup_vv_64to72<block_size>),
                       grid_size1, block_size,
                       shmem, res.get_stream(),
                       m_vv_int, 0UL, m_vv_out ) ;
  }


  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (intsc_hexhex_hip<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         m_dsubz, m_tsubz,
                         n_subz_intsc, m_vv_int ) ;

      RPlaunchHipKernel( (intsc_hexhex_hip_fixup_vv_64to72<block_size>),
                         gsize_fixup, block_size,
                         shmem, res.get_stream(),
                         m_vv_int, n_subz_intsc, m_vv_out ) ;


    }
    stopTimer();

  } else if ( vid == Lambda_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexhex_lambda = [=] __device__
          ( Index_type i )
         {
           __shared__ double vv_reduce[16] ;

           long blksize   = blockDim.x ;
           long blk       = blockIdx.x ;
           long ith       = blk*blksize + threadIdx.x ;
           double *vv_out = (double *) vv_int + 8*blk ;
           INTSC_HEXHEX_BODY; };

      auto intsc_hexhex_fixup_lambda = [=] __device__
          ( Index_type i )
         {
           int ith = blockIdx.x*block_size + threadIdx.x;
           FIXUP_VV_BODY ; } ;

      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (lambda_hip_forall<block_size,
                          decltype(intsc_hexhex_lambda)>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         ibegin, iend,
                         intsc_hexhex_lambda );

      RPlaunchHipKernel( (lambda_hip_forall<block_size,
                          decltype(intsc_hexhex_fixup_lambda)>),
                         gsize_fixup, block_size,
                         shmem, res.get_stream(),
                         ibegin, iend_fixup,
                         intsc_hexhex_fixup_lambda );


    }
    stopTimer();

  } else if ( vid == RAJA_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
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

      RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] __device__ (Index_type i)
          {
            int ith = blockIdx.x*block_size + threadIdx.x;
            FIXUP_VV_BODY ;
          }
      ) ;

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXHEX : Unknown Hip variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXHEX, Hip)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP
