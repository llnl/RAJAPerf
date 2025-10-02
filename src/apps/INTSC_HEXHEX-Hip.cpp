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

template < Size_type block_size >
__launch_bounds__(block_size,3)
__global__ void intsc_hexhex_hip
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type  const nisc_stage,
    Real_ptr vv_int )
{
  __shared__ Real_type vv_reduce[16] ;

  Index_type blksize = block_size ;   // blocksize = 64  must <= nth_per_isc
  Index_type blk     = blockIdx.x ;
  Index_type ith     = blk*blksize + threadIdx.x ;   // which thread with offset
  Index_type thridx  = threadIdx.x ;

  Real_ptr vv_out = (Real_ptr ) vv_int + 8*blk ;

  INTSC_HEXHEX_BODY;
}


template < Size_type block_size >
__global__ void intsc_hexhex_hip_fixup_vv_64to72
    ( Real_ptr const vv_int,   // [8*intsc blks] blocked volumes, moments
      Size_type const n_szpairs,  // number of subzone pairs
      Real_ptr vv_pair )       // [4*n_szpairs] output voluments, moments
{
  Int_type ith = blockIdx.x*block_size + threadIdx.x;
  FIXUP_VV_BODY ;
}


template < Size_type block_size >
void INTSC_HEXHEX::runHipVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0 ;
  const Index_type iend     = m_tri_per_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc = 8 * getActualProblemSize() ;
  const Size_type  nisc_stage   = n_subz_intsc ;

  const Size_type  n_szgrp     = n_subz_intsc / 8 ;
  const Size_type  gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, block_size) ;
  const Index_type iend_fixup  = gsize_fixup * block_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

  auto res{getHipResource()};

  INTSC_HEXHEX_DATA_SETUP;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const Size_type grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr Size_type shmem = 0;

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

      const Size_type grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexhex_lambda = [=] __device__
          ( Index_type i )
         {
           __shared__ Real_type vv_reduce[16] ;

           Index_type blksize   = block_size ;
           Index_type blk       = i / block_size ;
           Index_type ith       = i ;
           Index_type thridx    = i % block_size ;

           Real_ptr vv_out = (Real_ptr ) vv_int + 8*blk ;
           INTSC_HEXHEX_BODY; };

      auto intsc_hexhex_fixup_lambda = [=] __device__
          ( Index_type i )
         {
           Int_type ith = i ;
           FIXUP_VV_BODY ; } ;

      constexpr Size_type shmem = 0;

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
            __shared__ Real_type vv_reduce[16] ;

            Index_type blksize   = block_size ;
            Index_type blk       = i / block_size ;
            Index_type ith       = i ;
            Index_type thridx    = i % block_size ;

            Real_ptr vv_out = (Real_ptr ) vv_int + 8*blk ;
            INTSC_HEXHEX_BODY;
          }
      ) ;

      RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] __device__ (Index_type i)
          {
            Int_type ith = i ;
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
