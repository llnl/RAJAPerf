//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_INTSC_HEXRECT_BODY_HPP
#define RAJAPerf_Apps_INTSC_HEXRECT_BODY_HPP

namespace rajaperf {

// Helper functions for the INTSC_HEXRECT kernel

//   Clip a polygon returning polygon with active coordinate ain >= cut
//     Return number of vertices after clipping (might be zero).
//
RAJA_HOST_DEVICE
RAJA_INLINE Int_type clip_polygon_ge
    ( Real_ptr ain,              // input active coordinates
      Real_ptr bin, Real_ptr cin,  // input passive coordinates
      Bool_type const etob,   // Whether to clip from end to begin of ain
      Real_type const cut,         // cut value of active coordinate
      Int_type nv_in )       // number of sides in
{
  Int_type const max = max_polygon_pts ;   // max_polygon_pts = 10
  Int_type j, jbeg, jend, jr, inc ;

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
  for ( Index_type jj = jbeg ; jj != jend ; jj += inc ) {
    if ( ain[j] >= cut ) {
      ain[jr] = ain[j] ; bin[jr] = bin[j] ; cin[jr] = cin[j] ;
      jr += inc ;
      if ( ain[jj] < cut ) {
        Real_type den =  ain[j] - ain[jj] ;
        Real_type eta = ( cut - ain[jj] ) / den ;
        ain[jr] = cut ;
        bin[jr] = bin[jj] + ( bin[j] - bin[jj] ) * eta ;
        cin[jr] = cin[jj] + ( cin[j] - cin[jj] ) * eta ;
        jr += inc ;
      }
    } else if ( ain[jj] >= cut ) {
      Real_type den = ain[jj] - ain[j] ;
      Real_type eta = ( cut - ain[j] ) / den ;
      ain[jr] = cut ;
      bin[jr] = bin[j] + ( bin[jj] - bin[j] ) * eta ;
      cin[jr] = cin[j] + ( cin[jj] - cin[j] ) * eta ;
      jr += inc ;
    }
    j = jj ;
  }
  Int_type ret = ( etob ) ? jr : ( max - 1 - jr ) ;
  return ret ;
}



//   Clip a polygon returning polygon with active coordinate ain < cut
//   etob is always false here (source is at beginning of ain,
//   destination is at end), hence removed etob parameter.
//     Return number of vertices after clipping (might be zero).
//
RAJA_HOST_DEVICE
RAJA_INLINE Int_type clip_polygon_lt
    ( Real_ptr ain,              // input active coordinates
      Real_ptr bin, Real_ptr cin,  // input passive coordinates
      Real_type const cut,         // cut value of active coordinate
      Int_type nv_in )       // number of sides in
{
  //     See comments in clip_polygon_ge.
  Int_type const max = max_polygon_pts ;   // max_polygon_pts = 10
  Int_type j, jbeg, jend, jr, inc ;

  // etob is false : source at beginning of ain, destination at end
  jbeg = nv_in - 1 ;      jend = -1 ;
  j    = 0 ;               inc = -1 ;         jr = max - 1 ;

  for ( Index_type jj = jbeg ; jj != jend ; jj += inc ) {
    if ( ain[j] < cut ) {
      ain[jr] = ain[j] ; bin[jr] = bin[j] ; cin[jr] = cin[j] ;
      jr += inc ;
      if ( ain[jj] >= cut ) {
        Real_type den =  ain[jj] - ain[j] ;
        Real_type eta = ( cut - ain[j] ) / den ;
        ain[jr] = cut ;
        bin[jr] = bin[j] + ( bin[jj] - bin[j] ) * eta ;
        cin[jr] = cin[j] + ( cin[jj] - cin[j] ) * eta ;
        jr += inc ;
      }
    } else if ( ain[jj] < cut ) {
      Real_type den = ain[j] - ain[jj] ;
      Real_type eta = ( cut - ain[jj] ) / den ;
      ain[jr] = cut ;
      bin[jr] = bin[jj] + ( bin[j] - bin[jj] ) * eta ;
      cin[jr] = cin[jj] + ( cin[j] - cin[jj] ) * eta ;
      jr += inc ;
    }
    j = jj ;
  }
  Int_type ret = max - 1 - jr ;   // ret = number of vertices after clipping
  return ret ;
}



//   Return the vertex count tally.
//
//   3D : metric factor is z values of donor polygon
//
RAJA_HOST_DEVICE
RAJA_INLINE Int_type intsc24_shxf1
      ( Real_type const dtx,    // target zone x length
        Real_type const dty,    // target zone y length
        Real_type const x0,     // target zone lower x coordinate
        Real_type const y0,     // target zone lower y coordinate
        Real_type const z0,     // target zone lower z coordinate, for moment
        Real_ptr qx,          // clipped donor polygon (circular order)
        Int_type const shn,       // Number of vertices in donor polygon max 5
        Real_type & sum0,       // output area or volume
        Real_type & sumx,       // output x moment
        Real_type & sumy,       // output y moment
        Real_type & sumz )      // output z moment
{
  Real_type const one3   = 0.33333333333333333 ;

  Real_const_ptr qy = qx + max_polygon_pts ;  //  max_polygon_pts is 10
  Real_const_ptr qz = qy + max_polygon_pts ;

  Real_type xc0 = qx[0], xc1 = qx[1] ;
  Real_type yc0 = qy[0], yc1 = qy[1] ;
  Real_type zc0 = qz[0], zc1 = qz[1] ;
  for ( Index_type kk = 2 ; kk < shn ; kk++ ) {

    Real_type xc2 = qx[kk] ;
    Real_type yc2 = qy[kk] ;
    Real_type zc2 = qz[kk] ;

    Real_type s0tri = 0.0, sxtri = 0.0, sytri = 0.0, sztri = 0.0 ;
    Real_type metfac ;

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
    Real_type area = 0.5 *
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
RAJA_INLINE Int_type intsc24_hex_mask_to_list
    ( Int_type const mask,            // mask (used bits 0 to 23)
      Uchar_type (&mylist)[24] ) //  list of which mask bits are set
{
  Int_type count=0 ;
  for ( Index_type bit = 0 ; bit < 24 ; ++bit ) {
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
    ( Real_const_ptr xd,    // [8] donor x coordinates
      Real_const_ptr yd,    // [8] donor y coordinates
      Real_const_ptr zd,    // [8] donor z coordinates
      Real_type const xt0,    // target zone lower x boundary
      Real_type const yt0,    // target zone lower y boundary
      Real_type const zt0,    // target zone lower z boundary
      Real_type const a11,    // x multiplier
      Real_type const a22,    // y multiplier
      Int_type const f,         // which of six faces
      Int_type const k0,        // which of four facets of face
      Real_ptr xf,            // transformed facet x
      Real_ptr yf,            // transformed facet y
      Real_ptr zf )           // transformed facet z
{
  Int_type v0, v1, v2, v3 ;

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
  Int_type vv0, vv2 ;

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
RAJA_INLINE Int_type intsc24_hex_filter
    ( Real_const_ptr xd,    // [8] donor x coordinates
      Real_const_ptr yd,    // [8] donor y coordinates
      Real_const_ptr zd,    // [8] donor z coordinates
      Real_type const xt0,    // target zone lower x boundary
      Real_type const xt1,    // target zone upper x boundary
      Real_type const yt0,    // target zone lower y boundary
      Real_type const yt1,    // target zone upper y boundary
      Real_type const zt0,    // target zone lower z boundary
      Real_type const zt1,    // target zone upper z boundary
      Int_type &inside,
      Int_type &abovez,
      Int_type &clip )          // clip - 12 out of 24 triangles is common
{
  inside = abovez = clip = 0 ;   // initialize masks.

  //  target z spacing
  Real_type const dzt = ( zt1 > zt0 ) ? ( zt1 - zt0 ) : 0.0 ;

  Real_type a11, a22 ;    // Transformation to unit target frame 0<(x,y)<1
  {

    Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;
    Real_type det  = dtx * dty ;    // area of the target zone (determinant)

    //   Transformation to the unit target frame 0<(x,y)<1.
    Real_type deti = det / (det*det + 1.0e-80);

    a11 =  dty * deti;
    a22 =  dtx * deti;
  }

  Int_type count = 0 ;
  for ( Index_type f = 0 ; f < 6 ; ++f ) {   // six faces of the hexahedron

    for ( Index_type k0 = 0 ; k0 < 4 ; ++k0 ) {   // four triangles of the face

      // transformed facet coordinates
      Real_type xf[3], yf[3], zf[3] ;

      intsc24_hex_get_tri
          ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, xf, yf, zf ) ;

      //  Rule out facet if it is definitely outside a plane.
      if ( ! ( ( ( xf[0] <  0.0 ) && ( xf[1] <  0.0 ) && ( xf[2] <  0.0 ) ) ||
               ( ( xf[0] >= 1.0 ) && ( xf[1] >= 1.0 ) && ( xf[2] >= 1.0 ) ) ||
               ( ( yf[0] <  0.0 ) && ( yf[1] <  0.0 ) && ( yf[2] <  0.0 ) ) ||
               ( ( yf[0] >= 1.0 ) && ( yf[1] >= 1.0 ) && ( yf[2] >= 1.0 ) ) ||
               ( ( zf[0] <  0.0 ) && ( zf[1] <  0.0 ) && ( zf[2] <  0.0 ) ) )) {

        Int_type mask = 1 << count ;

        // test whether interior to x and y ranges
        if  ( ! (( xf[0] >= 0.0 ) && ( xf[1] >= 0.0 ) && ( xf[2] >= 0.0 ) &&
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
//
RAJA_HOST_DEVICE
RAJA_INLINE Int_type intsc24_hex
      ( Real_ptr xd,    // [24] donor x coordinates, workspace
        Real_ptr qx_work,   // [3*max_polygon_pts] workspace for polygons
        Real_type const xt0,    // target zone lower x boundary
        Real_type const xt1,    // target zone upper x boundary
        Real_type const yt0,    // target zone lower y boundary
        Real_type const yt1,    // target zone upper y boundary
        Real_type const zt0,    // target zone lower z boundary
        Real_type const zt1,    // target zone upper z boundary
        Real_type & sum0,       // output area or volume
        Real_type & sumx,       // output x moment
        Real_type & sumy,       // output y moment
        Real_type & sumz )      // output z moment
{
  sum0 = sumx = sumy = sumz = 0.0 ;
  Int_type vtxcnt = 0 ;

  //  Points oriented so that (v0, face center, v2) points out of positive zone
  //  static Int_type const vface[24] =
  //  {0, 2, 6, 4, 1, 5, 7, 3, 0, 4, 5, 1, 2, 3, 7, 6, 0, 1, 3, 2, 4, 6, 7, 5} ;

  // polygon for overlay
  Real_ptr yd = xd + 8 ;
  Real_ptr zd = yd + 8 ;

  Real_ptr rx = qx_work ;
  Real_ptr ry = rx + max_polygon_pts ;    // max_polygon_pts is 10
  Real_ptr rz = ry + max_polygon_pts ;

  //  target z spacing
  Real_type const dzt = ( zt1 > zt0 ) ? ( zt1 - zt0 ) : 0.0 ;

  Real_type a11, a22 ;    // Transformation to unit target frame 0<(x,y)<1
  {

    Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;
    Real_type det  = dtx * dty ;    // area of the target zone (determinant)

    //   Transformation to the unit target frame 0<(x,y)<1.
    Real_type deti = det / (det*det + 1.0e-80);

    a11 =  dty * deti;
    a22 =  dtx * deti;
  }

  Int_type inside, abovez, clip ;   // which facets interior, above, or clipped.

  intsc24_hex_filter
      ( xd, yd, zd, xt0, xt1, yt0, yt1, zt0, zt1, inside, abovez, clip ) ;

  Uchar_type facet_list[24] ;
  Int_type nclip = intsc24_hex_mask_to_list ( clip, facet_list ) ;

  for ( Index_type fi = 0 ; fi < nclip ; fi++ ) {  // facet index in facet_list

    Int_type f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    Int_type k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    //   Clip on y=1
    Int_type shn1 = clip_polygon_lt
        ( ry, rx, rz, 1.0, 3 ) ;

    //   Clip on y=0, the x axis.
    Int_type shn2 = clip_polygon_ge
        ( ry, rx, rz, true, 0.0, shn1 ) ;

    //   Clip on x=1
    Int_type shn3 = clip_polygon_lt
        ( rx, ry, rz, 1.0, shn2 ) ;

    //   Clip on x=0, the y axis.
    Int_type shn4 = clip_polygon_ge
        ( rx, ry, rz, true, 0.0, shn3 ) ;

    Real_type sx[24] ;
    Real_ptr sy = sx + 8 ;
    Real_ptr sz = sy + 8 ;
    for ( Index_type jj = 0 ; jj < shn4 ; ++jj ) {
      sx[jj] = rx[jj] ;   sy[jj] = ry[jj] ;  sz[jj] = rz[jj] ;
    }

    //  Upper polygon - clip above z=dzt
    Int_type shn = clip_polygon_ge
        ( rz, rx, ry, false, dzt, shn4 ) ;

    if ( shn >= 3 ) {     //  There is an upper polygon

      for ( Index_type jj = 0 ; jj < shn ; ++jj ) {
        rz[jj + max_polygon_pts - shn] = dzt ;   // project to upper face
      }

      Real_type asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
      Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;

      vtxcnt += intsc24_shxf1
          ( dtx, dty, xt0, yt0, zt0, rx + max_polygon_pts - shn, shn,
            asum0, asumx, asumy, asumz ) ;

      Real_type det  = dtx * dty ;    // area of the target zone (determinant)

      sum0 += asum0 * det ;
      sumx += asumx * det ;
      sumy += asumy * det ;
      sumz += asumz * det ;
    }

    for ( Index_type jj = 0 ; jj < shn4 ; ++jj ) {
      rx[jj] = sx[jj] ;   ry[jj] = sy[jj] ;  rz[jj] = sz[jj] ;
    }

    // Central polygon - clip below z=dzt
    shn1 = clip_polygon_lt
        ( rz, rx, ry, dzt, shn4 ) ;

    //  Central polygon - clip above z=0
    shn = clip_polygon_ge
        ( rz, rx, ry, true, 0.0, shn1 ) ;

    if ( shn >= 3 ) {     //  There is an intersecting polygon

      Real_type asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;

      Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;

      vtxcnt += intsc24_shxf1
          ( dtx, dty, xt0, yt0, zt0, rx, shn,
            asum0, asumx, asumy, asumz ) ;

      Real_type det  = dtx * dty ;    // area of the target zone (determinant)

      sum0 += asum0 * det ;
      sumx += asumx * det ;
      sumy += asumy * det ;
      sumz += asumz * det ;
    }
  }

  // abovez (contribution from facet above the target zone).
  nclip = intsc24_hex_mask_to_list ( abovez, facet_list ) ;

  for ( Index_type fi = 0 ; fi < nclip ; fi++ ) {  // facet index in facet_list

    Int_type f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    Int_type k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    rz[0] = rz[1] = rz[2] = dzt ;     // project Z to upper face.

    Int_type shn = 3 ;

    Real_type asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
    Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;

    vtxcnt += intsc24_shxf1
        ( dtx, dty, xt0, yt0, zt0, rx, shn,
          asum0, asumx, asumy, asumz ) ;

    Real_type det  = dtx * dty ;    // area of the target zone (determinant)

    sum0 += asum0 * det ;
    sumx += asumx * det ;
    sumy += asumy * det ;
    sumz += asumz * det ;
  }

  // inside (contribution from facets inside the target zone)
  nclip = intsc24_hex_mask_to_list ( inside, facet_list ) ;

  for ( Index_type fi = 0 ; fi < nclip ; fi++ ) {  // facet index in facet_list

    Int_type f  = facet_list[fi] >> 2 ;        //  which face is facet/4
    Int_type k0 = facet_list[fi] & 3 ;         //  which facet within face

    intsc24_hex_get_tri
        ( xd, yd, zd, xt0, yt0, zt0, a11, a22, f, k0, rx, ry, rz ) ;

    Int_type shn = 3 ;

    Real_type asum0 = 0.0, asumx = 0.0, asumy = 0.0, asumz = 0.0 ;
    Real_type dtx = xt1 - xt0, dty = yt1 - yt0 ;

    vtxcnt += intsc24_shxf1
        ( dtx, dty, xt0, yt0, zt0, rx, shn,
          asum0, asumx, asumy, asumz ) ;

    Real_type det  = dtx * dty ;    // area of the target zone (determinant)

    sum0 += asum0 * det ;
    sumx += asumx * det ;
    sumy += asumy * det ;
    sumz += asumz * det ;
  }

  return vtxcnt ;
}

}  // end namespace rajaperf


#define INTSC_HEXRECT_BODY \
  if ( irec < nrecords ) { \
    Real_ptr record = ((Real_type*)records) + 4 * irec ; \
    Real_type xd[24] ; \
    Real_ptr yd = xd + 8 ; \
    Real_ptr zd = yd + 8 ; \
    { \
      Int_type dzone = intsc_d[irec] ; \
      for ( Index_type j=0 ; j<8 ; j++) { \
        Int_type node = znlist[ 8*dzone + j ] ; \
        xd[j] = xdnode[node] ; \
        yd[j] = ydnode[node] ; \
        zd[j] = zdnode[node] ; } \
    } \
    { \
      Real_type sum0, sumx, sumy, sumz ; \
      Real_const_ptr zplane ; \
      Real_const_ptr yplane ; \
      Real_const_ptr xplane ; \
      Int_type jz, jy, jx ; \
      { \
        Int_ptr ncord = (Int_ptr) ncord_gpu ; \
        Real_const_ptr_ptr planes = ( Real_const_ptr_ptr ) ( ncord + 4 ) ; \
        zplane = ( Real_const_ptr ) ( planes + 3 ) ; \
        yplane = zplane + ncord[0] + 1 ; \
        xplane = yplane + ncord[1] + 1 ; \
        Int_type const nyzones = ncord[1] ; \
        Int_type const nxzones = ncord[2] ; \
        Int_type tz = intsc_t[irec] ; \
        jz = tz / ( nxzones * nyzones ) ; \
        jy = ( tz / nxzones ) % nyzones ; \
        jx = tz % nxzones ; \
      } \
      intsc24_hex \
          ( xd, my_qx, \
            xplane[jx], xplane[jx+1], yplane[jy], yplane[jy+1], \
            zplane[jz], zplane[jz+1], \
            sum0, sumx, sumy, sumz ) ; \
      { \
        Real_type vf   = 1.0 ; \
        record[0] = vf * sum0 ; \
        record[1] = vf * sumx ; \
        record[2] = vf * sumy ; \
        record[3] = vf * sumz ; \
      } \
    } \
  }

#define INTSC_HEXRECT_SEQ(i) \
  Index_type irec = i ; \
  Real_type xd_work[ 3*max_polygon_pts+1 ] ; \
  Real_ptr my_qx = xd_work ; \
  INTSC_HEXRECT_BODY ;

#define INTSC_HEXRECT_OMP(i) \
  INTSC_HEXRECT_SEQ(i)

#endif // closing include guard RAJAPerf_Apps_INTSC_HEXRECT_BODY_HPP
