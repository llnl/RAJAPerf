//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// MEMSET kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   x[i] = val ;
/// }
///

#ifndef RAJAPerf_Algorithm_MEMSET_HPP
#define RAJAPerf_Algorithm_MEMSET_HPP

#define MEMSET_DATA_SETUP \
  Real_ptr x = m_x; \
  Real_type val = m_val;

#define MEMSET_STD_ARGS  \
  x + ibegin, (int)val, (iend-ibegin)*sizeof(Real_type)

#define MEMSET_BODY \
  x[i] = val;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace algorithm
{

class MEMSET : public KernelBase
{
public:

  MEMSET(const RunParams& params);

  ~MEMSET();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineOpenMPTargetVariantTunings();

  void runSeqVariantDefault(VariantID vid);
  void runSeqVariantLibrary(VariantID vid);

  void runOpenMPVariant(VariantID vid);

  template < size_t block_size >
  void runCudaVariantBlock(VariantID vid);
  void runCudaVariantLibrary(VariantID vid);

  template < size_t block_size >
  void runHipVariantBlock(VariantID vid);
  void runHipVariantLibrary(VariantID vid);

  void runOpenMPTargetVariant(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Real_type m_val;
};

} // end namespace algorithm
} // end namespace rajaperf

#endif // closing endif for header file include guard
