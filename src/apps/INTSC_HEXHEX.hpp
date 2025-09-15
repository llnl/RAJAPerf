//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_INTSC_HEXHEX_HPP
#define RAJAPerf_Apps_INTSC_HEXHEX_HPP


#include "RAJA/RAJA.hpp"

#include "common/RPTypes.hpp"


RAJA_HOST_DEVICE
RAJA_INLINE void intsc_hexhex_stub(
)
{
}


#include "intsc_hexhex_helper.cpp"




#define  INTSC_HEXHEX_DATA_SETUP \
  Real_ptr const dsubz  = m_dsubz ; \
  Real_ptr const tsubz  = m_tsubz ; \
  Real_ptr       vv_int = m_vv_int ; \
  Real_ptr      vv_pair = m_vv_out ;

#define INTSC_HEXHEX_BODY_SEQ \
  long const n_dsz_tris = 12 ; \
  long const n_tsz_tets = 6 ; \
  long const nth_per_isc = n_dsz_tris * n_tsz_tets ; \
  long ipair   = ith / nth_per_isc ; \
  int dfacet  = ( ith / n_tsz_tets ) % n_dsz_tris ; \
  int ttet    = ith % n_tsz_tets ; \
  long pair_base_thr = ipair * nth_per_isc ; \
  long blk_base = blk * blksize ; \
  double vv_lo=0.0, vx_lo=0.0, vy_lo=0.0, vz_lo=0.0 ; \
  double vv_hi=0.0, vx_hi=0.0, vy_hi=0.0, vz_hi=0.0 ; \
  if ( ipair < nisc_stage ) { \
    double const *xds = dsubz + 24*ipair ; \
    double const *xts = tsubz + 24*ipair ; \
    hex_intsc_subz \
        ( xds, xts, dfacet, ttet, vv_lo, vx_lo, vy_lo, vz_lo ) ; \
  } \
  if ( pair_base_thr > blk_base ) { \
    vv_hi = vv_lo ; \
    vx_hi = vx_lo ; \
    vy_hi = vy_lo ; \
    vz_hi = vz_lo ; \
    vv_lo = 0.0 ; \
    vx_lo = 0.0 ; \
    vy_lo = 0.0 ; \
    vz_lo = 0.0 ; \
  }



#define INTSC_HEXHEX_BODY \
  INTSC_HEXHEX_BODY_SEQ \
  \
  __syncthreads() ; \
  for ( int k = 1 ; k < WARPSIZE ; k *= 2 ) { \
    vv_hi += __shfl_xor_sync ( 0xffffffff, vv_hi, k ) ; \
    vx_hi += __shfl_xor_sync ( 0xffffffff, vx_hi, k ) ; \
    vy_hi += __shfl_xor_sync ( 0xffffffff, vy_hi, k ) ; \
    vz_hi += __shfl_xor_sync ( 0xffffffff, vz_hi, k ) ; \
    vv_lo += __shfl_xor_sync ( 0xffffffff, vv_lo, k ) ; \
    vx_lo += __shfl_xor_sync ( 0xffffffff, vx_lo, k ) ; \
    vy_lo += __shfl_xor_sync ( 0xffffffff, vy_lo, k ) ; \
    vz_lo += __shfl_xor_sync ( 0xffffffff, vz_lo, k ) ; \
  } \
  int const nwarps = blksize / WARPSIZE ; \
  int k = threadIdx.x / WARPSIZE ; \
  if ( threadIdx.x == k*WARPSIZE ) { \
    vv_reduce[k+ 0] = vv_lo ; \
    vv_reduce[k+ 2] = vx_lo ; \
    vv_reduce[k+ 4] = vy_lo ; \
    vv_reduce[k+ 6] = vz_lo ; \
    vv_reduce[k+ 8] = vv_hi ; \
    vv_reduce[k+10] = vx_hi ; \
    vv_reduce[k+12] = vy_hi ; \
    vv_reduce[k+14] = vz_hi ; \
  } \
  __syncthreads() ; \
  if ( threadIdx.x < 8 ) { \
    for ( int k = 1 ; k < nwarps ; ++k ) { \
      vv_reduce[ 2*threadIdx.x ] += vv_reduce[ 2*threadIdx.x + 1 ] ; \
    } \
    vv_out[threadIdx.x] = vv_reduce[ 2 * threadIdx.x ] ; \
  }

