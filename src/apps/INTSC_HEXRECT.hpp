//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_INTSC_HEXRECT_HPP
#define RAJAPerf_Apps_INTSC_HEXRECT_HPP


#include "RAJA/RAJA.hpp"

#include "common/RPTypes.hpp"


RAJA_HOST_DEVICE
RAJA_INLINE void intsc_hexrect_stub(
)
{
}




//   Clip a polygon returning polygon with active coordinate ain >= cut
//     Return number of vertices after clipping (might be zero).
//
RAJA_HOST_DEVICE
RAJA_INLINE int clip_polygon_ge
    ( double *ain,              // input active coordinates
      double *bin, double *cin,  // input passive coordinates
      bool const etob,   // Whether to clip from end to begin of ain
      double const cut,         // cut value of active coordinate
      int nv_in )       // number of sides in
{
  int const max_polygon_pts = 10 ;
  int const max = max_polygon_pts ;
  int j, jbeg, jend, jr, inc ;

  if ( etob ) {    // source at end of ain, destination at beginning
    jbeg = max - nv_in ;    jend = max ;
    j    = max - 1 ;         inc = 1 ;          jr = 0 ;
  } else {         // source at beginning of ain, destination at end
    jbeg = nv_in - 1 ;      jend = -1 ;
    j    = 0 ;               inc = -1 ;         jr = max - 1 ;
  }

  //  Ensure that if bin[j] == bin[jj] we get exactly bin[j]
  //   from interpolation.  This prevents roundoff fluctuations
  //   from being introduced from interpolation.
  //
  //  Also ensure clip_polygon_ge and _lt have positive den and
  //  eta in all four cases, ensuring consistency when _ge and _lt
  //  are called for the same source polygon (ain), so that the
  //  _ge and _lt output are correctly complementary to each other.
  //
  for ( int jj = jbeg ; jj != jend ; jj += inc ) {
    if ( ain[j] >= cut ) {
      ain[jr] = ain[j] ; bin[jr] = bin[j] ; cin[jr] = cin[j] ;
      jr += inc ;
      if ( ain[jj] < cut ) {
        double den =  ain[j] - ain[jj] ;
        double eta = ( cut - ain[jj] ) / den ;
        ain[jr] = cut ;
        bin[jr] = bin[jj] + ( bin[j] - bin[jj] ) * eta ;
        cin[jr] = cin[jj] + ( cin[j] - cin[jj] ) * eta ;
        jr += inc ;
      }
    } else if ( ain[jj] >= cut ) {
      double den = ain[jj] - ain[j] ;
      double eta = ( cut - ain[j] ) / den ;
      ain[jr] = cut ;
      bin[jr] = bin[j] + ( bin[jj] - bin[j] ) * eta ;
      cin[jr] = cin[j] + ( cin[jj] - cin[j] ) * eta ;
      jr += inc ;
    }
    j = jj ;
  }
  int ret = ( etob ) ? jr : ( max - 1 - jr ) ;
  return ret ;
}



//   Clip a polygon returning polygon with active coordinate ain < cut
//   etob is always false here (source is at beginning of ain,
//   destination is at end), hence removed etob parameter.
//     Return number of vertices after clipping (might be zero).
//
RAJA_HOST_DEVICE
RAJA_INLINE int clip_polygon_lt
    ( double *ain,              // input active coordinates
      double *bin, double *cin,  // input passive coordinates
      double const cut,         // cut value of active coordinate
      int nv_in )       // number of sides in
{
  int const max_polygon_pts = 10 ;

  //     See comments in clip_polygon_ge.
  int const max = max_polygon_pts ;
  int j, jbeg, jend, jr, inc ;

  // etob is false : source at beginning of ain, destination at end
  jbeg = nv_in - 1 ;      jend = -1 ;
  j    = 0 ;               inc = -1 ;         jr = max - 1 ;

  for ( int jj = jbeg ; jj != jend ; jj += inc ) {
    if ( ain[j] < cut ) {
      ain[jr] = ain[j] ; bin[jr] = bin[j] ; cin[jr] = cin[j] ;
      jr += inc ;
      if ( ain[jj] >= cut ) {
        double den =  ain[jj] - ain[j] ;
        double eta = ( cut - ain[j] ) / den ;
        ain[jr] = cut ;
        bin[jr] = bin[j] + ( bin[jj] - bin[j] ) * eta ;
        cin[jr] = cin[j] + ( cin[jj] - cin[j] ) * eta ;
        jr += inc ;
      }
    } else if ( ain[jj] < cut ) {
      double den = ain[j] - ain[jj] ;
      double eta = ( cut - ain[jj] ) / den ;
      ain[jr] = cut ;
      bin[jr] = bin[jj] + ( bin[j] - bin[jj] ) * eta ;
      cin[jr] = cin[jj] + ( cin[j] - cin[jj] ) * eta ;
      jr += inc ;
    }
    j = jj ;
  }
  int ret = max - 1 - jr ;   // ret = number of vertices after clipping
  return ret ;
}



