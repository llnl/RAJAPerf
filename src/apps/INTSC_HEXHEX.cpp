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

  //   Run a smaller problem in sequential because it's slow.
  long factor = 1L ;
  if ( ( vid == Base_Seq ) or ( vid == Lambda_Seq ) or ( vid == RAJA_Seq ) ) {
    factor = 8L ;
  }

  setActualProblemSize( getDefaultProblemSize() / factor );

  // One standard intersection is 8 subzone intersections.
  long n_std_intsc  = getActualProblemSize() ;
  long n_subz_intsc = 8L * n_std_intsc ;

  // coordinates for donor zone
  double xdzone[8] =
      { m_xmin, m_xmax, m_xmin, m_xmax, m_xmin, m_xmax, m_xmin, m_xmax } ;

  double ydzone[8] =
      { m_ymin, m_ymin, m_ymax, m_ymax, m_ymin, m_ymin, m_ymax, m_ymax } ;

  double zdzone[8] =
      { m_zmin, m_zmin, m_zmin, m_zmin, m_zmax, m_zmax, m_zmax, m_zmax } ;

  double xtzone[8], ytzone[8], ztzone[8] ;
  for ( int i=0 ; i<8 ; ++i ) {
    xtzone[i] = xdzone[i] + m_shift ;
    ytzone[i] = ydzone[i] + m_shift ;
    ztzone[i] = zdzone[i] + m_shift ;
  }

  FILE *f = fopen ( "geomsubz.out", "w" ) ;

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
        ( 4L*n_subz_intsc*sizeof(double), getDataAlignment() ) ;

    ds_h = (Real_ptr) allocHostData
        ( 24L*n_subz_intsc*sizeof(double) , getDataAlignment() ) ;
    ts_h = (Real_ptr) allocHostData
        ( 24L*n_subz_intsc*sizeof(double) , getDataAlignment() ) ;

  } while ( false ) ;

  //  Repeat the same calculation n_subz_intsc times, expand the
  //  same donor and target zones.
  for ( int k=0 ; k < n_subz_intsc ; ++k ) {
    memcpy ( ds_h + 24L*k, dcoord, 24*sizeof(double) ) ;
    memcpy ( ts_h + 24L*k, tcoord, 24*sizeof(double) ) ;
  }

  allocAndCopyHostData ( m_dsubz, ds_h, 24L*n_subz_intsc, vid ) ;
  allocAndCopyHostData ( m_tsubz, ts_h, 24L*n_subz_intsc, vid ) ;

  do {
    using namespace detail ;
    deallocHostData ( ds_h ) ;
    deallocHostData ( ts_h ) ;
    deallocHostData ( dcoord ) ;
    deallocHostData ( tcoord ) ;
  } while ( false ) ;

  const int block_size = default_gpu_block_size ;
  m_nthreads = 72L * n_subz_intsc ;
  m_gsize    = RAJA_DIVIDE_CEILING_INT(m_nthreads, block_size) ;

  fprintf ( f, "workgroup size                   = %d\n" , block_size );
  fprintf ( f, "number of workgroups             = %ld\n", m_gsize ) ;
  fprintf ( f, "number of threads                = %ld\n", m_nthreads ) ;

  // intermediate volumes, moments
  allocData ( m_vv_int, 8L*m_gsize, vid ) ;

  allocAndInitDataConst ( m_vv_out, 4L*n_subz_intsc, 0.0, vid ) ;

  m_f_geomsubz = f ;
}


