//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/// Intersection between 24-sided hexahedrons and a rectangular solid,
///  volume and moments.
///
///   for ( irec == 0 ; irec < nrecords ; ++irec ) {
///     double *record = ((double*)records) + 4 * irec ;
///     double xd[24] ;
///     double *yd = xd + 8 ;
///     double *zd = yd + 8 ;
///     {
///       int dzone = intsc_d[irec] ;
///       for (int j=0 ; j<8 ; j++) {
///         int node = znlist[ 8*dzone + j ] ;
///         xd[j] = xdnode[node] ;
///         yd[j] = ydnode[node] ;
///         zd[j] = zdnode[node] ; }
///     }
///     {
///       double sum0, sumx, sumy, sumz ;
///       double const *zplane ;
///       double const *yplane ;
///       double const *xplane ;
///       int jz, jy, jx ;
///       {
///         int *ncord = (int*) ncord_gpu ;
///         double const **planes = ( double const** ) ( ncord + 4 ) ;
///         zplane = ( double const* ) ( planes + 3 ) ;
///         yplane = zplane + ncord[0] + 1 ;
///         xplane = yplane + ncord[1] + 1 ;
///         int const nyzones = ncord[1] ;
///         int const nxzones = ncord[2] ;
///         int tz = intsc_t[irec] ;
///         jz = tz / ( nxzones * nyzones ) ;
///         jy = ( tz / nxzones ) % nyzones ;
///         jx = tz % nxzones ;
///       }
///       intsc24_hex
///           ( xd, my_qx,
///             xplane[jx], xplane[jx+1], yplane[jy], yplane[jy+1],
///             zplane[jz], zplane[jz+1],
///             sum0, sumx, sumy, sumz ) ;
///       record[0] = sum0 ;
///       record[1] = sumx ;
///       record[2] = sumy ;
///       record[3] = sumz ;
///       }
///     }
///   }

#ifndef RAJAPerf_Apps_INTSC_HEXRECT_HPP
#define RAJAPerf_Apps_INTSC_HEXRECT_HPP


#include "RAJA/RAJA.hpp"

#include "common/RPTypes.hpp"

namespace rajaperf {

static int constexpr max_polygon_pts = 10 ;

//  24 triangular facets on hexahedron zone, intersected with rectangular solid
static Size_type constexpr tri_per_hex = 24 ;

//  Number of hex-rectangular solid intersections per donor zone
static Size_type constexpr intsc_per_zone = 8 ;

//  Number of computed values per intersection (volume, x, y, z moments).
static int constexpr nvals_hexrect = 4 ;

}  // end namespace rajaperf

#define  INTSC_HEXRECT_DATA_SETUP \
  Char_ptr ncord_gpu = m_ncord ; \
  Real_ptr xdnode = m_xdnode ; \
  Real_ptr ydnode = m_ydnode ; \
  Real_ptr zdnode = m_zdnode ; \
  Int_ptr znlist  = m_znlist  ; \
  Int_ptr intsc_d = m_intsc_d ; \
  Int_ptr intsc_t = m_intsc_t ; \
  Index_type const nrecords = m_nrecords ; \
  Real_ptr records = m_records ;

#include "INTSC_HEXRECT_BODY.hpp"

#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace apps
{

class INTSC_HEXRECT : public KernelBase
{
public:

  INTSC_HEXRECT(const RunParams& params);

  ~INTSC_HEXRECT();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);

  template < Size_type block_size >
  void runCudaVariantImpl(VariantID vid);
  template < Size_type block_size >
  void runHipVariantImpl(VariantID vid);

private:
  void setupTargetPlanes
      ( Real_ptr_ptr planes, Int_ptr ncord,
        Int_type const ndx, Int_type const ndy, Int_type const ndz,
        Real_type const x0, Real_type const y0, Real_type const z0,
        Real_type const sep ) ;

  void setupDonorMesh
      ( Real_type const sep,
        Real_type const xd0, Real_type const yd0, Real_type const zd0,
        Int_type const ndx, Int_type const ndy, Int_type const ndz,
        Real_ptr x, Real_ptr y, Real_ptr z,
        Int_ptr znlist ) ;

  void setupIntscPairs
      ( Int_const_ptr ncord,
        Int_type const ndx, Int_type const ndy, Int_type const ndz,
        Int_ptr intsc_d,
        Int_ptr intsc_t ) ;

  void copyTargetToDevice
      ( Real_const_ptr_ptr planes,
        Int_const_ptr ncord,
        VariantID vid ) ;

  void checkMoments
      ( Real_ptr records, Int_type const n_intsc,
        Int_type const ndx, Int_type const ndy, Int_type const ndz,
        Real_type const xd0, Real_type const yd0, Real_type const zd0,
        Real_type const x0, Real_type const y0, Real_type const z0,
        Real_type const sep,
        Real_type const sep1x, Real_type const sep1y, Real_type const sep1z,
        VariantID vid ) ;

  void checkScaledVolumes
      ( Real_const_ptr records,
        Int_type const x_scl_offs,
        Int_type const y_scl_offs,
        Int_type const z_scl_offs,
        Real_type const sep, VariantID vid ) ;

  static const Size_type default_gpu_block_size = 64;
  using gpu_block_sizes_type =
      integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Size_type m_ndzones ;    // number of "donor zones"
  Size_type m_ntzones ;    // number of "target zones"

  Real_ptr m_xdnode ;          // [ndnodes] x coordinates for donor
  Real_ptr m_ydnode ;          // [ndnodes] y coordinates for donor
  Real_ptr m_zdnode ;          // [ndnodes] z coordinates for donor
  Int_ptr m_znlist ;           // [donor zones][8] donor zone node list
  Char_ptr m_ncord ;           //  target dimensions and coordinates
  Int_ptr m_intsc_d ;          // [nrecords] donor zones to intersect
  Int_ptr m_intsc_t ;          // [nrecords] target zones to intersect
  Index_type m_nrecords ;      // Number of threads (one thread per record)
  Real_ptr m_records ;         // output volumes, moments
  Real_ptr m_records_h ;       // volumes, moments on the host

  Real_type m_xd0, m_yd0, m_zd0 ;    // donor corner
  Real_type m_x0, m_y0, m_z0 ;       // corner of target mesh
  Real_type m_sep ;                  // target mesh pitch (separation)
  Real_type m_sep1x, m_sep1y, m_sep1z ;   // donor mesh zone widths
  Int_type  m_x_scl_offs, m_y_scl_offs, m_z_scl_offs ;  // donor scaled offsets
  Int_type  m_ndx, m_ndy, m_ndz ;     // donor mesh dimensions (are equal)
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