//   Return the vertex count tally.
//
//   3D : metric factor is z values of donor polygon
//
RAJA_HOST_DEVICE
RAJA_INLINE int intsc24_shxf1
      ( double const dtx,    // target zone x length
        double const dty,    // target zone y length
        double const x0,     // target zone lower x coordinate
        double const y0,     // target zone lower y coordinate
        double const z0,     // target zone lower z coordinate, for moment
        double *qx,          // clipped donor polygon (circular order)
        int const shn,       // Number of vertices in donor polygon max 5
        double & sum0,       // output area or volume
        double & sumx,       // output x moment
        double & sumy,       // output y moment
        double & sumz )      // output z moment
{
  int const max_polygon_pts = 10 ;
  double const one3   = 0.33333333333333333 ;

  double const *qy = qx + max_polygon_pts ;
  double const *qz = qy + max_polygon_pts ;

  double xc0 = qx[0], xc1 = qx[1] ;
  double yc0 = qy[0], yc1 = qy[1] ;
  double zc0 = qz[0], zc1 = qz[1] ;
  for ( int kk = 2 ; kk < shn ; kk++ ) {

    double xc2 = qx[kk] ;
    double yc2 = qy[kk] ;
    double zc2 = qz[kk] ;

    double s0tri = 0.0, sxtri = 0.0, sytri = 0.0, sztri = 0.0 ;
    double metfac ;

    // Edge Midpoint quadrature each triangle
    //  For the z moment we have an additional 0.5 factor because
    //  z ranges from 0 to the plane of the polygon.
    metfac = 0.5 * ( zc0 + zc1 ) ;
    s0tri += metfac ;
    sxtri += metfac * ( x0 + 0.5 * dtx * ( xc0 + xc1 ) ) ;
    sytri += metfac * ( y0 + 0.5 * dty * ( yc0 + yc1 ) ) ;
    sztri += metfac * ( z0 + 0.5 * metfac ) ;

    metfac = 0.5 * ( zc1 + zc2 ) ;
    s0tri += metfac ;
    sxtri += metfac * ( x0 + 0.5 * dtx * ( xc1 + xc2 ) ) ;
    sytri += metfac * ( y0 + 0.5 * dty * ( yc1 + yc2 ) ) ;
    sztri += metfac * ( z0 + 0.5 * metfac ) ;

    metfac = 0.5 * ( zc2 + zc0 ) ;
    s0tri += metfac ;
    sxtri += metfac * ( x0 + 0.5 * dtx * ( xc2 + xc0 ) ) ;
    sytri += metfac * ( y0 + 0.5 * dty * ( yc2 + yc0 ) ) ;
    sztri += metfac * ( z0 + 0.5 * metfac ) ;

    //   area is positive for counterclockwise triangle (standard)
    double area = 0.5 *
        ( (xc0-xc1) * (yc0+yc1) + (xc1-xc2) * (yc1+yc2) +
          (xc2-xc0) * (yc2+yc0) ) ;

    sum0 += one3 * area * s0tri ;
    sumx += one3 * area * sxtri ;
    sumy += one3 * area * sytri ;
    sumz += one3 * area * sztri ;

    xc1 = xc2 ;         // advance to next edge of clipped polygon
    yc1 = yc2 ;
    zc1 = zc2 ;
  }
  return shn ;
}


