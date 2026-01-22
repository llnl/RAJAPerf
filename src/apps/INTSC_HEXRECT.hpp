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
///         Plane* plane = reinterpret_cast<Plane*>(ncord_gpu) ;
///         zplane = plane->planes[0] ;
///         yplane = plane->planes[1] ;
///         xplane = plane->planes[2] ;
///         Int_type const nyzones = plane->ncord[1] ;
///         Int_type const nxzones = plane->ncord[2] ;
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
  struct Plane {
    Int_type ncord[3];  // number of target zones
    Real_ptr planes[3]; // pointers to planes arrays
  };

  void setupTargetPlanes(VariantID vid);
  void setupDonorMesh();
  void setupIntscPairs();

  void checkMoments(VariantID vid);
  void checkScaledVolumes(VariantID vid);

  static const Size_type default_gpu_block_size = 64;
  using gpu_block_sizes_type =
      integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_xdnode ;          // [ndnodes] x coordinates for donor
  Real_ptr m_ydnode ;          // [ndnodes] y coordinates for donor
  Real_ptr m_zdnode ;          // [ndnodes] z coordinates for donor
  Int_ptr m_znlist ;           // [donor zones][8] donor zone node list
  Char_ptr m_ncord ;           // target dimensions and coordinates
  Int_ptr m_intsc_d ;          // [nrecords] donor zones to intersect
  Int_ptr m_intsc_t ;          // [nrecords] target zones to intersect
  Real_ptr m_records ;         // output volumes, moments
  Real_ptr m_records_h ;       // volumes, moments on the host

  Real_type m_xd0, m_yd0, m_zd0 ;    // donor corner
  Real_type m_x0, m_y0, m_z0 ;       // corner of target mesh
  Real_type m_sep ;                  // target mesh pitch (separation)
  Real_type m_sep1x, m_sep1y, m_sep1z ;   // donor mesh zone widths
  Int_type  m_x_scl_offs, m_y_scl_offs, m_z_scl_offs ;  // donor scaled offsets
  Int_type  m_ndx, m_ndy, m_ndz ;    // donor mesh dimensions (are equal)

  Index_type m_ndzones ;    // number of "donor zones"
  Index_type m_ntzones ;    // number of "target zones"
  Index_type m_ndnodes;     // number of nodes in donor mesh
  Index_type m_nrecords ;   // Number of threads (one thread per record)
  Index_type m_nplanes;     // total number of target places (in all dimensions)
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
