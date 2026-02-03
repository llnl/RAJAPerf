//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// REDUCE3_INT kernel reference implementation:
///
/// Int_type vsum = m_vsum_init;
/// Int_type vmin = m_vmin_init;
/// Int_type vmax = m_vmax_init;
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   vsum += vec[i] ;
///   vmin = RAJA_MIN(vmin, vec[i]) ;
///   vmax = RAJA_MAX(vmax, vec[i]) ;
/// }
///
/// m_vsum = vsum;
/// m_vmin = vmin;
/// m_vmax = vmax;
///
/// RAJA_MIN/MAX are macros that do what you would expect.
///

#ifndef RAJAPerf_Basic_REDUCE3_INT_HPP
#define RAJAPerf_Basic_REDUCE3_INT_HPP


#define REDUCE3_INT_DATA_SETUP \
  Int_ptr vec = m_vec; \

#define REDUCE3_INT_BODY  \
  vsum += vec[i] ; \
  vmin = RAJA_MIN(vmin, vec[i]) ; \
  vmax = RAJA_MAX(vmax, vec[i]) ;

#define REDUCE3_INT_BODY_RAJA  \
  vsum += vec[i] ; \
  vmin.min(vec[i]) ; \
  vmax.max(vec[i]) ;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class REDUCE3_INT : public KernelBase
{
public:

  REDUCE3_INT(const RunParams& params);

  ~REDUCE3_INT();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineOpenMPTargetVariantTunings();
  void defineKokkosVariantTunings();
  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runOpenMPTargetVariant(VariantID vid);
  void runKokkosVariant(VariantID vid);

  template < size_t tune_idx >
  void runSeqVariant(VariantID vid);

  template < size_t tune_idx >
  void runOpenMPVariant(VariantID vid);

  template < size_t block_size, typename MappingHelper >
  void runCudaVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runCudaVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runCudaVariantRAJANewReduce(VariantID vid);

  template < size_t block_size, typename MappingHelper >
  void runHipVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runHipVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runHipVariantRAJANewReduce(VariantID vid);

  template < size_t work_group_size > 
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Int_ptr m_vec;
  Int_type m_vsum;
  Int_type m_vsum_init;
  Int_type m_vmax;
  Int_type m_vmax_init;
  Int_type m_vmin;
  Int_type m_vmin_init;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