//   Convert a 24 bit mask of predicates to a list where the bit is set.
//       One bit per facet of the triangle.
//
//   Compactifying mask to list might reduce branch divergence.
//
RAJA_HOST_DEVICE
RAJA_INLINE int intsc24_hex_mask_to_list
    ( int const mask,            // mask (used bits 0 to 23)
      unsigned char mylist[24] ) //  list of which mask bits are set
{
  int count=0 ;
  for ( int bit = 0 ; bit < 24 ; ++bit ) {
    if ( 0 != ( mask & (1<<bit) ) ) { mylist[count++] = bit ; }
  }
  return count ;
}



//   Get a triangle facet of the hexahedron,
//   transformed to frame of Cartesian target zone is 0<=(x,y)<=1 and
//   minimum z is 0.
//
RAJA_HOST_DEVICE
RAJA_INLINE void intsc24_hex_get_tri
    ( double const *xd,    // [8] donor x coordinates
      double const *yd,    // [8] donor y coordinates
      double const *zd,    // [8] donor z coordinates
      double const xt0,    // target zone lower x boundary
      double const yt0,    // target zone lower y boundary
      double const zt0,    // target zone lower z boundary
      double const a11,    // x multiplier
      double const a22,    // y multiplier
      int const f,         // which of six faces
      int const k0,        // which of four facets of face
      double xf[3],        // transformed facet x
      double yf[3],        // transformed facet y
      double zf[3] )       // transformed facet z
{
  int v0, v1, v2, v3 ;

  switch (f) {
  case 0:  v0 = 0 ; v1 = 2 ; v2 = 6 ; v3 = 4 ; break ;
  case 1:  v0 = 1 ; v1 = 5 ; v2 = 7 ; v3 = 3 ; break ;
  case 2:  v0 = 0 ; v1 = 4 ; v2 = 5 ; v3 = 1 ; break ;
  case 3:  v0 = 2 ; v1 = 3 ; v2 = 7 ; v3 = 6 ; break ;
  case 4:  v0 = 0 ; v1 = 1 ; v2 = 3 ; v3 = 2 ; break ;
  default: v0 = 4 ; v1 = 6 ; v2 = 7 ; v3 = 5 ; break ;
  }

  //   Face center is triangle point 1.
  //   Transformation of face center to the unit target frame 0<(x,y)<1.
  xf[1] = ( 0.25 * (xd[v0] + xd[v1] + xd[v2] + xd[v3]) - xt0 ) * a11 ;
  yf[1] = ( 0.25 * (yd[v0] + yd[v1] + yd[v2] + yd[v3]) - yt0 ) * a22 ;
  zf[1] =   0.25 * (zd[v0] + zd[v1] + zd[v2] + zd[v3]) - zt0 ;

  // triangle vertices
  int vv0, vv2 ;

  switch (k0) {
  case 0:   vv0 = v0 ;   vv2 = v1 ;  break ;
  case 1:   vv0 = v1 ;   vv2 = v2 ;  break ;
  case 2:   vv0 = v2 ;   vv2 = v3 ;  break ;
  default:  vv0 = v3 ;   vv2 = v0 ;  break ;
  }
  //   Coordinates of a triangle facet of the donor hexahedron.
  xf[0] = a11 * ( xd[vv0] - xt0 ) ;   xf[2] = a11 * ( xd[vv2] - xt0 ) ;
  yf[0] = a22 * ( yd[vv0] - yt0 ) ;   yf[2] = a22 * ( yd[vv2] - yt0 ) ;
  zf[0] =         zd[vv0] - zt0   ;   zf[2] =         zd[vv2] - zt0   ;
}




