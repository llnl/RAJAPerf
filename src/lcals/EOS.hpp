//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// EOS kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   x[i] = u[i] + r*( z[i] + r*y[i] ) +
///                 t*( u[i+3] + r*( u[i+2] + r*u[i+1] ) +
///                    t*( u[i+6] + q*( u[i+5] + q*u[i+4] ) ) );
/// }
///

#ifndef RAJAPerf_Lcals_EOS_HPP
#define RAJAPerf_Lcals_EOS_HPP


#define EOS_DATA_SETUP \
  Real_ptr x = m_x; \
  Real_ptr y = m_y; \
  Real_ptr z = m_z; \
  Real_ptr u = m_u; \
\
  const Real_type q = m_q; \
  const Real_type r = m_r; \
  const Real_type t = m_t;

#define EOS_BODY  \
  x[i] = u[i] + r*( z[i] + r*y[i] ) + \
                t*( u[i+3] + r*( u[i+2] + r*u[i+1] ) + \
                   t*( u[i+6] + q*( u[i+5] + q*u[i+4] ) ) );


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace lcals
{

class EOS : public KernelBase
{
public:

  EOS(const RunParams& params);

  ~EOS();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void runSeqVariant(VariantID vid);
  void defineSeqVariantTunings();
  void runOpenMPVariant(VariantID vid);
  void defineOpenMPVariantTunings();
  void runOpenMPTargetVariant(VariantID vid);
  void defineOpenMPTargetVariantTunings();
  void runKokkosVariant(VariantID vid);
  void defineKokkosVariantTunings();

  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Real_ptr m_y;
  Real_ptr m_z;
  Real_ptr m_u;

  Real_type m_q;
  Real_type m_r;
  Real_type m_t;

  Index_type m_array_length;
};

} // end namespace lcals
} // end namespace rajaperf

#endif // closing endif for header file include guard
