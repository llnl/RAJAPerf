//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>


namespace rajaperf
{
namespace apps
{


void INTSC_HEXRECT::copyTargetToDevice
    ( double const **planes, // [3] Target mesh planes in (z,y,x)
      int const* ncord )     // [3] number of target zones in (z,y,x)
{
  int my_ncord[4] = {0} ;     // Fourth integer for alignment.
  my_ncord[2] = ncord[2] ;    // x
  my_ncord[1] = ncord[1] ;    // y
  my_ncord[0] = ncord[0] ;    // z

  // ncord is the number of zones in each direction.
  long nplanes = my_ncord[2] + my_ncord[1] + my_ncord[0] + 3 ;

  // Pack the coordinates together, on the GPU.
  //  Allocate 3 spots for pointers to planes arrays, to set on GPU.
  long planes_size = 4 * sizeof(int) + nplanes * sizeof(double) + 3 * sizeof(void*) ;

  char *ncord_host = new char [ planes_size ] ;  // host buffer
  memset ( ncord_host, 0, planes_size ) ;

  //  Build the buffer on the host in order to reduce the number
  //  of cudaMemcpy calls which are slow.
  memcpy ( ncord_host, my_ncord, 4*sizeof(int) ) ;

  long pos = 4 * sizeof(int) ;
  pos += 3 * sizeof(void*) ;    // pointers to planes arrays
  for ( long dir = 0 ; dir < 3 ; ++dir ) {   // Loop over directions.
    if ( my_ncord[dir] > 0 ) {
      long n = (my_ncord[dir]+1)*sizeof(double) ;   // copy bytes
      memcpy ( ncord_host + pos, planes[dir], n ) ;
      pos   += n ;
    }
  }

  //   Copy the buffer to the device.
  allocAndCopyHostData ( m_ncord, ncord_host, planes_size, m_vid ) ;
  delete[] (ncord_host) ;
}


//   Set up the Cartesian target mesh.  It is a series of Cartesian planes
// in each direction.
//
//  The target mesh is one more than the number of donor zones in
// each direction, so that each donor zone may intersect eight target zones.
//
void INTSC_HEXRECT::setupTargetPlanes
    ( double **planes, int *ncord,
      int const ndx, int const ndy, int const ndz,  // donor zones each dir.
      double const x0, double const y0, double const z0,  // corner
      double const sep )    // plane pitch (separation)
{
  int nx = ndx + 2 ;     // number of target planes
  int ny = ndy + 2 ;
  int nz = ndz + 2 ;
  int ntx = ndx + 1 ;    // number of target zones each direction
  int nty = ndy + 1 ;
  int ntz = ndz + 1 ;

  planes[0] = new double[nx+ny+nz] ;
  planes[1] = planes[0] + nz ;
  planes[2] = planes[1] + ny ;

  //  Target mesh plane coordinates.
  for ( int k=0 ; k < nz ; ++k ) {  planes[0][k] = z0 + k*sep ; }
  for ( int k=0 ; k < ny ; ++k ) {  planes[1][k] = y0 + k*sep ; }
  for ( int k=0 ; k < nx ; ++k ) {  planes[2][k] = x0 + k*sep ; }

  ncord[0] = ntz ;    ncord[1] = nty ;    ncord[2] = ntx ;

}


//  Set up the donor mesh.  It is Cartesian for this test but it
//  is really an unstructured mesh for the geometry kernel.
//
//  Zones are slightly smaller than the target mesh zones, so that the
//  volumes of intersection vary.
//
void INTSC_HEXRECT::setupDonorMesh
    ( double const sep,    // Target mesh plane pitch (separation)
      double const xd0, double const yd0, double const zd0,  // donor corner
      int const ndx, int const ndy, int const ndz,  // donor zones each dir.
      double *x, double *y, double *z,   // node coordinates (output)
      int *znlist )     // zone node list.  Kernel uses indirect addressing.
{
  //  slightly smaller zone widths for donor mesh.
  m_sep1x = sep * ( 1.0 - 0.5 / (double)(ndx+1) ) ;
  m_sep1y = sep * ( 1.0 - 0.5 / (double)(ndy+1) ) ;
  m_sep1z = sep * ( 1.0 - 0.5 / (double)(ndz+1) ) ;

  for ( int kz = 0 ; kz < ndz+1 ; ++kz ) {
    for ( int ky = 0 ; ky < ndy+1 ; ++ky ) {
      for ( int kx = 0 ; kx < ndx+1 ; ++kx ) {
        int node = kx + (ndx+1) * (ky + (ndy+1) * kz)  ;

        x[node] = xd0 + kx * m_sep1x ;
        y[node] = yd0 + ky * m_sep1y ;
        z[node] = zd0 + kz * m_sep1z ;

      }}}

  for ( int jz = 0 ; jz < ndz ; ++jz ) {
    for ( int jy = 0 ; jy < ndy ; ++jy ) {
      for ( int jx = 0 ; jx < ndx ; ++jx ) {

        int zone = jx + ndx * (jy + ndy * jz ) ;

        int node0 = jx + (ndx+1) * (jy + (ndy+1) * jz ) ;

        znlist[ 8*zone     ] = node0 ;
        znlist[ 8*zone + 1 ] = node0 + 1 ;
        znlist[ 8*zone + 2 ] = node0 + (ndx+1) ;
        znlist[ 8*zone + 3 ] = node0 + (ndx+1) + 1 ;
        znlist[ 8*zone + 4 ] = node0 + (ndx+1)*(ndy+1) ;
        znlist[ 8*zone + 5 ] = node0 + (ndx+1)*(ndy+1) + 1 ;
        znlist[ 8*zone + 6 ] = node0 + (ndx+1)*(ndy+1) + (ndx+1) ;
        znlist[ 8*zone + 7 ] = node0 + (ndx+1)*(ndy+1) + (ndx+1) + 1 ;
      }}}
}


//   Determine which pairs of donor and target zones intersect.
//
//   Normally this requires a filtering algorithm but for this
//   test we can calculate it from the alignment of the donor and
//   target meshes.
//
void INTSC_HEXRECT::setupIntscPairs
    ( int const *ncord,    // number of target zones each direction
      int const ndx, int const ndy, int const ndz,  // donor zones each dir.
      int *intsc_d,    // Donor zone of each intersecting pair
      int *intsc_t )   // Target zone of each intersecting pair
{
  int ntx = ncord[2] ;    // number of target zones each direction
  int nty = ncord[1] ;

  for ( int jz = 0 ; jz < ndz ; ++jz ) {
    for ( int jy = 0 ; jy < ndy ; ++jy ) {
      for ( int jx = 0 ; jx < ndx ; ++jx ) {

        int zone = jx + ndx * (jy + ndy * jz ) ;

        for ( int i = 0 ; i<8 ; ++i ) { intsc_d[ 8*zone+i ] = zone ; }

        int tzone0 = jx + ntx * (jy + nty * jz) ;

        //  which target zones to intersect.
        intsc_t[ 8*zone     ] = tzone0 ;
        intsc_t[ 8*zone + 1 ] = tzone0 + 1 ;
        intsc_t[ 8*zone + 2 ] = tzone0 + ntx ;
        intsc_t[ 8*zone + 3 ] = tzone0 + ntx + 1 ;
        intsc_t[ 8*zone + 4 ] = tzone0 + ntx*nty ;
        intsc_t[ 8*zone + 5 ] = tzone0 + ntx*nty + 1 ;
        intsc_t[ 8*zone + 6 ] = tzone0 + ntx*nty + ntx ;
        intsc_t[ 8*zone + 7 ] = tzone0 + ntx*nty + ntx + 1 ;
      }}}
}


void INTSC_HEXRECT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_vid = vid ;