//   Categorize the triangles.
//      1) interior         0<=x,y<=1,  0<=z<=dzt
//      2) above z=dzt, and 0<=x,y<=1
//      3) needs clip
//      4) outside
RAJA_HOST_DEVICE
RAJA_INLINE int intsc24_hex_filter
    ( double const *xd,    // [8] donor x coordinates
      double const *yd,    // [8] donor y coordinates
      double const *zd,    // [8] donor z coordinates
      double const xt0,    // target zone lower x boundary
      double const xt1,    // target zone upper x boundary
      double const yt0,    // target zone lower y boundary
      double const yt1,    // target zone upper y boundary
      double const zt0,    // target zone lower z boundary
      double const zt1,    // target zone upper z boundary
      int &inside,
      int &abovez,
      int &clip )          // clip - 12 out of 24 triangles is common
{
  inside = abovez = clip = 0 ;   // initialize masks.

  //  target z spacing
  double const dzt = ( zt1 > zt0 ) ? ( zt1 - zt0 ) : 0.0 ;

  double a11, a22 ;    // Transformation to unit target frame 0<(x,y)<1
  do {

    double dtx = xt1 - xt0, dty = yt1 - yt0 ;
    double det  = dtx * dty ;    // area of the target zone (determinant)

    //   Transformation to the unit target frame 0<(x,y)<1.
    double deti = det / (det*det + 1.0e-80);

    a11 =  dty * deti;
    a22 =  dtx * deti;
  } while ( false ) ;

  int count = 0 ;
  for ( int f = 0 ; f < 6 ; ++f ) {   // six faces of the hexahedron

    for ( int k0 = 0 ; k0 < 4 ; ++k0 ) {   // four triangles of the face

      // transformed facet coordinates
      double xf[3], yf[3], zf[3] ;

      intsc24_hex_get_tri
          ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, xf, yf, zf ) ;

      //  Rule out facet if it is definitely outside a plane.
      if (not( ( ( xf[0] <  0.0 ) && ( xf[1] <  0.0 ) && ( xf[2] <  0.0 ) ) ||
               ( ( xf[0] >= 1.0 ) && ( xf[1] >= 1.0 ) && ( xf[2] >= 1.0 ) ) ||
               ( ( yf[0] <  0.0 ) && ( yf[1] <  0.0 ) && ( yf[2] <  0.0 ) ) ||
               ( ( yf[0] >= 1.0 ) && ( yf[1] >= 1.0 ) && ( yf[2] >= 1.0 ) ) ||
               ( ( zf[0] <  0.0 ) && ( zf[1] <  0.0 ) && ( zf[2] <  0.0 ) ) )) {

        int mask = 1 << count ;

        // test whether interior to x and y ranges
        if  (not(( xf[0] >= 0.0 ) && ( xf[1] >= 0.0 ) && ( xf[2] >= 0.0 ) &&
                 ( xf[0] <  1.0 ) && ( xf[1] <  1.0 ) && ( xf[2] <  1.0 ) &&
                 ( yf[0] >= 0.0 ) && ( yf[1] >= 0.0 ) && ( yf[2] >= 0.0 ) &&
                 ( yf[0] <  1.0 ) && ( yf[1] <  1.0 ) && ( yf[2] <  1.0 )) ) {

          clip |= mask ;   // Not interior, will run clip on this facet
        } else {

          //  Facet is interior to x and y ranges.

          if ( ( zf[0] >= dzt ) && ( zf[1] >= dzt ) && ( zf[2] >= dzt ) ) {

            abovez |= mask ;   // facet above dzt
          } else {

            if ( ( zf[0] >= 0.0 ) && ( zf[1] >= 0.0 ) && ( zf[2] >= 0.0 ) &&
                 ( zf[0] <  dzt ) && ( zf[1] <  dzt ) && ( zf[2] <  dzt ) ) {

              inside |= mask ;
            } else {
              clip |= mask ;    // Not interior, will run clip on facet.
            }
          }
        }
      }
      ++count ;
    }
  }
  return 0 ;
}





