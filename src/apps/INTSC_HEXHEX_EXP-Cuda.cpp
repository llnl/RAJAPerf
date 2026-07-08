//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX_EXP.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

#define RAJAPERF_HEXHEX_WARPSIZE RAJA_CUDA_WARPSIZE
#if RAJAPERF_HEXHEX_WARPSIZE == 32
#define RAJAPERF_HEXHEX_shfl_xor(val,n) __shfl_xor_sync(0xffffffff,val,n)
#else
#error "unexpected RAJA_CUDA_WARPSIZE"
#endif

template < Size_type tile_size, Size_type block_size >
__launch_bounds__(block_size)
__global__ void stage2_subz_aos_to_aosoa_pair_kernel(
    Real_ptr const dAos,
    Real_ptr const tAos,
    Size_type const nPairs,
    Real_ptr const dBlk,
    Real_ptr const tBlk)
{
  __shared__ Real_type tile[24][tile_size + 1];

  Index_type const tileId = blockIdx.x;
  Index_type const pairBase = tileId * tile_size;
  Index_type const tid = threadIdx.x;

  for (Int_type pass = 0; pass < 2; ++pass) {
    Real_ptr const src = pass == 0 ? dAos : tAos;

    for (Index_type e = tid; e < 24 * tile_size; e += block_size) {
      Int_type const p = e / 24;
      Int_type const component = e - 24 * p;
      Index_type const ipair = pairBase + p;

      tile[component][p] = (ipair < nPairs)
                         ? src[24 * ipair + component]
                         : Real_type(0.0);
    }

    __syncthreads();

    Real_ptr const dst = (pass == 0 ? dBlk : tBlk) + tileId * 24 * tile_size;

    for (Index_type e = tid; e < 24 * tile_size; e += block_size) {
      Int_type const component = e / tile_size;
      Int_type const p = e - component * tile_size;

      dst[component * tile_size + p] = tile[component][p];
    }

    __syncthreads();
  }
}

template <Int_type TTET, Size_type block_size>
__launch_bounds__(block_size)
__global__ void stage2_target_tet_kernel_aosoa(
    Real_ptr const dsubz_blk,
    Real_ptr const tsubz_blk,
    Size_type const nPairs,
    Real_ptr const vv_out)
{
  extern __shared__ Real_type shmem[];

  Index_type const lane = threadIdx.x;
  Index_type const tile = blockIdx.x;
  Index_type const ipair = tile * block_size + lane;

  if (ipair >= nPairs) {
    return;
  }

  Real_const_ptr const dTile = dsubz_blk + tile * 24 * block_size;
  Real_const_ptr const tTile = tsubz_blk + tile * 24 * block_size;
  Real_type *cand_vx = shmem;
  Real_type *cand_vy = cand_vx + 12 * block_size;
  Real_type *cand_vz = cand_vy + 12 * block_size;

  HexHexTargetRefMapExp ref_map;
  make_target_tet_ref_map_new_fixed_aosoa<TTET, block_size>(
      tTile, lane, ref_map);

  Moment4 ref_sum;
  ref_sum.v = Real_type(0.0);
  ref_sum.mx = Real_type(0.0);
  ref_sum.my = Real_type(0.0);
  ref_sum.mz = Real_type(0.0);

#pragma unroll 1
  for (Int_type dtet = 0; dtet < 6; ++dtet) {
    Tet4 t;

    transform_donor_tet_fixed_runtime_dtet_aosoa<TTET, block_size>(
        dTile, lane, dtet, ref_map, t);

    Moment4 const ref_mom =
        intersect_tettet_edgeface_shared_exp<block_size>(
            t, cand_vx, cand_vy, cand_vz, lane);

    ref_sum.v += ref_mom.v;
    ref_sum.mx += ref_mom.mx;
    ref_sum.my += ref_mom.my;
    ref_sum.mz += ref_mom.mz;
  }

  Real_type const det = compute_target_det_exp<TTET, block_size>(tTile, lane);

  Real_type const vv_sum = det * ref_sum.v;

  Real_type vx_sum;
  {
    Real_type x0, e1x, e2x, e3x;
    load_target_x_row_exp<TTET, block_size>(
        tTile, lane, x0, e1x, e2x, e3x);
    vx_sum = det * (x0 * ref_sum.v +
                    e1x * ref_sum.mx +
                    e2x * ref_sum.my +
                    e3x * ref_sum.mz);
  }

  Real_type vy_sum;
  {
    Real_type y0, e1y, e2y, e3y;
    load_target_y_row_exp<TTET, block_size>(
        tTile, lane, y0, e1y, e2y, e3y);
    vy_sum = det * (y0 * ref_sum.v +
                    e1y * ref_sum.mx +
                    e2y * ref_sum.my +
                    e3y * ref_sum.mz);
  }

  Real_type vz_sum;
  {
    Real_type z0, e1z, e2z, e3z;
    load_target_z_row_exp<TTET, block_size>(
        tTile, lane, z0, e1z, e2z, e3z);
    vz_sum = det * (z0 * ref_sum.v +
                    e1z * ref_sum.mx +
                    e2z * ref_sum.my +
                    e3z * ref_sum.mz);
  }

  Index_type const std_i = ipair / hexhex_exp_fixup_groupsize;
  Index_type const sub_i = ipair % hexhex_exp_fixup_groupsize;

  Real_ptr out =
      vv_out +
      hexhex_exp_nvals_per_std_intsc * std_i +
      hexhex_exp_nvals_per_pair * sub_i;

  if constexpr (TTET == 0) {
    out[0] = vv_sum;
    out[1] = vx_sum;
    out[2] = vy_sum;
    out[3] = vz_sum;
  } else {
    out[0] += vv_sum;
    out[1] += vx_sum;
    out[2] += vy_sum;
    out[3] += vz_sum;
  }
}