  //   Run a smaller problem in sequential because it's slow.
  long factor = 1L ;
  if ( ( vid == Base_Seq ) or ( vid == Lambda_Seq ) or ( vid == RAJA_Seq ) ) {
    factor = 8L ;
  }

  setActualProblemSize( getDefaultProblemSize() / factor );
  m_nrecords = 8 * getActualProblemSize() ;

  m_ndx = (Int_type)
      ( cbrt( (double) getActualProblemSize() + 0.5 ) ) ;
  m_ndy = m_ndx ;
  m_ndz = m_ndx ;
  int const ndx = m_ndx ;
  int const ndy = m_ndy ;
  int const ndz = m_ndz ;

  // scaled offsets for donor mesh
  m_x_scl_offs = 1 ;
  m_y_scl_offs = 2 ;
  m_z_scl_offs = 3 ;
  int const x_scl_offs = m_x_scl_offs ;
  int const y_scl_offs = m_y_scl_offs ;
  int const z_scl_offs = m_z_scl_offs ;

  double *planes[3] ;
  int ncord[3] ;

  m_sep = 0.1 ;           // target plane pitch (separation)
  m_x0 = -2.0 ;           // corner of target mesh
  m_y0 = -1.0 ;
  m_z0 = 1.0 ;
  double sep = m_sep ;