//  Map a hexahedral donor zone (24 triangular facets) onto
//  a Cartesian zone.
//
RAJA_HOST_DEVICE
RAJA_INLINE int intsc24_hex
      ( double *xd,    // [24] donor x coordinates, workspace
        double *qx_work,   // [3*max_polygon_pts] workspace for polygons
        double const xt0,    // target zone lower x boundary
        double const xt1,    // target zone upper x boundary
        double const yt0,    // target zone lower y boundary
        double const yt1,    // target zone upper y boundary
        double const zt0,    // target zone lower z boundary
        double const zt1,    // target zone upper z boundary
        double & sum0,       // output area or volume
        double & sumx,       // output x moment
        double & sumy,       // output y moment
        double & sumz )      // output z moment
{
  int const max_polygon_pts = 10 ;

  sum0 = sumx = sumy = sumz = 0.0 ;
  int vtxcnt = 0 ;

  //  Points oriented so that (v0, face center, v2) points out of positive zone
  //  static int const vface[24] =
  //  {0, 2, 6, 4, 1, 5, 7, 3, 0, 4, 5, 1, 2, 3, 7, 6, 0, 1, 3, 2, 4, 6, 7, 5} ;

  // polygon for overlay
  double *yd = xd + 8 ;
  double *zd = yd + 8 ;

  double *rx = qx_work ;
  double *ry = rx + max_polygon_pts ;
  double *rz = ry + max_polygon_pts ;

  //  target z spacing
  double const dzt = ( zt1 > zt0 ) ? ( zt1 - zt0 ) : 0.0 ;

  double a11, a22 ;    // Transformation to unit target frame 0<(x,y)<1
  do {

    double dtx = xt1 - xt0, dty = yt1 - yt0 ;
    double det  = dtx * dty ;    // area of the target zone (determinant)

    //   Transformation to the unit target frame 0<(x,y)<1.
    double deti = det / (det*det + 1.0e-80);

    a11 =  dty * deti;
    a22 =  dtx * deti;
  } while ( false ) ;

  int inside, abovez, clip ;   // which facets interior, above, or clipped.

  intsc24_hex_filter
      ( xd, yd, zd, xt0, xt1, yt0, yt1, zt0, zt1, inside, abovez, clip ) ;

  unsigned char facet_list[24] ;
  int nclip = intsc24_hex_mask_to_list ( clip, facet_list ) ;

  for ( int fi = 0 ; fi < nclip ; fi++ ) {    // facet index in facet_list

    int f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    int k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    //   Clip on y=1
    int shn1 = clip_polygon_lt
        ( ry, rx, rz, 1.0, 3 ) ;

    //   Clip on y=0, the x axis.
    int shn2 = clip_polygon_ge
        ( ry, rx, rz, true, 0.0, shn1 ) ;

    //   Clip on x=1
    int shn3 = clip_polygon_lt
        ( rx, ry, rz, 1.0, shn2 ) ;

    //   Clip on x=0, the y axis.
    int shn4 = clip_polygon_ge
        ( rx, ry, rz, true, 0.0, shn3 ) ;

    double sx[24] ;
    double *sy = sx + 8 ;
    double *sz = sy + 8 ;
    for ( int jj = 0 ; jj < shn4 ; ++jj ) {
      sx[jj] = rx[jj] ;   sy[jj] = ry[jj] ;  sz[jj] = rz[jj] ;
    }

    //  Upper polygon - clip above z=dzt
    int shn = clip_polygon_ge
        ( rz, rx, ry, false, dzt, shn4 ) ;

    if ( shn >= 3 ) {     //  There is an upper polygon

      for ( int jj = 0 ; jj < shn ; ++jj ) {
        rz[jj + max_polygon_pts - shn] = dzt ;   // project to upper face
      }

      double asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
      double dtx = xt1 - xt0, dty = yt1 - yt0 ;

      vtxcnt += intsc24_shxf1
          ( dtx, dty, xt0, yt0, zt0, rx + max_polygon_pts - shn, shn,
            asum0, asumx, asumy, asumz ) ;

      double det  = dtx * dty ;    // area of the target zone (determinant)

      sum0 += asum0 * det ;
      sumx += asumx * det ;
      sumy += asumy * det ;
      sumz += asumz * det ;
    }

    for ( int jj = 0 ; jj < shn4 ; ++jj ) {
      rx[jj] = sx[jj] ;   ry[jj] = sy[jj] ;  rz[jj] = sz[jj] ;
    }

    // Central polygon - clip below z=dzt
    shn1 = clip_polygon_lt
        ( rz, rx, ry, dzt, shn4 ) ;

    //  Central polygon - clip above z=0
    shn = clip_polygon_ge
        ( rz, rx, ry, true, 0.0, shn1 ) ;

    if ( shn >= 3 ) {     //  There is an intersecting polygon

      double asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;

      double dtx = xt1 - xt0, dty = yt1 - yt0 ;

      vtxcnt += intsc24_shxf1
          ( dtx, dty, xt0, yt0, zt0, rx, shn,
            asum0, asumx, asumy, asumz ) ;

      double det  = dtx * dty ;    // area of the target zone (determinant)

      sum0 += asum0 * det ;
      sumx += asumx * det ;
      sumy += asumy * det ;
      sumz += asumz * det ;
    }
  }

  // abovez (contribution from facet above the target zone).
  nclip = intsc24_hex_mask_to_list ( abovez, facet_list ) ;

  for ( int fi = 0 ; fi < nclip ; fi++ ) {    // facet index in facet_list

    int f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    int k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    rz[0] = rz[1] = rz[2] = dzt ;     // project Z to upper face.

    int shn = 3 ;

    double asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
    double dtx = xt1 - xt0, dty = yt1 - yt0 ;

    vtxcnt += intsc24_shxf1
        ( dtx, dty, xt0, yt0, zt0, rx, shn,
          asum0, asumx, asumy, asumz ) ;

    double det  = dtx * dty ;    // area of the target zone (determinant)

    sum0 += asum0 * det ;
    sumx += asumx * det ;
    sumy += asumy * det ;
    sumz += asumz * det ;
  }

  // inside (contribution from facets inside the target zone)
  nclip = intsc24_hex_mask_to_list ( inside, facet_list ) ;

  for ( int fi = 0 ; fi < nclip ; fi++ ) {    // facet index in facet_list

    int f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    int k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    int shn = 3 ;

    double asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
    double dtx = xt1 - xt0, dty = yt1 - yt0 ;

    vtxcnt += intsc24_shxf1
        ( dtx, dty, xt0, yt0, zt0, rx, shn,
          asum0, asumx, asumy, asumz ) ;

    double det  = dtx * dty ;    // area of the target zone (determinant)

    sum0 += asum0 * det ;
    sumx += asumx * det ;
    sumy += asumy * det ;
    sumz += asumz * det ;
  }

  return vtxcnt ;
}



