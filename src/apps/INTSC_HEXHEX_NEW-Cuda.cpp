//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX_NEW.hpp"

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

__device__ RAJA_INLINE void hexhex_warp_reduce4(
    Real_type &vv,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz)
{
  for (Int_type k = 1; k < RAJAPERF_HEXHEX_WARPSIZE; k *= 2) {
    vv += RAJAPERF_HEXHEX_shfl_xor(vv, k);
    vx += RAJAPERF_HEXHEX_shfl_xor(vx, k);
    vy += RAJAPERF_HEXHEX_shfl_xor(vy, k);
    vz += RAJAPERF_HEXHEX_shfl_xor(vz, k);
  }
}

template < Size_type block_size >
__launch_bounds__(block_size)
__global__ void intsc_hexhex_new_pair_stage2_tettet
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type const nisc_stage,
    Real_ptr vv_pair )
{
  static_assert(block_size == 128,
                "Stage 2 kernel expects four warps per block");

  constexpr Int_type warp_size = RAJAPERF_HEXHEX_WARPSIZE;
  constexpr Int_type pairs_per_block = block_size / warp_size;

  Int_type const tid = threadIdx.x;
  Int_type const warp = tid / warp_size;
  Int_type const lane = tid & (warp_size - 1);

  Index_type const ipair = blockIdx.x * pairs_per_block + warp;
  bool const valid = ipair < nisc_stage;

  __shared__ Real_type sd[pairs_per_block][24];
  __shared__ Real_type st[pairs_per_block][24];
  __shared__ HexHexTargetTetMapNew smap[pairs_per_block][6];

  if (valid && lane < 24) {
    sd[warp][lane] = dsubz[24 * ipair + lane];
    st[warp][lane] = tsubz[24 * ipair + lane];
  }

  __syncthreads();

  if (valid && lane < 6) {
    make_target_tet_map_new(st[warp], lane, smap[warp][lane]);
  }

  __syncthreads();

  Real_type vv = 0.0;
  Real_type vx = 0.0;
  Real_type vy = 0.0;
  Real_type vz = 0.0;

  if (valid) {
    if (lane < 32) {
      stage2_compute_tettet_task_fast(
          lane, sd[warp], smap[warp], vv, vx, vy, vz);
    }

    if (lane < 4) {
      stage2_compute_tettet_task_fast(
          lane + 32, sd[warp], smap[warp], vv, vx, vy, vz);
    }
  }

  hexhex_warp_reduce4(vv, vx, vy, vz);

  if (valid && lane == 0) {
    Index_type const std_i = ipair / hexhex_new_fixup_groupsize;
    Index_type const sub_i = ipair % hexhex_new_fixup_groupsize;

    Real_ptr out =
        vv_pair +
        hexhex_new_nvals_per_std_intsc * std_i +
        hexhex_new_nvals_per_pair * sub_i;

    out[0] = vv;
    out[1] = vx;
    out[2] = vy;
    out[3] = vz;
  }
}

template <Int_type TTET, Int_type DTET, Size_type block_size>
__launch_bounds__(block_size)
__global__ void stage2_tettet_task_kernel(
    Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type const nPairs,
    Real_ptr const partial)
{
  Index_type const ipair = blockIdx.x * blockDim.x + threadIdx.x;

  if (ipair >= nPairs) {
    return;
  }

  Real_const_ptr const ds = dsubz + 24 * ipair;
  Real_const_ptr const ts = tsubz + 24 * ipair;

  HexHexTargetTetMapNew map;
  make_target_tet_map_new_fixed<TTET>(ts, map);

  Real_type x[4], y[4], z[4];
  transform_donor_tet_fixed<TTET, DTET>(ds, map, x, y, z);

  Real_type vref = Real_type(0.0);
  Real_type mxref = Real_type(0.0);
  Real_type myref = Real_type(0.0);
  Real_type mzref = Real_type(0.0);

  intersect_tettet_edgeface_new(x, y, z, vref, mxref, myref, mzref);

  Real_type const vv = map.det * vref;
  Real_type const vx =
      map.det * (map.x0  * vref +
                 map.e1x * mxref +
                 map.e2x * myref +
                 map.e3x * mzref);
  Real_type const vy =
      map.det * (map.y0  * vref +
                 map.e1y * mxref +
                 map.e2y * myref +
                 map.e3y * mzref);
  Real_type const vz =
      map.det * (map.z0  * vref +
                 map.e1z * mxref +
                 map.e2z * myref +
                 map.e3z * mzref);

  constexpr Int_type TASK = 6 * TTET + DTET;

  partial[(TASK * 4 + 0) * nPairs + ipair] = vv;
  partial[(TASK * 4 + 1) * nPairs + ipair] = vx;
  partial[(TASK * 4 + 2) * nPairs + ipair] = vy;
  partial[(TASK * 4 + 3) * nPairs + ipair] = vz;
}

