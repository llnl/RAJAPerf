//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// REDUCE_SUM kernel reference implementation:
///
/// Real_type sum = std::reduce(x+ibegin, x+iend);
/// // or
/// Real_type sum = std::accumulate(x+ibegin, x+iend, 0.0);
/// // or
/// Real_type sum = 0.0;
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   sum += x[i] ;
/// }
///

#ifndef RAJAPerf_Algorithm_REDUCE_SUM_HPP
#define RAJAPerf_Algorithm_REDUCE_SUM_HPP

#define REDUCE_SUM_DATA_SETUP \
  Real_ptr x = m_x;

#define REDUCE_SUM_STD_ARGS  \
  x + ibegin, x + iend

#define REDUCE_SUM_BODY \
  sum += x[i];


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace algorithm
{

class REDUCE_SUM : public KernelBase
{
public:

  REDUCE_SUM(const RunParams& params);

  ~REDUCE_SUM();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineOpenMPTargetVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  template < size_t tune_idx >
  void runSeqVariant(VariantID vid);

  template < size_t tune_idx >
  void runOpenMPVariant(VariantID vid);

  void runCudaVariantCub(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runCudaVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runCudaVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runCudaVariantRAJANewReduce(VariantID vid);

  void runHipVariantRocprim(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runHipVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runHipVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runHipVariantRAJANewReduce(VariantID vid);

  void runOpenMPTargetVariant(VariantID vid);

  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Real_type m_sum_init;
  Real_type m_sum;
};

} // end namespace algorithm
} // end namespace rajaperf

#endif // closing endif for header file include guard
