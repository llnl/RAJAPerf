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




RAJA_HOST_DEVICE
RAJA_INLINE void clip_polygon_ge_0
    ( double *cin,   // the cut coordinate, can be xin, yin, or zin.
      double *xin, double *yin, double *zin, double *hin, // input coordinates
      int &first, int &avail, int *next )   // linked list
{
  int j  = first ;

  int first0 = first ;
  int j1 = -1, j2 = -1 ;
  int jj1 = -1, jj2 = -1 ;

  double c0 = ( j >= 0 ) ? cin[j] : 0.0 ;
  double c00 = c0 ;
  double clast = c0 ;

  while ( j >= 0 ) {
    int jj = next[j] ;
    int jp = jj ;       // advancing, jp is -1 at end.
    if ( jj < 0 ) { jj = first0 ; }   // last edge of polygon

    double c1 = cin[jj] ;
    if ( ( c0 >= 0 ) and ( c1 < 0 ) ) {
      j1 = j ;
      jj1 = jj ;
    }
    if ( ( c0 < 0 ) and ( c1 >= 0 ) ) {
      j2 = j ;
      jj2 = jj ;
    }
    j = jp ;
    clast = c0 ;
    c0 = c1 ;
  }

  int jr1=-1, jr2=-1 ;

  if ( j1 >= 0 ) {   // Insert first crossover point

    jr1 = avail ;
    avail = next[avail] ;
    double eta = ( 0.0 - cin[jj1] ) / ( cin[j1] - cin[jj1] ) ;
    xin[jr1] = xin[j1] * eta + xin[jj1] * ( 1.0 - eta ) ;
    yin[jr1] = yin[j1] * eta + yin[jj1] * ( 1.0 - eta ) ;
    zin[jr1] = zin[j1] * eta + zin[jj1] * ( 1.0 - eta ) ;
    hin[jr1] = hin[j1] * eta + hin[jj1] * ( 1.0 - eta ) ;

    jr2 = avail ;      // Insert second crossover point
    avail = next[avail] ;
    eta = ( 0.0 - cin[j2] ) / ( cin[jj2] - cin[j2] ) ;
    xin[jr2] = xin[jj2] * eta + xin[j2] * ( 1.0 - eta ) ;
    yin[jr2] = yin[jj2] * eta + yin[j2] * ( 1.0 - eta ) ;
    zin[jr2] = zin[jj2] * eta + zin[j2] * ( 1.0 - eta ) ;
    hin[jr2] = hin[jj2] * eta + hin[j2] * ( 1.0 - eta ) ;
  }

  first = -1 ;

  j = first0 ;
  while ( j >= 0 ) {   // Make removed points available.
    int jp = next[j] ;
    if ( cin[j] < 0.0 ) {
      next[j] = avail ;
      avail = j ;
    } else if ( first == -1 ) {
      first = j ;        // Set first point for output polygon.
    }
    j = jp ;
  }


  if ( j1 >= 0 ) {     // Set linked list for crossover points.
    next[j1] = jr1 ;
    next[jr1] = jr2 ;
    next[jr2] = ( ( clast < 0 ) or ( c00 < 0 ) ) ? -1 : jj2 ;
  }
  return ;
}



//   Simplified volume calculation, for the area under one
//   polygonal face, on the Cuda device.
//   Planar polygon.
//   Compute volume, moments between polygon and the z=0 plane.
RAJA_HOST_DEVICE
RAJA_INLINE void cuda_hex_volpolyh_1poly
    ( double *x, double *y, double *z,
      int const first,
      int const *next,
      double &vv,
      double &vx,
      double &vy,
      double &vz )
{
  if ( first < 0 ) { return ; }   // No polygon remains after clipping.

  int j0 = first ;

  double x0  = x[j0] ;
  double y0  = y[j0] ;
  double z0  = z[j0] ;

  int j1 = next[j0] ;

  double x1  = x[j1] ;
  double y1  = y[j1] ;
  double z1  = z[j1] ;
  double dx1 = x1 - x0 ;
  double dy1 = y1 - y0 ;

  int j2 = next[j1] ;

  while ( j2 >= 0 ) {   // Vertices

    double x2  = x[j2] ;
    double y2  = y[j2] ;
    double z2  = z[j2] ;
    double dx2 = x2 - x0 ;
    double dy2 = y2 - y0 ;

    double area2 = (dx1 * dy2 - dx2 * dy1) ;
    double v0 = ( z0 + z1 + z2 ) * area2 ;
    vv += v0 ;
    vx += v0 * ( x0 + x1 + x2 ) + area2 * ( x0 * z0 + x1 * z1 + x2 * z2 );
    vy += v0 * ( y0 + y1 + y2 ) + area2 * ( y0 * z0 + y1 * z1 + y2 * z2 );
    vz += ( z0 * z0 + z1 * z1 + z2 * z2 + z0 * z1 + z0 * z2 + z1 * z2 )
        * area2 ;

    x1=x2 ;   y1=y2 ;   z1=z2 ;    // Rotate.
    dx1=dx2 ; dy1=dy2 ;

    j2 = next[j2] ;
  }
}



