//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/// Intersection between two 24-sided hexahedrons, volume and moments.
///
///  for ( itri=0 ; itri < 576*n_std_intsc ; ++itri ) {
///   long const n_dsz_tris = 12 ;
///   long const n_tsz_tets = 6 ;
///   long const nth_per_isc = n_dsz_tris * n_tsz_tets ;
///   long ipair   = itri / nth_per_isc ;
///   int dfacet  = ( itri / n_tsz_tets ) % n_dsz_tris ;
///   int ttet    = itri % n_tsz_tets ;
///   long pair_base_thr = ipair * nth_per_isc ;
///   long blk_base = blk * blksize ;
///   double vv=0.0, vx=0.0, vy=0.0, vz=0.0 ;
///
///   double const *xds = dsubz + 24*ipair ;
///   double const *xts = tsubz + 24*ipair ;
///   hex_intsc_subz ( xds, xts, dfacet, ttet, vv, vx, vy, vz ) ;
///
///   vv_out[4*ipair+0]  = vv ;
///   vv_out[4*ipair+1]  = vx ;
///   vv_out[4*ipair+2]  = vy ;
///   vv_out[4*ipair+3]  = vz ;
///  }
///
/// void hex_intsc_subz
///     ( double const *xds, double const *xts,
///       int const dfacet, int const ttet,
///       double &vv, double &vx, double &vy, double &vz )
///    {
///      double const *yds = xds + 8 ;
///      double const *zds = yds + 8 ;
///      double const *yts = xts + 8 ;
///      double const *zts = yts + 8 ;
///
///      double xdt[3], ydt[3], zdt[3] ;
///      copy_donor_triangle ( xdt, ydt, zdt, xds, yds, zds ) ;
///      double xtt[4], ytt[4], ztt[4] ;
///      copy_target_tet ( xtt, ytt, ztt, xts, yts, zts ) ;
///
///      cuda_intsc_tri_tet
///       ( xdt, ydt, zdt, xtt, ytt, ztt, vv, vx, vy, vz ) ;
///    }


#ifndef RAJAPerf_Apps_INTSC_HEXHEX_HPP
#define RAJAPerf_Apps_INTSC_HEXHEX_HPP


#include "RAJA/RAJA.hpp"

#include "common/RPTypes.hpp"


RAJA_HOST_DEVICE
RAJA_INLINE void intsc_hexhex_stub(
)
{
}


#include "intsc_hexhex_body.hpp"


#define  INTSC_HEXHEX_DATA_SETUP \
  Real_ptr const dsubz  = m_dsubz ; \
  Real_ptr const tsubz  = m_tsubz ; \
  Real_ptr       vv_int = m_vv_int ; \
  Real_ptr      vv_pair = m_vv_out ;


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

  void check_intsc_volume_moments ( long const n_intsc, Real_const_ptr vv ) ;

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

  static constexpr Real_type m_xmin = -0.2 ;  // coordinates of hex corners
  static constexpr Real_type m_xmax = -0.1 ;
  static constexpr Real_type m_ymin =  0.1 ;
  static constexpr Real_type m_ymax =  0.2 ;
  static constexpr Real_type m_zmin = -0.8 ;
  static constexpr Real_type m_zmax = -0.7 ;
  static constexpr Real_type m_shift = 0.01 ; // shift target relative to donor
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