template < Size_type block_size >
__launch_bounds__(block_size)
__global__ void intsc_hexhex_exp
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type  const nisc_stage,
    Real_ptr vv_int )
{
  __shared__ Real_type vv_reduce[hexhex_exp_len_vv_reduce] ;

  Index_type blksize = block_size ;   // blocksize = 64  must <= hexhex_exp_tri_per_pair
  Index_type blk     = blockIdx.x ;
  Index_type ith     = blk*blksize + threadIdx.x ;  // which thread with offset
  Index_type thridx  = threadIdx.x ;

  Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;

  INTSC_HEXHEX_EXP_BODY;
}


template < Size_type block_size >
__global__ void intsc_hexhex_exp_fixup_vv_64to72
    ( Real_ptr const vv_int,   // [8*intsc blks] blocked volumes, moments
      Size_type const n_szpairs,  // number of subzone pairs
      Real_ptr vv_pair )       // [4*n_szpairs] output voluments, moments
{
  Int_type i = blockIdx.x*block_size + threadIdx.x;
  INTSC_HEXHEX_EXP_FIXUP_VV_BODY ;
}



template < Size_type block_size >
void INTSC_HEXHEX_EXP::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = hexhex_exp_tri_per_std_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc= m_n_subz_intsc ;
  const Size_type  nisc_stage  = m_n_subz_intsc ;

  // n_szgrp is number of groups of subzone pairs in fixup kernel.
  // gsize_fixup = fixup kernel grid size (1 thread per group of subzone pairs)
  // iend_fixup = number of threads for fixup kernel.
  //      Kernel has bounds check to mask out excess threads.

  const Size_type  n_szgrp     =
      RAJA_DIVIDE_CEILING_INT(n_subz_intsc, hexhex_exp_fixup_groupsize)  ;
  const Size_type  gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, block_size) ;
  const Index_type iend_fixup  = gsize_fixup * block_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

  auto res{getCudaResource()};

  INTSC_HEXHEX_EXP_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    constexpr Size_type stage2_tile_size = 128;
    constexpr Size_type stage2_transpose_block_size = 256;
    constexpr Size_type stage2_target_tet_kernels = 6;
    constexpr Size_type stage2_transpose_kernels = 1;
    setKernelsPerRep(stage2_transpose_kernels + stage2_target_tet_kernels);
    const Size_type stage2_grid_size =
        RAJA_DIVIDE_CEILING_INT(n_subz_intsc, stage2_tile_size);
    const Size_type stage2_aosoa_len =
        24 * stage2_grid_size * stage2_tile_size;

    Real_ptr dsubz_blk;
    Real_ptr tsubz_blk;
    allocData(DataSpace::CudaDevice, dsubz_blk, stage2_aosoa_len);
    allocData(DataSpace::CudaDevice, tsubz_blk, stage2_aosoa_len);

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      constexpr Size_type shmem = 0;
      constexpr Size_type target_tet_shmem =
          3 * 12 * stage2_tile_size * sizeof(Real_type);

      RPlaunchCudaKernel( (stage2_subz_aos_to_aosoa_pair_kernel<
                              stage2_tile_size, stage2_transpose_block_size>),
                          stage2_grid_size, stage2_transpose_block_size,
                          shmem, res.get_stream(),
                          m_dsubz, m_tsubz, n_subz_intsc,
                          dsubz_blk, tsubz_blk ) ;

