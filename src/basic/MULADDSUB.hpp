//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// MULADDSUB kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   out1[i] = in1[i] * in2[i] ;
///   out2[i] = in1[i] + in2[i] ;
///   out3[i] = in1[i] - in2[i] ;
/// }
///

#ifndef RAJAPerf_Basic_MULADDSUB_HPP
#define RAJAPerf_Basic_MULADDSUB_HPP

#define MULADDSUB_DATA_SETUP \
  Real_ptr out1 = m_out1; \
  Real_ptr out2 = m_out2; \
  Real_ptr out3 = m_out3; \
  Real_ptr in1 = m_in1; \
  Real_ptr in2 = m_in2;

#define MULADDSUB_BODY  \
  out1[i] = in1[i] * in2[i] ; \
  out2[i] = in1[i] + in2[i] ; \
  out3[i] = in1[i] - in2[i] ;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class MULADDSUB : public KernelBase
{
public:

  MULADDSUB(const RunParams& params);

  ~MULADDSUB();

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

  Real_ptr m_out1;
  Real_ptr m_out2;
  Real_ptr m_out3;
  Real_ptr m_in1;
  Real_ptr m_in2;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
