//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXHEX.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


void INTSC_HEXHEX::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_vid = vid ;    // Remember variant to deallocate data.

  // One standard intersection is 8 subzone intersections.
  long n_intsc = 8L*getDefaultProblemSize() ;

  // coordinates for donor zone
  double xdzone[8] =
      { -0.2, -0.1, -0.2, -0.1, -0.2, -0.1, -0.2, -0.1 } ;

  double ydzone[8] =
      { 0.1, 0.1, 0.2, 0.2, 0.1, 0.1, 0.2, 0.2 } ;

  double zdzone[8] =
      { -0.8, -0.8, -0.8, -0.8, -0.7, -0.7, -0.7, -0.7 } ;

  double shift=0.01 ;
  double xtzone[8], ytzone[8], ztzone[8] ;
  for ( int i=0 ; i<8 ; ++i ) {
    xtzone[i] = xdzone[i] + shift ;
    ytzone[i] = ydzone[i] + shift ;
    ztzone[i] = zdzone[i] + shift ;
  }

  printf ( "\n\nnumber of standard intersections = %ld\n", n_intsc ) ;

  FILE *f = fopen ( "geomsubz.out", "w" ) ;

  auto wt_out = [=] ( long const *izone1, double const *vv1 ) {
    if ( fabs(vv1[0]) > 1.0e-20 ) {
      fprintf ( f,
                "intsc = %7d   overlay volume = %19.11e\n"
                "                        x moment =%19.11e\n"
                "                        y moment =%19.11e\n"
                "                        z moment =%19.11e\n"
               , 0, vv1[0], vv1[1], vv1[2], vv1[3] ) ;
    } } ;

  Real_ptr dcoord ;   // donor  coordinates [24]
  Real_ptr tcoord ;   // target coordinates [24]

  // Expanded donor and target coordinates on host
  Real_ptr ds_h, ts_h ;

  do {
    using namespace detail ;

    dcoord = (Real_ptr) allocHostData ( 24*sizeof(double), getDataAlignment() );
    memcpy ( dcoord   , xdzone, 8*sizeof(double) ) ;
    memcpy ( dcoord+ 8, ydzone, 8*sizeof(double) ) ;
    memcpy ( dcoord+16, zdzone, 8*sizeof(double) ) ;

    tcoord = (Real_ptr) allocHostData ( 24*sizeof(double), getDataAlignment() );
    memcpy ( tcoord   , xtzone, 8*sizeof(double) ) ;
    memcpy ( tcoord+ 8, ytzone, 8*sizeof(double) ) ;
    memcpy ( tcoord+16, ztzone, 8*sizeof(double) ) ;

    m_vv = (Real_ptr) allocHostData
        ( 4L*n_intsc*sizeof(double), getDataAlignment() ) ;

    ds_h = (Real_ptr) allocHostData
        ( 24L*n_intsc*sizeof(double) , getDataAlignment() ) ;
    ts_h = (Real_ptr) allocHostData
        ( 24L*n_intsc*sizeof(double) , getDataAlignment() ) ;

  } while ( false ) ;

  //  Repeat the same calculation n_intsc times, expand the
  //  same donor and target zones.
  for ( int k=0 ; k < n_intsc ; ++k ) {
    memcpy ( ds_h + 24L*k, dcoord, 24*sizeof(double) ) ;
    memcpy ( ts_h + 24L*k, tcoord, 24*sizeof(double) ) ;
  }

  allocAndCopyHostData ( m_dsubz, ds_h, 24L*n_intsc, vid ) ;
  allocAndCopyHostData ( m_tsubz, ts_h, 24L*n_intsc, vid ) ;

  do {
    using namespace detail ;
    deallocHostData ( ds_h ) ;
    deallocHostData ( ts_h ) ;
    deallocHostData ( dcoord ) ;
    deallocHostData ( tcoord ) ;
  } while ( false ) ;

  const int block_size = default_gpu_block_size ;
  m_nthreads = 72L * n_intsc ;
  m_gsize    = RAJA_DIVIDE_CEILING_INT(m_nthreads, block_size) ;

  fprintf ( f, "workgroup size                   = %d\n" , block_size );
  fprintf ( f, "number of workgroups             = %ld\n", m_gsize ) ;
  fprintf ( f, "number of threads                = %ld\n", m_nthreads ) ;

  // intermediate volumes, moments
  allocData ( m_vv_int, 8L*m_gsize, vid ) ;

  m_f_geomsubz = f ;
}


INTSC_HEXHEX::INTSC_HEXHEX(const RunParams& params)
  : KernelBase(rajaperf::Apps_INTSC_HEXHEX, params)
{
  constexpr size_t number_of_intsc = 100*100*100 ;
  setDefaultProblemSize(number_of_intsc);
  setDefaultReps(10);

  setActualProblemSize( getDefaultProblemSize() );

  setItsPerRep( number_of_intsc );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  setBytesReadPerRep( 48*sizeof(Real_type) * getItsPerRep() );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr size_t flops_per_tri = 700 ;
  constexpr size_t tri_per_intsc = 576 ;
  constexpr size_t flops_per_intsc = flops_per_tri * tri_per_intsc ;

  setFLOPsPerRep(number_of_intsc * flops_per_intsc);

  checksum_scale_factor = 0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() );

  setComplexity(Complexity::N);

  setUsesFeature(Forall);

  setVariantDefined( Base_Seq );
  setVariantDefined( Lambda_Seq );
  setVariantDefined( RAJA_Seq );

  setVariantDefined( Base_OpenMP );
  setVariantDefined( Lambda_OpenMP );
  setVariantDefined( RAJA_OpenMP );

  setVariantDefined( Base_OpenMPTarget );
  setVariantDefined( RAJA_OpenMPTarget );

  setVariantDefined( Base_CUDA );
  setVariantDefined( Lambda_CUDA );
  setVariantDefined( RAJA_CUDA );

  setVariantDefined( Base_HIP );
  setVariantDefined( Lambda_HIP );
  setVariantDefined( RAJA_HIP );

  setVariantDefined( Base_SYCL );
  setVariantDefined( RAJA_SYCL );
}

INTSC_HEXHEX::~INTSC_HEXHEX()
{
}



void INTSC_HEXHEX::updateChecksum(VariantID vid, size_t tune_idx)
{
  // checksum[vid][tune_idx] += calcChecksum(m_sum, m_array_length, m_checksum_scale_factor, vid  );
}

void INTSC_HEXHEX::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  using namespace detail ;
  deallocHostData ( m_vv ) ;
  deallocData ( m_dsubz, vid ) ;
  deallocData ( m_tsubz, vid ) ;
  deallocData ( m_vv_int, vid ) ;

  fclose ( m_f_geomsubz ) ;
}

} // end namespace apps
} // end namespace rajaperf