//   Number of subzone intersections = 8 * number of standard intersections.
//
void INTSC_HEXHEX::check_intsc_volume_moments
    ( FILE* f,
      long const n_subz_intsc,  // number of subzone intersections
      double const *vv )   // computed volumes, moments on the host
{
  int rank = 0;
#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif // RAJA_PERFSUITE_ENABLE_MPI

  // Check on rank 0, other ranks are identical.
  if ( rank == 0 ) {

    long n_std_intsc = getActualProblemSize() ;
    printf ( "\n\nnumber of standard intersections = %ld\n", n_std_intsc ) ;

    //   Determine the correct volume and moments.
    double v0, vx, vy, vz ;

    double xmin = m_xmin, ymin = m_ymin, zmin = m_zmin ;
    double xmax = m_xmax, ymax = m_ymax, zmax = m_zmax ;

    if ( m_shift > 0.0 ) {
      xmin += m_shift ;   ymin += m_shift ;   zmin += m_shift ;
    } else {
      xmax -= m_shift ;   ymax -= m_shift ;   zmax -= m_shift ;
    }
    double dx = xmax - xmin, dy = ymax - ymin, dz = zmax - zmin ;
    if ( dx <= 0.0 or dy <= 0.0 or dz <= 0.0 ) {
      v0 = vx = vy = vz = 0.0 ;
    } else {
      double xc = 0.5 * ( xmax + xmin ) ;
      double yc = 0.5 * ( ymax + ymin ) ;
      double zc = 0.5 * ( zmax + zmin ) ;

      v0 = dx * dy * dz ;
      vx = v0 * xc ;
      vy = v0 * yc ;
      vz = v0 * zc ;
    }

    fprintf ( f, " correct   volume = %19.11e\n"
              " correct x moment = %19.11e\n"
              " correct y moment = %19.11e\n"
              " correct z moment = %19.11e\n", v0, vx, vy, vz ) ;

    // Do the check.
    double tolsq = 1.0e-24 ;
    double tolsqv = tolsq * v0*v0 ;
    double tolsqx = tolsq * v0*v0 *
        ( fabs(xmax) + fabs(xmin) ) *  ( fabs(xmax) + fabs(xmin) ) ;
    double tolsqy = tolsq * v0*v0 *
        ( fabs(ymax) + fabs(ymin) ) *  ( fabs(ymax) + fabs(ymin) ) ;
    double tolsqz = tolsq * v0*v0 *
        ( fabs(zmax) + fabs(zmin) ) *  ( fabs(zmax) + fabs(zmin) ) ;
    printf ( "tolsqv = %13.5e\ntolsqx = %13.5e\ntolsqy = %13.5e\ntolsqz = %13.5e\n", tolsqv, tolsqx, tolsqy, tolsqz ) ;
    bool correct = true ;
    for ( long k = 0 ; k < n_subz_intsc ; ++k ) {
      double dv  = vv[ 4*k + 0 ] - v0 ;   // diff between computed and correct
      double dxm = vv[ 4*k + 1 ] - vx ;
      double dym = vv[ 4*k + 2 ] - vy ;
      double dzm = vv[ 4*k + 3 ] - vz ;
      if ( ( dv*dv   > tolsqv ) or
           ( dxm*dxm > tolsqx ) or
           ( dym*dym > tolsqy ) or
           ( dzm*dzm > tolsqz ) ) {
        correct = false ;
        fprintf ( f, "k = %ld    vv = %19.11e\n"
                  "k = %ld    vx = %19.11e\n"
                  "k = %ld    vy = %19.11e\n"
                  "k = %ld    vz = %19.11e\n", k, vv[4*k],
                  k, vv[4*k+1], k, vv[4*k+2], k, vv[4*k+3] ) ;
        break ;
      }
      if ( k % (n_subz_intsc-1) == 0 ) {
        fprintf ( f, "k = %9ld    vv = %24.16e\n"
                  "k = %9ld    vx = %24.16e\n"
                  "k = %9ld    vy = %24.16e\n"
                  "k = %9ld    vz = %24.16e\n", k, vv[4*k],
                  k, vv[4*k+1], k, vv[4*k+2], k, vv[4*k+3] ) ;
      }
    }
    if ( correct ) {
      fprintf ( f, "%s", "Volumes and moments are correct.\n" ) ;
    } else {
      fprintf ( f, "%s", "Volumes and moments are INCORRECT.\n" ) ;
    }
  }
}


INTSC_HEXHEX::INTSC_HEXHEX(const RunParams& params)
  : KernelBase(rajaperf::Apps_INTSC_HEXHEX, params)
{
  //  one standard intersection = eight subzone intersections.
  //  Set number of standard intersections here.
  //
  constexpr size_t num_std_intsc = 100*100*100 ;
  setDefaultProblemSize(num_std_intsc);
  setDefaultReps(1);

  setItsPerRep( num_std_intsc );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  setBytesReadPerRep( 48*sizeof(Real_type) * getItsPerRep() );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr size_t flops_per_tri = 700 ;
  constexpr size_t flops_per_intsc = flops_per_tri * m_tri_per_intsc ;

  setFLOPsPerRep(num_std_intsc * flops_per_intsc);

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

// put setup here

void INTSC_HEXHEX::updateChecksum(VariantID vid, size_t tune_idx)
{
  // One standard intersection is 8 subzone intersections.
  long n_std_intsc  = getActualProblemSize() ;
  long n_subz_intsc = 8L * n_std_intsc ;

  copyData ( DataSpace::Host, m_vv,
             getDataSpace(vid), m_vv_out, 4L*n_subz_intsc ) ;

  check_intsc_volume_moments
      ( stdout, n_subz_intsc, m_vv ) ;

  detail::deallocHostData ( m_vv ) ;

  checksum[vid][tune_idx] += calcChecksum(m_vv_out, 4L*n_subz_intsc, vid  );
}

void INTSC_HEXHEX::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData ( m_dsubz, vid ) ;
  deallocData ( m_tsubz, vid ) ;
  deallocData ( m_vv_int, vid ) ;
  deallocData ( m_vv_out, vid ) ;

  fclose ( m_f_geomsubz ) ;
}

} // end namespace apps
} // end namespace rajaperf
