//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#include "AppsData.hpp"
#include "common/DataUtils.hpp"

#include <cmath>
#include <iomanip>


namespace rajaperf
{
namespace apps
{




INTSC_HEXRECT::INTSC_HEXRECT(const RunParams& params)
  : KernelBase(rajaperf::Apps_INTSC_HEXRECT, params)
{
  //  Each donor zone intersects eight "target zones"
  //
  //  Default problem size is 50 cubed "donor zones", one million intersections
  //   Problem size is specified as number of intersections, which is
  //     intsc_per_zone (=8) * number of donor zones.
  //   Number of donor zones is a cube number, "side" is the length
  //   of a side of the cube (cube root of number of donor zones)
  //
  constexpr Size_type a3_def = 50 ;
  Size_type n_intsc_def = intsc_per_zone * a3_def * a3_def * a3_def ;
  setDefaultProblemSize(n_intsc_def);
  setDefaultReps(1);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(3);

  setUsesFeature(Forall);

  addVariantTunings();
}

void INTSC_HEXRECT::setSize(Index_type target_size, Index_type target_reps)
{
  //  Command line --size specifies requested number of intersections.
  //  Requested number of intersections will be converted to an even cube
  //  number of intersections.
  //
  Index_type a3 = static_cast<Size_type>(std::cbrt(target_size + 0.5));

  // number of donor zones on a side of the cube
  Index_type side = a3 / 2 ;

  if ( side < 1L ) { side = 1L ; }

  m_ndzones = side * side * side;  // number of "donor zones" on a side
  Index_type n_intsc = intsc_per_zone*m_ndzones;   // number of intersections
  m_ntzones = n_intsc;             // one "target zone" per intersection

  // m_nrecords = number of intersections = 8 * number of donor zones
  m_nrecords = n_intsc ;

  m_ndx = static_cast<Int_type>(std::cbrt(m_ndzones + 0.5));
  m_ndy = m_ndx ;
  m_ndz = m_ndx ;

  // a simple Cartesian mesh
  m_ndnodes = static_cast<Index_type>(m_ndx+1) *
              static_cast<Index_type>(m_ndy+1) *
              static_cast<Index_type>(m_ndz+1) ;

  m_nplanes = static_cast<Index_type>(m_ndx+2) +
              static_cast<Index_type>(m_ndy+2) +
              static_cast<Index_type>(m_ndz+2) ;

  setActualProblemSize( n_intsc );
  setRunReps( target_reps );

  setItsPerRep( n_intsc );
  setKernelsPerRep(1);

  // Pack the sizes, pointers, and coordinates together.
  Size_type planes_size = sizeof(Plane) + m_nplanes * sizeof(Real_type);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * m_ndnodes + // xdnode
                           1*sizeof(Real_type) * m_ndnodes + // ydnode
                           1*sizeof(Real_type) * m_ndnodes + // zdnode
                           1*sizeof(Int_type) * 8*m_ndzones + // znlist
                           1*sizeof(Char_type) * planes_size + // ncord
                           1*sizeof(Int_type) * m_nrecords + // intsc_d
                           1*sizeof(Int_type) * m_nrecords + // intsc_t
                           1*sizeof(Real_type) * 4L*m_nrecords ); // records

  // touched data size, not actual number of stores and loads
  // see VOL3D.cpp
  //  Bytes Read : donor node coords (3 doubles), target plane
  //  coords (negligible); donor zone list (8 * m_ndzones) and
  //  intersection lists intsc_d and intsc_t each 1 integer per intersection.
  setBytesReadPerRep( 3*(side+1)*(side+1)*(side+1)*sizeof(Real_type) +
                      8*m_ndzones*sizeof(Int_type) +
                      2*sizeof(Int_type) * getItsPerRep() );

  // Bytes written : nvals_hexrect (=4) doubles for each intersection.
  setBytesWrittenPerRep( nvals_hexrect*sizeof(Real_type) * getItsPerRep() );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  constexpr Size_type flops_per_tri = 150 ;
  constexpr Size_type flops_per_intsc = flops_per_tri * tri_per_hex ;

