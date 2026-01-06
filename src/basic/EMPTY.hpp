//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// EMPTY kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///
/// }
///

#ifndef RAJAPerf_Basic_EMPTY_HPP
#define RAJAPerf_Basic_EMPTY_HPP

#define EMPTY_DATA_SETUP

// Add something that will stop the loop from being optimized out
// while adding as little overhead as possible.
#if defined(_WIN32) || defined(_WIN64)
// Generate a loop with one write to the stack.
#define EMPTY_BODY volatile auto unused = i; RAJA_UNUSED_VAR( unused ) ;
#else
// Generate an empty loop.
#define EMPTY_BODY asm volatile ( "" ) ; RAJA_UNUSED_VAR( i ) ;
#endif


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class EMPTY : public KernelBase
{
public:

  EMPTY(const RunParams& params);

  ~EMPTY();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineOpenMPTargetVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid);

  template < size_t block_size, typename MappingHelper >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