#define  INTSC_HEXRECT_DATA_SETUP \
  char *ncord_gpu = m_ncord ; \
  double const *xdnode = (double const*) m_xdnode ; \
  double const *ydnode = (double const*) m_ydnode ; \
  double const *zdnode = (double const*) m_zdnode ; \
  int const *znlist = (int const*) m_znlist ; \
  int const *intsc_d = (int const*) m_intsc_d ; \
  int const *intsc_t = (int const*) m_intsc_t ; \
  long const nrecords = m_nrecords ; \
  double *records = (double *)m_records ;

#define INTSC_HEXRECT_BODY \
  if ( irec < nrecords ) { \
    double *record = ((double*)records) + 4 * irec ; \
    double xd[24] ; \
    double *yd = xd + 8 ; \
    double *zd = yd + 8 ; \
    do { \
      int dzone = intsc_d[irec] ; \
      for (int j=0 ; j<8 ; j++) { \
        int node = znlist[ 8*dzone + j ] ; \
        xd[j] = xdnode[node] ; \
        yd[j] = ydnode[node] ; \
        zd[j] = zdnode[node] ; } \
    } while ( false ) ; \
    do { \
      double sum0, sumx, sumy, sumz ; \
      double const *zplane ; \
      double const *yplane ; \
      double const *xplane ; \
      int jz, jy, jx ; \
      do { \
        int *ncord = (int*) ncord_gpu ; \
        double const **planes = ( double const** ) ( ncord + 4 ) ; \
        zplane = ( double const* ) ( planes + 3 ) ; \
        yplane = zplane + ncord[0] + 1 ; \
        xplane = yplane + ncord[1] + 1 ; \
        int const nyzones = ncord[1] ; \
        int const nxzones = ncord[2] ; \
        int tz = intsc_t[irec] ; \
        jz = tz / ( nxzones * nyzones ) ; \
        jy = ( tz / nxzones ) % nyzones ; \
        jx = tz % nxzones ; \
      } while ( false ) ; \
      intsc24_hex \
          ( xd, my_qx, \
            xplane[jx], xplane[jx+1], yplane[jy], yplane[jy+1], \
            zplane[jz], zplane[jz+1], \
            sum0, sumx, sumy, sumz ) ; \
      do { \
        double vf   = 1.0 ; \
        record[0] = vf * sum0 ; \
        record[1] = vf * sumx ; \
        record[2] = vf * sumy ; \
        record[3] = vf * sumz ; \
      } while ( false ) ; \
    } while ( false ) ; \
  }

