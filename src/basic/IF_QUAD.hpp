//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// IF_QUAD kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   Real_type s = b[i]*b[i] - 4.0*a[i]*c[i];
///   if ( s >= 0 ) {
///     s = sqrt(s);
///     x2[i] = (-b[i]+s)/(2.0*a[i]);
///     x1[i] = (-b[i]-s)/(2.0*a[i]);
///   } else {
///     x2[i] = 0.0;
///     x1[i] = 0.0;
///   }
/// }
///

#ifndef RAJAPerf_Basic_IF_QUAD_HPP
#define RAJAPerf_Basic_IF_QUAD_HPP

#define IF_QUAD_DATA_SETUP \
  Real_ptr a = m_a; \
  Real_ptr b = m_b; \
  Real_ptr c = m_c; \
  Real_ptr x1 = m_x1; \
  Real_ptr x2 = m_x2;

#define IF_QUAD_BODY  \
  Real_type s = b[i]*b[i] - 4.0*a[i]*c[i]; \
  if ( s >= 0 ) { \
    s = sqrt(s); \
    x2[i] = (-b[i]+s)/(2.0*a[i]); \
    x1[i] = (-b[i]-s)/(2.0*a[i]); \
  } else { \
    x2[i] = 0.0; \
    x1[i] = 0.0; \
  }

#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class IF_QUAD : public KernelBase
{
public:

  IF_QUAD(const RunParams& params);

  ~IF_QUAD();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineOpenMPTargetVariantTunings();
  void defineKokkosVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid);
  void runKokkosVariant(VariantID vid);

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_a;
  Real_ptr m_b;
  Real_ptr m_c;
  Real_ptr m_x1;
  Real_ptr m_x2;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