template < Size_type block_size >
__launch_bounds__(block_size)
__global__ void stage2_tettet_reduce_kernel(
    Real_ptr const partial,
    Size_type const nPairs,
    Real_ptr const vv_out)
{
  Index_type const ipair = blockIdx.x * blockDim.x + threadIdx.x;

  if (ipair >= nPairs) {
    return;
  }

  Real_type vv = Real_type(0.0);
  Real_type vx = Real_type(0.0);
  Real_type vy = Real_type(0.0);
  Real_type vz = Real_type(0.0);

#pragma unroll
  for (Int_type task = 0; task < 36; ++task) {
    vv += partial[(task * 4 + 0) * nPairs + ipair];
    vx += partial[(task * 4 + 1) * nPairs + ipair];
    vy += partial[(task * 4 + 2) * nPairs + ipair];
    vz += partial[(task * 4 + 3) * nPairs + ipair];
  }

  Index_type const std_i = ipair / hexhex_new_fixup_groupsize;
  Index_type const sub_i = ipair % hexhex_new_fixup_groupsize;

  Real_ptr out =
      vv_out +
      hexhex_new_nvals_per_std_intsc * std_i +
      hexhex_new_nvals_per_pair * sub_i;

  out[0] = vv;
  out[1] = vx;
  out[2] = vy;
  out[3] = vz;
}

template < Size_type block_size >
__launch_bounds__(block_size)
__global__ void stage2_subz_aos_to_soa_kernel(
    Real_ptr const aos,
    Size_type const nPairs,
    Real_ptr const soa)
{
  Index_type const i = blockIdx.x * blockDim.x + threadIdx.x;
  Index_type const len = 24 * nPairs;

  if (i >= len) {
    return;
  }

  Int_type const component = i / nPairs;
  Index_type const ipair = i - component * nPairs;

  soa[i] = aos[24 * ipair + component];
}