RAJA_HOST_DEVICE
RAJA_INLINE void cuda_intsc_tri_tet
    ( double const xdt[3],    // donor triangle coordinates
      double const ydt[3],
      double const zdt[3],
      double xtt[4],    // target tet coordinates (modified here)
      double ytt[4],
      double ztt[4],
      double &vv_thr,     // volume contribution for this triangle-tet
      double &vx_thr,     // x moment contribution for this triangle-tet
      double &vy_thr,     // y moment contribution for this triangle-tet
      double &vz_thr )    // z moment contribution for this triangle-tet
{
  double det, deti ;
  double ha[9] ;      // 1 - x - y - z

  double xa[9], ya[9], za[9], h2[10] ;
  int *next1 = (int*) h2 ;
  int *next  = next1 + 10 ;

  double vv = 0.0, vx = 0.0, vy = 0.0, vz = 0.0 ;  // volume, moments.

  xtt[1] -= xtt[0] ;
  xtt[2] -= xtt[0] ;
  xtt[3] -= xtt[0] ;
  ytt[1] -= ytt[0] ;
  ytt[2] -= ytt[0] ;
  ytt[3] -= ytt[0] ;
  ztt[1] -= ztt[0] ;
  ztt[2] -= ztt[0] ;
  ztt[3] -= ztt[0] ;

  det =
      xtt[1]  *  ytt[2]  *  ztt[3]  - xtt[1]  *  ytt[3]  *  ztt[2]  +
      xtt[2]  *  ytt[3]  *  ztt[1]  - xtt[2]  *  ytt[1]  *  ztt[3]  +
      xtt[3]  *  ytt[1]  *  ztt[2]  - xtt[3]  *  ytt[2]  *  ztt[1]  ;
  deti = det / ( det*det + 1.0e-100 ) ;

  // Cross products.
  double cyz = ytt[2] * ztt[3] - ztt[2] * ytt[3] ;
  double czx = ztt[2] * xtt[3] - xtt[2] * ztt[3] ;
  double cxy = xtt[2] * ytt[3] - ytt[2] * xtt[3] ;

  //   Coordinates of the facet in the transformed frame.
  xa[0] = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
      (zdt[0] - ztt[0]) * cxy ;
  xa[1] = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
      (zdt[1] - ztt[0]) * cxy ;
  xa[2] = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
      (zdt[2] - ztt[0]) * cxy ;

  cyz = ytt[3] * ztt[1] - ztt[3] * ytt[1] ;
  czx = ztt[3] * xtt[1] - xtt[3] * ztt[1] ;
  cxy = xtt[3] * ytt[1] - ytt[3] * xtt[1] ;

  ya[0] = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
      (zdt[0] - ztt[0]) * cxy ;
  ya[1] = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
      (zdt[1] - ztt[0]) * cxy ;
  ya[2] = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
      (zdt[2] - ztt[0]) * cxy ;

  cyz = ytt[1] * ztt[2] - ztt[1] * ytt[2] ;
  czx = ztt[1] * xtt[2] - xtt[1] * ztt[2] ;
  cxy = xtt[1] * ytt[2] - ytt[1] * xtt[2] ;

  za[0] = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
      (zdt[0] - ztt[0]) * cxy ;
  za[1] = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
      (zdt[1] - ztt[0]) * cxy ;
  za[2] = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
      (zdt[2] - ztt[0]) * cxy ;

  xa[0] *= deti ;    xa[1] *= deti ;    xa[2] *= deti ;
  ya[0] *= deti ;    ya[1] *= deti ;    ya[2] *= deti ;
  za[0] *= deti ;    za[1] *= deti ;    za[2] *= deti ;

  //  Clip on h2 first.
  ha[0] = 1.0 - xa[0] - ya[0] - za[0] ;
  ha[1] = 1.0 - xa[1] - ya[1] - za[1] ;
  ha[2] = 1.0 - xa[2] - ya[2] - za[2] ;
  h2[0] = 1.0 - xa[0] - ya[0] ;
  h2[1] = 1.0 - xa[1] - ya[1] ;
  h2[2] = 1.0 - xa[2] - ya[2] ;

  //  Initialize triangle and available slots.
  next[0] = 1 ;   next[1] = 2 ;   next[2] = -1 ;
  next[3] = 4 ;   next[4] = 5 ;   next[5] = 6 ;  next[6] = 7 ;
  next[7] = 8 ;   next[8] = -1 ;

  int first = 0 ;
  int avail = 3 ;

  clip_polygon_ge_0
      ( h2, xa, ya, za, ha, first, avail, next ) ;

  //  Clip on Cartesian faces of the unit tet.
  clip_polygon_ge_0
      ( xa, xa, ya, za, ha, first, avail, next ) ;

  clip_polygon_ge_0
      ( ya, xa, ya, za, ha, first, avail, next ) ;

  clip_polygon_ge_0
      ( za, xa, ya, za, ha, first, avail, next ) ;

  int first1 = first, avail1 = avail;
  for ( int k = 0 ; k < 9 ; ++k ) {
    next1[k] = next[k] ;
  }

  //  Clip on h>=0

  clip_polygon_ge_0
      ( ha, xa, ya, za, ha, first, avail, next ) ;


  cuda_hex_volpolyh_1poly( xa, ya, za, first, next, vv, vx, vy, vz ) ;


  //  In dimensionless transformed coordinates, quantity smaller
  // than machine epsilon is not significant.
  int j = first1 ;
  while ( j >= 0 ) {
    ha[j] = -ha[j] - 1.0e-50 ;
    j = next1[j] ;
  }

  // Clip on h<0
  clip_polygon_ge_0
      ( ha, xa, ya, za, ha, first1, avail1, next1 ) ;

  //  project to unit tet.
  j = first1 ;
  while ( j >= 0 ) {
    za[j] = 1.0 - xa[j] - ya[j] ;
    j = next1[j] ;
  }

  cuda_hex_volpolyh_1poly( xa, ya, za, first1, next1, vv, vx, vy, vz ) ;

  //  Volume, moments of the intersection in the unit tet frame.
  vv *= 0.16666666666666667 ;
  vx *= 0.041666666666666667 ;
  vy *= 0.041666666666666667 ;
  vz *= 0.041666666666666667 ;

  //   Transform moments to the physical frame.
  vx_thr += det * (xtt[0] * vv + xtt[1] * vx + xtt[2] * vy + xtt[3] * vz);
  vy_thr += det * (ytt[0] * vv + ytt[1] * vx + ytt[2] * vy + ytt[3] * vz);
  vz_thr += det * (ztt[0] * vv + ztt[1] * vx + ztt[2] * vy + ztt[3] * vz);

  //   Transform intersection volume to the physical frame.
  vv_thr += det * vv ;
}



