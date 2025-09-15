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

#include "intsc_hexrect_body.hpp"

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