#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace apps
{
class ADomain;

class INTSC_HEXRECT : public KernelBase
{
public:

  INTSC_HEXRECT(const RunParams& params);

  ~INTSC_HEXRECT();

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
  void setupTargetPlanes
      ( double **planes, int *ncord,
        int const ndx, int const ndy, int const ndz,
        double const x0, double const y0, double const z0,
        double const sep ) ;

  void setupDonorMesh
      ( double const sep,
        double const xd0, double const yd0, double const zd0,
        int const ndx, int const ndy, int const ndz,
        double *x, double *y, double *z,
        int *znlist ) ;

  void setupIntscPairs
      ( int const *ncord,
        int const ndx, int const ndy, int const ndz,
        int *intsc_d,
        int *intsc_t ) ;

  void copyTargetToDevice
      ( double const **planes,
        int const* ncord ) ;

  void checkMoments
      ( double *records, int const n_intsc,
        int const ndx, int const ndy, int const ndz,
        double const xd0, double const yd0, double const zd0,
        double const x0, double const y0, double const z0,
        double const sep,
        double const sep1x, double const sep1y, double const sep1z ) ;

  void checkScaledVolumes
      ( double const *records,
        int const x_scl_offs, int const y_scl_offs, int const z_scl_offs,
        double const sep ) ;

  void intscHexRectSeq        ( Index_type i, Index_type iend ) ;
  void intscHexRectOMP        ( Index_type i, Index_type iend ) ;
  void intscHexRectOMP_Target ( Index_type i, Index_type iend ) ;

  void check_intsc_volume_moments
      ( FILE* f, long const n_intsc, double const *vv ) ;

  static const size_t default_gpu_block_size = 64;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  static const size_t m_tri_per_intsc = 24 ;

  VariantID m_vid ;
  long m_gsize ;        // grid size (number of blocks)
  long m_nthreads ;     // total number of gpu threads (=64*m_gsize)

  size_t m_ndzones ;    // number of "donor zones"
  size_t m_ntzones ;    // number of "target zones"

  Real_ptr m_xdnode ;          // [ndnodes] x coordinates for donor
  Real_ptr m_ydnode ;          // [ndnodes] y coordinates for donor
  Real_ptr m_zdnode ;          // [ndnodes] z coordinates for donor
  Int_ptr m_znlist ;           // [donor zones][8] donor zone node list
  char *m_ncord ;              //  target dimensions and coordinates
  Int_ptr m_intsc_d ;          // [nrecords] donor zones to intersect
  Int_ptr m_intsc_t ;          // [nrecords] target zones to intersect
  long m_nrecords ;            // Number of threads (one thread per record)
  Real_ptr m_records ;         // output volumes, moments
  Real_ptr m_records_h ;       // volumes, moments on the host

  Real_type m_xd0, m_yd0, m_zd0 ;    // donor corner
  Real_type m_x0, m_y0, m_z0 ;       // corner of target mesh
  Real_type m_sep ;                  // target mesh pitch (separation)
  Real_type m_sep1x, m_sep1y, m_sep1z ;   // donor mesh zone widths
  Int_type  m_x_scl_offs, m_y_scl_offs, m_z_scl_offs ;  // donor scaled offsets
  Int_type  m_ndx, m_ndy, m_ndz ;     // donor mesh dimensions (are equal)

  FILE *m_f_hexrect ;  // output file

};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
