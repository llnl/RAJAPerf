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


void INTSC_HEXHEX::intsc_hexhex_setup
    ()
{
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

  // Make contiguous arrays

  m_dcoord = new double[24] ;
  memcpy ( m_dcoord   , xdzone, 8*sizeof(double) ) ;
  memcpy ( m_dcoord+ 8, ydzone, 8*sizeof(double) ) ;
  memcpy ( m_dcoord+16, zdzone, 8*sizeof(double) ) ;

  m_tcoord = new double[24] ;
  memcpy ( m_tcoord   , xtzone, 8*sizeof(double) ) ;
  memcpy ( m_tcoord+ 8, ytzone, 8*sizeof(double) ) ;
  memcpy ( m_tcoord+16, ztzone, 8*sizeof(double) ) ;

  m_vv = new double [ 4L * n_intsc * sizeof(double) ] ;

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
  delete[] m_dcoord ;
  delete[] m_tcoord ;
  delete[] m_vv ;
  fclose ( m_f_geomsubz ) ;
}

void INTSC_HEXHEX::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  //  auto reset_x = allocAndInitDataConstForInit(m_x, m_array_length, Real_type(0.0), vid);
}

void INTSC_HEXHEX::updateChecksum(VariantID vid, size_t tune_idx)
{
  // checksum[vid][tune_idx] += calcChecksum(m_sum, m_array_length, m_checksum_scale_factor, vid  );
}

void INTSC_HEXHEX::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  // deallocData(m_x, vid);
}

} // end namespace apps
} // end namespace rajaperf