//  This is not needed on Seq and OMP CPU variants.
//
#define FIXUP_VV_BODY \
  double *vv          = vv_pair + 32*ith ; \
  double const *vv_in = vv_int  + 72*ith ; \
  int k=0 ; \
  if ( 8*ith + k < n_szpairs ) { \
    vv[4*k+0] = vv_in[8*k+0] + vv_in[8*k+8] ; \
    vv[4*k+1] = vv_in[8*k+1] + vv_in[8*k+9] ; \
    vv[4*k+2] = vv_in[8*k+2] + vv_in[8*k+10] ; \
    vv[4*k+3] = vv_in[8*k+3] + vv_in[8*k+11] ; \
  } \
  for ( int k=1 ; k<8 ; ++k ) { \
    if ( 8*ith + k < n_szpairs ) { \
      vv[4*k+0] = vv_in[8*k+4] + vv_in[8*k+8] ; \
      vv[4*k+1] = vv_in[8*k+5] + vv_in[8*k+9] ; \
      vv[4*k+2] = vv_in[8*k+6] + vv_in[8*k+10] ; \
      vv[4*k+3] = vv_in[8*k+7] + vv_in[8*k+11] ; \
    } \
  }


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace apps
{
class ADomain;

class INTSC_HEXHEX : public KernelBase
{
public:

  INTSC_HEXHEX(const RunParams& params);

  ~INTSC_HEXHEX();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void runSeqVariant(VariantID vid, size_t tune_idx);
  void runOpenMPVariant(VariantID vid, size_t tune_idx);
  void runCudaVariant(VariantID vid, size_t tune_idx);
  void runHipVariant(VariantID vid, size_t tune_idx);
  void runOpenMPTargetVariant(VariantID vid, size_t tune_idx);
  void runSyclVariant(VariantID vid, size_t tune_idx);

  void setCudaTuningDefinitions(VariantID vid);
  void setHipTuningDefinitions(VariantID vid);
  void setSyclTuningDefinitions(VariantID vid);

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  void intscHexHexSeq        ( Index_type i, Index_type iend ) ;
  void intscHexHexOMP        ( Index_type i, Index_type iend ) ;
  void intscHexHexOMP_Target ( Index_type i, Index_type iend ) ;

  void check_intsc_volume_moments
      ( FILE* f, long const n_intsc, double const *vv ) ;

  static constexpr size_t m_tri_per_intsc = 576 ;
  static const size_t default_gpu_block_size = 64;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  VariantID m_vid ;
  Real_ptr m_mydata;
  Real_ptr m_dsubz ;    // donor subzone coordinates
  Real_ptr m_tsubz ;    // target subzone coordinates

  long m_gsize ;        // grid size (number of blocks)
  long m_nthreads ;     // total number of gpu threads (=64*m_gsize)
  Real_ptr m_vv_int ;   // intermediate volumes and moments
  Real_ptr m_vv_out ;   // [4*n_nitsc] computed volumes, moments on device

  Real_ptr m_vv ;       // [4*n_intsc] computed volumes, moments on host

  static constexpr double m_xmin = -0.2 ;  // coordinates of hex corners
  static constexpr double m_xmax = -0.1 ;
  static constexpr double m_ymin =  0.1 ;
  static constexpr double m_ymax =  0.2 ;
  static constexpr double m_zmin = -0.8 ;
  static constexpr double m_zmax = -0.7 ;
  static constexpr double m_shift = 0.01 ; // shift of target relative to donor
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