#define LAUNCH_TARGET_TET(TTET)                                             \
      RPlaunchCudaKernel(                                                   \
          (stage2_target_tet_kernel_aosoa<TTET, stage2_tile_size>),          \
          stage2_grid_size, stage2_tile_size,                                \
          target_tet_shmem, res.get_stream(),                                \
          dsubz_blk, tsubz_blk, n_subz_intsc, m_vv_out )

      LAUNCH_TARGET_TET(0);
      LAUNCH_TARGET_TET(1);
      LAUNCH_TARGET_TET(2);
      LAUNCH_TARGET_TET(3);
      LAUNCH_TARGET_TET(4);
      LAUNCH_TARGET_TET(5);

#undef LAUNCH_TARGET_TET

    }
    stopTimer();

    deallocData(DataSpace::CudaDevice, dsubz_blk);
    deallocData(DataSpace::CudaDevice, tsubz_blk);

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      const Size_type grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexhex_exp_lambda = [=] __device__ ( Index_type i ) {

          __shared__ Real_type vv_reduce[hexhex_exp_len_vv_reduce] ;

          Index_type blksize   = block_size ;
          Index_type blk       = i / block_size ;
          Index_type ith       = i ;
          Index_type thridx    = i % block_size ;

          Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;
          INTSC_HEXHEX_EXP_BODY ;
      } ;

      auto intsc_hexhex_exp_fixup_lambda = [=] __device__ ( Index_type i ) {
          INTSC_HEXHEX_EXP_FIXUP_VV_BODY ;
      } ;

      constexpr Size_type shmem = 0;

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexhex_exp_lambda)>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend,
                          intsc_hexhex_exp_lambda );

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexhex_exp_fixup_lambda)>),
                          gsize_fixup, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend_fixup,
                          intsc_hexhex_exp_fixup_lambda );


    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i)
          {
            RAJA_TEAM_SHARED Real_type vv_reduce[hexhex_exp_len_vv_reduce] ;

            Index_type blksize   = block_size ;
            Index_type blk       = i / block_size ;
            Index_type ith       = i ;
            Index_type thridx    = i % block_size ;

            Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;
            INTSC_HEXHEX_EXP_BODY;
          }
      ) ;

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] __device__ (Index_type i)
          {
            INTSC_HEXHEX_EXP_FIXUP_VV_BODY ;
          }
      ) ;

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXHEX_EXP : Unknown Cuda variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXHEX_EXP, Cuda, Base_CUDA, Lambda_CUDA, RAJA_CUDA)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