  setFLOPsPerRep(n_intsc * flops_per_intsc);
}

INTSC_HEXRECT::~INTSC_HEXRECT()
{
}

//   Set up the Cartesian target mesh.  It is a series of Cartesian planes
// in each direction.
//
//  The target mesh is one more than the number of donor zones in
// each direction, so that each donor zone may intersect eight target zones.
//
void INTSC_HEXRECT::setupTargetPlanes(VariantID vid)
{
  // ncord is the number of target zones in each direction.
  Int_type ncord[3];
  ncord[0] = m_ndz + 1;
  ncord[1] = m_ndy + 1;
  ncord[2] = m_ndx + 1;

  // coordinate of first plane in each direction.
  Real_type coords0[3];
  coords0[0] = m_z0;
  coords0[1] = m_y0;
  coords0[2] = m_x0;

  // Pack the sizes, pointers, and coordinates together.
  Size_type planes_size = sizeof(Plane) + m_nplanes * sizeof(Real_type);

  auto a_nc = allocDataForInit(m_ncord, planes_size, vid);

  //  Build the buffer on the host in order to reduce the number
  //  of cudaMemcpy calls which are slow.
  Plane* plane_ptr = reinterpret_cast<Plane*>(m_ncord);
  Real_ptr setup_cord_ptr = reinterpret_cast<Real_ptr>(m_ncord + sizeof(Plane));
  Real_ptr final_cord_ptr = reinterpret_cast<Real_ptr>(a_nc.get_final_ptr() + sizeof(Plane));

  // Loop over directions.
  for ( Index_type dir = 0 ; dir < 3 ; ++dir ) {

    plane_ptr->ncord[dir] = ncord[dir] ;
    plane_ptr->planes[dir] = final_cord_ptr;

    // Set target mesh plane coordinates.
    for ( Index_type k = 0; k < ncord[dir]+1; ++k ) {
      setup_cord_ptr[k] = coords0[dir] + k*m_sep;
    }

    setup_cord_ptr += ncord[dir]+1;
    final_cord_ptr += ncord[dir]+1;
  }

}


//  Set up the donor mesh.  It is Cartesian for this test but it
//  is really an unstructured mesh for the geometry kernel.
//
//  Zones are slightly smaller than the target mesh zones, so that the
//  volumes of intersection vary.
//
void INTSC_HEXRECT::setupDonorMesh()
{
  //  slightly smaller zone widths for donor mesh.
  m_sep1x = m_sep * ( 1.0 - 0.5 / (Real_type)(m_ndx+1) ) ;
  m_sep1y = m_sep * ( 1.0 - 0.5 / (Real_type)(m_ndy+1) ) ;
  m_sep1z = m_sep * ( 1.0 - 0.5 / (Real_type)(m_ndz+1) ) ;

  for ( Index_type kz = 0 ; kz < m_ndz+1 ; ++kz ) {
    for ( Index_type ky = 0 ; ky < m_ndy+1 ; ++ky ) {
      for ( Index_type kx = 0 ; kx < m_ndx+1 ; ++kx ) {
        Int_type node = kx + (m_ndx+1) * (ky + (m_ndy+1) * kz)  ;

        m_xdnode[node] = m_xd0 + kx * m_sep1x ;
        m_ydnode[node] = m_yd0 + ky * m_sep1y ;
        m_zdnode[node] = m_zd0 + kz * m_sep1z ;

      }
    }
  }

  for ( Index_type jz = 0 ; jz < m_ndz ; ++jz ) {
    for ( Index_type jy = 0 ; jy < m_ndy ; ++jy ) {
      for ( Index_type jx = 0 ; jx < m_ndx ; ++jx ) {

        Int_type zone = jx + m_ndx * (jy + m_ndy * jz ) ;

        Int_type node0 = jx + (m_ndx+1) * (jy + (m_ndy+1) * jz ) ;

        m_znlist[ 8*zone     ] = node0 ;
        m_znlist[ 8*zone + 1 ] = node0 + 1 ;
        m_znlist[ 8*zone + 2 ] = node0 + (m_ndx+1) ;
        m_znlist[ 8*zone + 3 ] = node0 + (m_ndx+1) + 1 ;
        m_znlist[ 8*zone + 4 ] = node0 + (m_ndx+1)*(m_ndy+1) ;
        m_znlist[ 8*zone + 5 ] = node0 + (m_ndx+1)*(m_ndy+1) + 1 ;
        m_znlist[ 8*zone + 6 ] = node0 + (m_ndx+1)*(m_ndy+1) + (m_ndx+1) ;
        m_znlist[ 8*zone + 7 ] = node0 + (m_ndx+1)*(m_ndy+1) + (m_ndx+1) + 1 ;
      }
    }
  }
}


//   Determine which pairs of donor and target zones intersect.
//
//   Normally this requires a filtering algorithm but for this
//   test we can calculate it from the alignment of the donor and
//   target meshes.
//
void INTSC_HEXRECT::setupIntscPairs()
{
  Int_type ntx = m_ndx + 1; // number of target zones each direction
  Int_type nty = m_ndy + 1;

  for ( Index_type jz = 0 ; jz < m_ndz ; ++jz ) {
    for ( Index_type jy = 0 ; jy < m_ndy ; ++jy ) {
      for ( Index_type jx = 0 ; jx < m_ndx ; ++jx ) {

        Int_type zone = jx + m_ndx * (jy + m_ndy * jz ) ;

        for ( Index_type i = 0 ; i < 8 ; ++i ) {
          m_intsc_d[ 8*zone+i ] = zone ;
        }

        Int_type tzone0 = jx + ntx * (jy + nty * jz) ;

        //  which target zones to intersect.
        m_intsc_t[ 8*zone     ] = tzone0 ;
        m_intsc_t[ 8*zone + 1 ] = tzone0 + 1 ;
        m_intsc_t[ 8*zone + 2 ] = tzone0 + ntx ;
        m_intsc_t[ 8*zone + 3 ] = tzone0 + ntx + 1 ;
        m_intsc_t[ 8*zone + 4 ] = tzone0 + ntx*nty ;
        m_intsc_t[ 8*zone + 5 ] = tzone0 + ntx*nty + 1 ;
        m_intsc_t[ 8*zone + 6 ] = tzone0 + ntx*nty + ntx ;
        m_intsc_t[ 8*zone + 7 ] = tzone0 + ntx*nty + ntx + 1 ;
      }
    }
  }
}


void INTSC_HEXRECT::setUp(VariantID vid,
                          Size_type RAJAPERF_UNUSED_ARG(tune_idx))
{
  // scaled offsets for donor mesh
  m_x_scl_offs = 1 ;
  m_y_scl_offs = 2 ;
  m_z_scl_offs = 3 ;

  m_sep = 0.1 ;           // target plane pitch (separation)
  m_x0 = -2.0 ;           // corner of target mesh
  m_y0 = -1.0 ;
  m_z0 = 1.0 ;

  //   corner of donor mesh
  auto corner_d = [=](Real_type const c0, Real_type const sep,
                      Int_type const scl_offs, Int_type const ndc) {

    return c0 + sep * (static_cast<Real_type>(1.0) - static_cast<Real_type>(scl_offs) / static_cast<Real_type>(ndc+1));
  };

  m_xd0 = corner_d(m_x0, m_sep, m_x_scl_offs, m_ndx);
  m_yd0 = corner_d(m_y0, m_sep, m_y_scl_offs, m_ndy);
  m_zd0 = corner_d(m_z0, m_sep, m_z_scl_offs, m_ndz);

  //  donor zone coordinates (a simple Cartesian mesh).
  auto a_xd = allocDataForInit(m_xdnode, m_ndnodes, vid);
  auto a_yd = allocDataForInit(m_ydnode, m_ndnodes, vid);
  auto a_zd = allocDataForInit(m_zdnode, m_ndnodes, vid);

  // zone node list for the donor mesh
  auto a_zl = allocDataForInit(m_znlist, 8*m_ndzones, vid);

  setupDonorMesh();

  setupTargetPlanes(vid);

  // which zones to intersect.  Computed by hand for this test of
  // the geometry kernel.
  auto a_id = allocDataForInit(m_intsc_d, m_nrecords, vid);
  auto a_it = allocDataForInit(m_intsc_t, m_nrecords, vid);

  setupIntscPairs();

  allocAndInitDataConst( m_records, 4L*m_nrecords, 0.0, vid ) ;

  //  Output records copied to the host.
  allocData( DataSpace::Host, m_records_h, 4L*m_nrecords ) ;
}


//   Number of subzone intersections = 8 * number of standard intersections.
//
void INTSC_HEXRECT::checkMoments(VariantID vid) // Print variant name in case of error.
{
  Real_ptr records = m_records_h; // volumes, moments from GPU (rescaled here)
  Int_type const n_intsc = m_nrecords; // number of intersections = 8*m_ndx*m_ndy*m_ndz

  {
    Real_type scale = 8.0 * (Real_type)((m_ndx+1)*(m_ndy+1)*(m_ndz+1)) / (m_sep*m_sep*m_sep) ;

    //  Scale the volumes to produce integers.
    for ( Index_type i = 0 ; i < 4*n_intsc ; ++i ) {
      records[i] *= scale ;
    }

    //  Compute centroids from the moments that were computed on the GPU.
    for ( Index_type irec = 0 ; irec < n_intsc ; ++irec ) {
      Real_type svv = records[4*irec] ;       // scaled volume
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

    Real_ptr zca, zcb, yca, ycb, xca, xcb ;
    allocData  ( DataSpace::Host, zca, m_ndz ) ;
    allocData  ( DataSpace::Host, zcb, m_ndz ) ;
    allocData  ( DataSpace::Host, yca, m_ndy ) ;
    allocData  ( DataSpace::Host, ycb, m_ndy ) ;
    allocData  ( DataSpace::Host, xca, m_ndx ) ;
    allocData  ( DataSpace::Host, xcb, m_ndx ) ;

    for ( Index_type jz = 0 ; jz < m_ndz ; ++jz ) {
      Real_type za = m_zd0 + jz * m_sep1z ;
      Real_type zp = m_z0 + (jz+1)*m_sep ;
      zca[jz] = 0.5 * ( za + zp ) ;
      zcb[jz] = zca[jz] + 0.5 * m_sep1z ;
    }

    for ( Index_type jy = 0 ; jy < m_ndy ; ++jy ) {
      Real_type ya = m_yd0 + jy * m_sep1y ;
      Real_type yp = m_y0 + (jy+1)*m_sep ;
      yca[jy] = 0.5 * ( ya + yp ) ;
      ycb[jy] = yca[jy] + 0.5 * m_sep1y ;
    }

    for ( Index_type jx = 0 ; jx < m_ndx ; ++jx ) {
      Real_type xa = m_xd0 + jx * m_sep1x ;
      Real_type xp = m_x0 + (jx+1)*m_sep ;
      xca[jx] = 0.5 * ( xa + xp ) ;
      xcb[jx] = xca[jx] + 0.5 * m_sep1x ;
    }


    {
      Int_type rec0 = 0 ;
      Real_type maxerr = 0.0 ;

      Int_type loc_jx=0, loc_jy=0, loc_jz=0 ;
      Int_type loc_irec=0 ;
      Real_type expect_xc=0.0, expect_yc=0.0, expect_zc=0.0 ;
      Real_type calc_xc=0.0, calc_yc=0.0, calc_zc=0.0 ;

      for ( Index_type jz = 0 ; jz < m_ndz ; ++jz ) {
        for ( Index_type jy = 0 ; jy < m_ndy ; ++jy ) {
          for ( Index_type jx = 0 ; jx < m_ndx ; ++jx ) {

            for ( Index_type irec = rec0 ; irec < rec0+8 ; ++irec ) {

              Bool_type is_max=false ;

              Real_type z_cen =  ( ( irec & 4 ) != 0 ) ? zcb[jz] : zca[jz] ;
              Real_type y_cen =  ( ( irec & 2 ) != 0 ) ? ycb[jy] : yca[jy] ;
              Real_type x_cen =  ( ( irec & 1 ) != 0 ) ? xcb[jx] : xca[jx] ;

              Real_type error ;
              error = fabs ( x_cen - records[4*irec+1] ) ;
              if ( error > maxerr) { maxerr = error ; is_max = true ; }

              error = fabs ( y_cen - records[4*irec+2] ) ;
              if ( error > maxerr) { maxerr = error ; is_max = true ; }

              error = fabs ( z_cen - records[4*irec+3] ) ;
              if ( error > maxerr) { maxerr = error ; is_max = true ; }

              if ( is_max ) {    // found new maximum error
                loc_jx = jx ;
                loc_jy = jy ;
                loc_jz = jz ;
                loc_irec = irec ;
                expect_xc = x_cen ;
                expect_yc = y_cen ;
                expect_zc = z_cen ;
                calc_xc = records[4*irec+1] ;
                calc_yc = records[4*irec+2] ;
                calc_zc = records[4*irec+3] ;
              }
            }

            rec0 += 8 ;
          }}}

      Real_type const tol = 1.0e-12 * m_sep * (Real_type)m_ndx ;
      if ( maxerr > tol ) {
        Char_const_ptr tst = "INTSC_HEXRECT:" ;

        getCout()
            << tst
            << " Centroid error exceeds tolerance for "
            << getVariantName(vid) << "."
            << std::endl
            << tst
            << " Maximum error at (x,y,z)=("
            << std::setw(3) << loc_jx << ","
            << std::setw(3) << loc_jy << ","
            << std::setw(3) << loc_jz << ")"
            << " irec=" << loc_irec
            << "  tolerance =  "
            << std::scientific << std::setprecision(2) << std::setw(8) << tol
            << std::endl
            << tst
            << " Maximum error = "
            << std::setprecision(8) << std::setw(16) << maxerr
            << std::endl
            << tst
            << " Computed ("
            << std::setprecision(15)
            << std::setw(23) << calc_xc << ","
            << std::setw(23) << calc_yc << ","
            << std::setw(23) << calc_zc << ")"
            << std::endl
            << tst
            << " Expected ("
            << std::setw(23) << expect_xc << ","
            << std::setw(23) << expect_yc << ","
            << std::setw(23) << expect_zc << ")"
            << std::endl ;
      }
    }

    deallocData ( DataSpace::Host, xca ) ;
    deallocData ( DataSpace::Host, xcb ) ;
    deallocData ( DataSpace::Host, yca ) ;
    deallocData ( DataSpace::Host, ycb ) ;
    deallocData ( DataSpace::Host, zca ) ;
    deallocData ( DataSpace::Host, zcb ) ;
  }
}

void INTSC_HEXRECT::checkScaledVolumes(VariantID vid) // Print variant name in case of error.
{
  Real_const_ptr records = m_records_h; // volumes, moments on host (rescaled here)

  {
    Real_type scale =
        8.0 * (Real_type)((m_ndx+1)*(m_ndy+1)*(m_ndz+1)) / (m_sep*m_sep*m_sep) ;

    Int_type loc_jx=0, loc_jy=0, loc_jz=0 ;
    Int_type loc_krec=0 ;
    Real_type expect_v=0.0 ;
    Real_type calc_v=0.0 ;

    Int_type rec0 = 0 ;
    Real_type maxerr = 0.0 ;
    for ( Index_type jz = 0 ; jz < m_ndz ; ++jz ) {
      Int_type zm = jz + 2*m_z_scl_offs ;
      Int_type zp = 2*(m_ndz+1) - 1 - zm ;

      for ( Index_type jy = 0 ; jy < m_ndy ; ++jy ) {
        Int_type ym = jy + 2*m_y_scl_offs ;
        Int_type yp = 2*(m_ndy+1) - 1 - ym ;

        for ( Index_type jx = 0 ; jx < m_ndx ; ++jx ) {

          Int_type xm = jx + 2*m_x_scl_offs ;
          Int_type xp = 2*(m_ndx+1) - 1 - xm ;

          //  Check the intersections for this donor zone.
          for ( Index_type krec = 0 ; krec < 8 ; ++krec ) {
            Int_type vol = 1 ;

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
            Int_type irec = rec0 + krec ;   // intersection record index
            Real_type error ;
            error = fabs ( (Real_type)vol - records[4*irec] ) ;

            if ( error > maxerr) { // found new maximum error
              maxerr = error ;
              loc_jx = jx ;
              loc_jy = jy ;
              loc_jz = jz ;
              loc_krec = krec ;
              expect_v = vol ;
              calc_v = records[4*irec] ;
            }
          }

          rec0 += 8 ;
        }}}
    maxerr /= scale ;   // Compare the unscaled volume error.

    Real_type const tol = 1.0e-12 * m_sep*m_sep*m_sep ;
    if ( maxerr > tol ) {
      Char_const_ptr tst = "INTSC_HEXRECT:" ;

      getCout()
          << tst
          << " Volume error exceeds tolerance for "
          << getVariantName(vid).c_str() << "."
          << std::endl
          << tst
          << " Maximum error at (x,y,z)=("
          << std::setw(3) << loc_jx << ","
          << std::setw(3) << loc_jy << ","
          << std::setw(3) << loc_jz << ")"
          << " krec=" << loc_krec
          << std::scientific
          << "  tolerance =" << std::setprecision(2) << std::setw(10) << tol
          << std::endl
          << tst
          << " Maximum error ="
          << std::setprecision(8) << std::setw(17) << maxerr
          << std::endl
          << tst
          << " Computed "
          << std::setprecision(15)
          << std::setw(23) << calc_v / scale
          << std::endl
          << tst
          << " Expected "
          << std::setw(23) << expect_v / scale
          << std::endl ;
    }
  }
}



void INTSC_HEXRECT::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  copyData(DataSpace::Host, m_records_h,
           getDataSpace(vid), m_records, 4L*m_nrecords) ;

  checkMoments(vid);

  checkScaledVolumes(vid);

  addToChecksum(DataSpace::Host, m_records_h, 4L*m_nrecords);
}

void INTSC_HEXRECT::tearDown(VariantID vid,
                             Size_type RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_records, vid);
  deallocData(m_intsc_t, vid);
  deallocData(m_intsc_d, vid);
  deallocData(m_znlist, vid);
  deallocData(m_xdnode, vid);
  deallocData(m_ydnode, vid);
  deallocData(m_zdnode, vid);
  deallocData(DataSpace::Host, m_records_h);
}

} // end namespace apps
} // end namespace rajaperf