//  Compute the contribution of a donor triangle and a target tet
//         to intersection between hex subzones.
//   Each subzone is twelve triangular facets (six tets).
//
RAJA_HOST_DEVICE
RAJA_INLINE void hex_intsc_subz
    ( double const *xds,    //  [24] donor subzone coords
      double const *xts,    //  [24] target subzone coords
      int const dfacet,     // which donor facet
      int const ttet,       // which target tet
      double &vv_thr,     // volume contribution for this triangle-tet
      double &vx_thr,     // x moment contribution for this triangle-tet
      double &vy_thr,     // y moment contribution for this triangle-tet
      double &vz_thr )    // z moment contribution for this triangle-tet
{
  double const *yds = xds + 8 ;
  double const *zds = yds + 8 ;

  double const *yts = xts + 8 ;
  double const *zts = yts + 8 ;

  vv_thr = 0.0 ;
  vx_thr = 0.0 ;
  vy_thr = 0.0 ;
  vz_thr = 0.0 ;

  int const n_dfacets = 12 ;
  int const len_cycnod = n_dfacets / 2 + 1 ;

  //  coordinates of the donor triangle
  double xdt[3], ydt[3], zdt[3] ;

  do {
    //  cyclic nodes to form facets with node 0.
    int cyc_nod[len_cycnod] = { 1, 5, 4, 6, 2, 3, 1 } ;

    // which subzone vertices form the triangular facet.
    int v0, v1, v2 ;
    if ( dfacet < 6 ) {
      v0 = 0 ;
      v1 = cyc_nod[dfacet] ;
      v2 = cyc_nod[dfacet+1] ;
    } else {
      v0 = 7 ;
      v1 = cyc_nod[n_dfacets-dfacet] ;
      v2 = cyc_nod[n_dfacets-dfacet - 1] ;  // reverse order
    }

    //  Donor triangle coordinates.
    xdt[0] = xds[v0] ;    // Donor facet vertices
    xdt[1] = xds[v1] ;
    xdt[2] = xds[v2] ;
    ydt[0] = yds[v0] ;
    ydt[1] = yds[v1] ;
    ydt[2] = yds[v2] ;
    zdt[0] = zds[v0] ;
    zdt[1] = zds[v1] ;
    zdt[2] = zds[v2] ;

  } while ( false ) ;

  //   Set up the target tet and do the intersections.

  double xtt[4], ytt[4], ztt[4] ;

  xtt[0] = xts[0] ;
  ytt[0] = yts[0] ;
  ztt[0] = zts[0] ;

  //  subzone vertices that form the cycle for tets.
  int vert_cyc[6] = { 1, 3, 2, 6, 4, 5 } ;

  int v1 = vert_cyc[ttet] ;
  xtt[1] = xts[v1] ;
  ytt[1] = yts[v1] ;
  ztt[1] = zts[v1] ;
  int v2 = vert_cyc[(ttet+1)%6] ;
  xtt[2] = xts[v2] ;
  ytt[2] = yts[v2] ;
  ztt[2] = zts[v2] ;
  xtt[3] = xts[7] ;
  ytt[3] = yts[7] ;
  ztt[3] = zts[7] ;

  cuda_intsc_tri_tet
      ( xdt, ydt, zdt, xtt, ytt, ztt,
        vv_thr, vx_thr, vy_thr, vz_thr ) ;
}



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