  //   corner of donor mesh
  auto corner_d =
      [=] ( double const c0, double const sep,
            int const scl_offs, int const ndc ) {

    double corner = c0 + sep * (1.0 - (double)scl_offs / (double)(ndc+1) ) ;
    return corner ;
  } ;

  m_xd0 = corner_d ( m_x0, sep, x_scl_offs, ndx ) ;
  m_yd0 = corner_d ( m_y0, sep, y_scl_offs, ndy ) ;
  m_zd0 = corner_d ( m_z0, sep, z_scl_offs, ndz ) ;

  //  donor zone coordinates (a simple Cartesian mesh).
  double *xdnode = new double [ 3*(ndx+1)*(ndy+1)*(ndz+1) ] ;
  double *ydnode = xdnode + (ndx+1)*(ndy+1)*(ndz+1) ;
  double *zdnode = ydnode + (ndx+1)*(ndy+1)*(ndz+1) ;

  // zone node list for the donor mesh
  int *znlist = new int [ 8*ndx*ndy*ndz ] ;

  setupDonorMesh
      ( sep, m_xd0, m_yd0, m_zd0, ndx, ndy, ndz,
        xdnode, ydnode, zdnode, znlist ) ;

  setupTargetPlanes
      ( planes, ncord,
        ndx, ndy, ndz,
        m_x0, m_y0, m_z0, sep ) ;

  // which zones to intersect.  Computed by hand for this test of
  // the geometry kernel.
  int *intsc_d = new int [ m_nrecords ] ;
  int *intsc_t = new int [ m_nrecords ] ;

  setupIntscPairs
      ( ncord, ndx, ndy, ndz, intsc_d, intsc_t ) ;

  double const **planes_c = const_cast<double const**>(planes) ;

  copyTargetToDevice ( planes_c, ncord ) ;

  int ndzones = ndx * ndy * ndz ;
  int ndnodes = (ndx+1) * (ndy+1) * (ndz+1) ;

  allocAndCopyHostData ( m_xdnode, xdnode, ndnodes, vid ) ;
  allocAndCopyHostData ( m_ydnode, ydnode, ndnodes, vid ) ;
  allocAndCopyHostData ( m_zdnode, zdnode, ndnodes, vid ) ;

  allocAndCopyHostData ( m_znlist, znlist, 8*ndzones, vid ) ;

  allocAndCopyHostData ( m_intsc_d, intsc_d, m_nrecords, vid ) ;
  allocAndCopyHostData ( m_intsc_t, intsc_t, m_nrecords, vid ) ;

  allocAndInitDataConst ( m_records, 4L*m_nrecords, 0.0, vid ) ;

  m_records_h = (Real_ptr) detail::allocHostData
        ( 4L*m_nrecords*sizeof(double), getDataAlignment() ) ;