template <Int_type TTET, Size_type block_size>
__launch_bounds__(block_size)
__global__ void stage2_target_tet_kernel(
    Real_ptr const dsubz_soa,
    Real_ptr const tsubz_soa,
    Size_type const nPairs,
    Real_ptr const vv_out)
{
  Index_type const ipair = blockIdx.x * blockDim.x + threadIdx.x;

  if (ipair >= nPairs) {
    return;
  }

  HexHexTargetTetMapNew map;
  make_target_tet_map_new_fixed_soa<TTET>(
      tsubz_soa, nPairs, ipair, map);

  Real_type vv_sum = Real_type(0.0);
  Real_type vx_sum = Real_type(0.0);
  Real_type vy_sum = Real_type(0.0);
  Real_type vz_sum = Real_type(0.0);

#pragma unroll 1
  for (Int_type dtet = 0; dtet < 6; ++dtet) {
    Real_type x[4], y[4], z[4];

    transform_donor_tet_fixed_runtime_dtet_soa<TTET>(
        dsubz_soa, nPairs, ipair, dtet, map, x, y, z);

    Real_type vref = Real_type(0.0);
    Real_type mxref = Real_type(0.0);
    Real_type myref = Real_type(0.0);
    Real_type mzref = Real_type(0.0);

    intersect_tettet_edgeface_new(x, y, z, vref, mxref, myref, mzref);

    vv_sum += map.det * vref;
    vx_sum += map.det * (map.x0  * vref +
                         map.e1x * mxref +
                         map.e2x * myref +
                         map.e3x * mzref);
    vy_sum += map.det * (map.y0  * vref +
                         map.e1y * mxref +
                         map.e2y * myref +
                         map.e3y * mzref);
    vz_sum += map.det * (map.z0  * vref +
                         map.e1z * mxref +
                         map.e2z * myref +
                         map.e3z * mzref);
  }

  Index_type const std_i = ipair / hexhex_new_fixup_groupsize;
  Index_type const sub_i = ipair % hexhex_new_fixup_groupsize;

  Real_ptr out =
      vv_out +
      hexhex_new_nvals_per_std_intsc * std_i +
      hexhex_new_nvals_per_pair * sub_i;

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
__global__ void intsc_hexhex_new
  ( Real_ptr const dsubz,
    Real_ptr const tsubz,
    Size_type  const nisc_stage,
    Real_ptr vv_int )
{
  __shared__ Real_type vv_reduce[hexhex_new_len_vv_reduce] ;

  Index_type blksize = block_size ;   // blocksize = 64  must <= hexhex_new_tri_per_pair
  Index_type blk     = blockIdx.x ;
  Index_type ith     = blk*blksize + threadIdx.x ;  // which thread with offset
  Index_type thridx  = threadIdx.x ;

  Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;

  INTSC_HEXHEX_NEW_BODY;
}


template < Size_type block_size >
__global__ void intsc_hexhex_new_fixup_vv_64to72
    ( Real_ptr const vv_int,   // [8*intsc blks] blocked volumes, moments
      Size_type const n_szpairs,  // number of subzone pairs
      Real_ptr vv_pair )       // [4*n_szpairs] output voluments, moments
{
  Int_type i = blockIdx.x*block_size + threadIdx.x;
  INTSC_HEXHEX_NEW_FIXUP_VV_BODY ;
}



template < Size_type block_size >
void INTSC_HEXHEX_NEW::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = hexhex_new_tri_per_std_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc= m_n_subz_intsc ;
  const Size_type  nisc_stage  = m_n_subz_intsc ;

  // n_szgrp is number of groups of subzone pairs in fixup kernel.
  // gsize_fixup = fixup kernel grid size (1 thread per group of subzone pairs)
  // iend_fixup = number of threads for fixup kernel.
  //      Kernel has bounds check to mask out excess threads.

  const Size_type  n_szgrp     =
      RAJA_DIVIDE_CEILING_INT(n_subz_intsc, hexhex_new_fixup_groupsize)  ;
  const Size_type  gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, block_size) ;
  const Index_type iend_fixup  = gsize_fixup * block_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

  auto res{getCudaResource()};

  INTSC_HEXHEX_NEW_DATA_SETUP;

  if ( vid == Base_CUDA ) {

    constexpr Size_type stage2_block_size = 128;
    constexpr Size_type stage2_target_tet_kernels = 6;
    constexpr Size_type stage2_transpose_kernels = 2;
    setKernelsPerRep(stage2_transpose_kernels + stage2_target_tet_kernels);
    const Size_type stage2_grid_size =
        RAJA_DIVIDE_CEILING_INT(n_subz_intsc, stage2_block_size);
    const Size_type stage2_soa_len = 24 * n_subz_intsc;
    const Size_type stage2_transpose_grid_size =
        RAJA_DIVIDE_CEILING_INT(stage2_soa_len, stage2_block_size);

    Real_ptr dsubz_soa;
    Real_ptr tsubz_soa;
    allocData(DataSpace::CudaDevice, dsubz_soa, stage2_soa_len);
    allocData(DataSpace::CudaDevice, tsubz_soa, stage2_soa_len);

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      constexpr Size_type shmem = 0;

      RPlaunchCudaKernel( (stage2_subz_aos_to_soa_kernel<stage2_block_size>),
                          stage2_transpose_grid_size, stage2_block_size,
                          shmem, res.get_stream(),
                          m_dsubz, n_subz_intsc, dsubz_soa ) ;

      RPlaunchCudaKernel( (stage2_subz_aos_to_soa_kernel<stage2_block_size>),
                          stage2_transpose_grid_size, stage2_block_size,
                          shmem, res.get_stream(),
                          m_tsubz, n_subz_intsc, tsubz_soa ) ;

#define LAUNCH_TARGET_TET(TTET)                                             \
      RPlaunchCudaKernel(                                                   \
          (stage2_target_tet_kernel<TTET, stage2_block_size>),               \
          stage2_grid_size, stage2_block_size,                               \
          shmem, res.get_stream(),                                           \
          dsubz_soa, tsubz_soa, n_subz_intsc, m_vv_out )

      LAUNCH_TARGET_TET(0);
      LAUNCH_TARGET_TET(1);
      LAUNCH_TARGET_TET(2);
      LAUNCH_TARGET_TET(3);
      LAUNCH_TARGET_TET(4);
      LAUNCH_TARGET_TET(5);

#undef LAUNCH_TARGET_TET

    }
    stopTimer();

    deallocData(DataSpace::CudaDevice, dsubz_soa);
    deallocData(DataSpace::CudaDevice, tsubz_soa);

  } else if ( vid == Lambda_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      const Size_type grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexhex_new_lambda = [=] __device__ ( Index_type i ) {

          __shared__ Real_type vv_reduce[hexhex_new_len_vv_reduce] ;

          Index_type blksize   = block_size ;
          Index_type blk       = i / block_size ;
          Index_type ith       = i ;
          Index_type thridx    = i % block_size ;

          Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;
          INTSC_HEXHEX_NEW_BODY ;
      } ;

      auto intsc_hexhex_new_fixup_lambda = [=] __device__ ( Index_type i ) {
          INTSC_HEXHEX_NEW_FIXUP_VV_BODY ;
      } ;

      constexpr Size_type shmem = 0;

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexhex_new_lambda)>),
                          grid_size, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend,
                          intsc_hexhex_new_lambda );

      RPlaunchCudaKernel( (lambda_cuda_forall<block_size,
                           decltype(intsc_hexhex_new_fixup_lambda)>),
                          gsize_fixup, block_size,
                          shmem, res.get_stream(),
                          ibegin, iend_fixup,
                          intsc_hexhex_new_fixup_lambda );


    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i)
          {
            RAJA_TEAM_SHARED Real_type vv_reduce[hexhex_new_len_vv_reduce] ;

            Index_type blksize   = block_size ;
            Index_type blk       = i / block_size ;
            Index_type ith       = i ;
            Index_type thridx    = i % block_size ;

            Real_ptr vv_int_p = (Real_ptr ) vv_int + 8*blk ;
            INTSC_HEXHEX_NEW_BODY;
          }
      ) ;

      RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] __device__ (Index_type i)
          {
            INTSC_HEXHEX_NEW_FIXUP_VV_BODY ;
          }
      ) ;

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXHEX_NEW : Unknown Cuda variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXHEX_NEW, Cuda, Base_CUDA, Lambda_CUDA, RAJA_CUDA)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