  delete[] ( intsc_d ) ;
  delete[] ( intsc_t ) ;
  delete[] ( xdnode ) ;
  delete[] ( znlist ) ;
  delete[] ( planes[0] ) ;
}


//   Number of subzone intersections = 8 * number of standard intersections.
//
void INTSC_HEXRECT::checkMoments
    ( double *records,  // volumes, moments from GPU (rescaled here)
      int const n_intsc,   // number of intersections = 8*ndx*ndy*ndz
      int const ndx, int const ndy, int const ndz,  // donor zones each dir.
      double const xd0, double const yd0, double const zd0,  // donor corner
      double const x0, double const y0, double const z0,  // target corner
      double const sep,  //  target plane pitch
      double const sep1x, double const sep1y, double const sep1z )
{
  int rank = 0;
#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif // RAJA_PERFSUITE_ENABLE_MPI

  // Check on rank 0, other ranks are identical.
  if ( rank == 0 ) {
    double scale = 8.0 * (double)((ndx+1)*(ndy+1)*(ndz+1)) / (sep*sep*sep) ;

    //  Scale the volumes to produce integers.
    for ( int i = 0 ; i < 4*n_intsc ; ++i ) {
      records[i] *= scale ;
    }

    //  Compute centroids from the moments that were computed on the GPU.
    for ( int irec = 0 ; irec < n_intsc ; ++irec ) {
      double svv = records[4*irec] ;       // scaled volume
      if ( svv > 0.5 ) {       // prevent potential /0
        records[4*irec+1] /= svv ;   //  Compute centroids from moments.
        records[4*irec+2] /= svv ;
        records[4*irec+3] /= svv ;
      } else {
        records[4*irec+1] = 0.0 ;   //  undefined centroid
        records[4*irec+2] = 0.0 ;
        records[4*irec+3] = 0.0 ;
      }
    }

    double *zca = new double [ndz] ;
    double *zcb = new double [ndz] ;
    double *yca = new double [ndy] ;
    double *ycb = new double [ndy] ;
    double *xca = new double [ndx] ;
    double *xcb = new double [ndx] ;

    for ( int jz = 0 ; jz < ndz ; ++jz ) {
      double za = zd0 + jz * sep1z ;
      double zp = z0 + (jz+1)*sep ;
      zca[jz] = 0.5 * ( za + zp ) ;
      zcb[jz] = zca[jz] + 0.5 * sep1z ;
    }

    for ( int jy = 0 ; jy < ndy ; ++jy ) {
      double ya = yd0 + jy * sep1y ;
      double yp = y0 + (jy+1)*sep ;
      yca[jy] = 0.5 * ( ya + yp ) ;
      ycb[jy] = yca[jy] + 0.5 * sep1y ;
    }

    for ( int jx = 0 ; jx < ndx ; ++jx ) {
      double xa = xd0 + jx * sep1x ;
      double xp = x0 + (jx+1)*sep ;
      xca[jx] = 0.5 * ( xa + xp ) ;
      xcb[jx] = xca[jx] + 0.5 * sep1x ;
    }


    do {
      int rec0 = 0 ;
      double maxerr = 0.0 ;
      for ( int jz = 0 ; jz < ndz ; ++jz ) {
        for ( int jy = 0 ; jy < ndy ; ++jy ) {
          for ( int jx = 0 ; jx < ndx ; ++jx ) {

            for ( int irec = rec0 ; irec < rec0+8 ; ++irec ) {

              double z_cen =  ( ( irec & 4 ) != 0 ) ? zcb[jz] : zca[jz] ;
              double y_cen =  ( ( irec & 2 ) != 0 ) ? ycb[jy] : yca[jy] ;
              double x_cen =  ( ( irec & 1 ) != 0 ) ? xcb[jx] : xca[jx] ;

              double error ;
              error = fabs ( x_cen - records[4*irec+1] ) ;
              if ( error > maxerr) { maxerr = error ; }

              error = fabs ( y_cen - records[4*irec+2] ) ;
              if ( error > maxerr) { maxerr = error ; }

              error = fabs ( z_cen - records[4*irec+3] ) ;
              if ( error > maxerr) { maxerr = error ; }
            }
            rec0 += 8 ;
          }}}

      printf ( "Max centroid error = %16.8e\n", maxerr ) ;

    } while ( false ) ;

    delete[] ( xca ) ;
    delete[] ( xcb ) ;
    delete[] ( yca ) ;
    delete[] ( ycb ) ;
    delete[] ( zca ) ;
    delete[] ( zcb ) ;

  }
}


void INTSC_HEXRECT::checkScaledVolumes
    ( double const *records,  // volumes, moments on host (rescaled here)
      int const x_scl_offs, int const y_scl_offs,  // scaled offsets
      int const z_scl_offs,
      double const sep )  //  target plane pitch
{
  int rank = 0;
#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif // RAJA_PERFSUITE_ENABLE_MPI

  // Check on rank 0, other ranks are identical.
  if ( rank == 0 ) {
    int ndx = m_ndx ;
    int ndy = m_ndy ;
    int ndz = m_ndz ;

    double scale = 8.0 * (double)((ndx+1)*(ndy+1)*(ndz+1)) / (sep*sep*sep) ;

    int rec0 = 0 ;
    double maxerr = 0.0 ;
    for ( int jz = 0 ; jz < ndz ; ++jz ) {
      int zm = jz + 2*z_scl_offs ;
      int zp = 2*(ndz+1) - 1 - zm ;

      for ( int jy = 0 ; jy < ndy ; ++jy ) {
        int ym = jy + 2*y_scl_offs ;
        int yp = 2*(ndy+1) - 1 - ym ;

        for ( int jx = 0 ; jx < ndx ; ++jx ) {

          int xm = jx + 2*x_scl_offs ;
          int xp = 2*(ndx+1) - 1 - xm ;

          //  Check the intersections for this donor zone.
          for ( int krec = 0 ; krec < 8 ; ++krec ) {
            int vol = 1 ;

            // vol is the correct scaled volume, an integer.
            if ( (krec & 4) > 0 ) {
              vol *= zp ;
            } else {
              vol *= zm ;
            }
            if ( (krec & 2) > 0 ) {
              vol *= yp ;
            } else {
              vol *= ym ;
            }
            if ( (krec & 1) > 0 ) {
              vol *= xp ;
            } else {
              vol *= xm ;
            }
            int irec = rec0 + krec ;   // intersection record index
            double error ;
            error = fabs ( (double)vol - records[4*irec] ) ;
            if ( error > maxerr) { maxerr = error ; }
          }

          rec0 += 8 ;
        }}}
    maxerr /= scale ;   // Print the unscaled volume error.
    printf ( "Max volume error  = %16.8e\n", maxerr ) ;
  }
}


INTSC_HEXRECT::INTSC_HEXRECT(const RunParams& params)
  : KernelBase(rajaperf::Apps_INTSC_HEXRECT, params)
{
  //  Each donor zone intersects eight "target zones"
  //
  long side = 100 ;
  m_ndzones = side * side * side ;
  size_t n_intsc = 8L*m_ndzones ;   // number of intersections to compute
  m_ntzones = n_intsc ;          // one "target zone" per intersection
  setDefaultProblemSize(n_intsc);
  setDefaultReps(1);

  setItsPerRep( n_intsc );
  setKernelsPerRep(1);

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  setBytesReadPerRep( 48*sizeof(Real_type) * getItsPerRep() );
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr size_t flops_per_tri = 700 ;
  constexpr size_t flops_per_intsc = flops_per_tri * m_tri_per_intsc ;

  setFLOPsPerRep(n_intsc * flops_per_intsc);

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

INTSC_HEXRECT::~INTSC_HEXRECT()
{
}



void INTSC_HEXRECT::updateChecksum(VariantID vid, size_t tune_idx)
{
  // checksum[vid][tune_idx] += calcChecksum(m_sum, m_array_length, m_checksum_scale_factor, vid  );
  (void)(vid+0) ;
  (void)(tune_idx+0) ;
}

void INTSC_HEXRECT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  copyData ( DataSpace::Host, m_records_h,
             getDataSpace(vid), m_records, 4L*m_nrecords ) ;

  checkMoments
      ( m_records, m_nrecords,
        m_ndx, m_ndy, m_ndz, m_xd0, m_yd0, m_zd0,
        m_x0, m_y0, m_z0, m_sep, m_sep1x, m_sep1y, m_sep1z ) ;

  checkScaledVolumes
      ( m_records,
        m_x_scl_offs, m_y_scl_offs, m_z_scl_offs, m_sep ) ;

  detail::deallocHostData ( m_records_h ) ;
  deallocData ( m_records, vid ) ;
  deallocData ( m_intsc_t, vid ) ;
  deallocData ( m_intsc_d, vid ) ;
  deallocData ( m_znlist, vid ) ;
  deallocData ( m_xdnode, vid ) ;
  deallocData ( m_ydnode, vid ) ;
  deallocData ( m_zdnode, vid ) ;

}

} // end namespace apps
} // end namespace rajaperf
