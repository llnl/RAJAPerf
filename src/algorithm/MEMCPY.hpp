//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// MEMCPY kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   y[i] = x[i] ;
/// }
///

#ifndef RAJAPerf_Algorithm_MEMCPY_HPP
#define RAJAPerf_Algorithm_MEMCPY_HPP

#define MEMCPY_DATA_SETUP \
  Real_ptr x = m_x; \
  Real_ptr y = m_y; \

#define MEMCPY_STD_ARGS  \
  y + ibegin, x + ibegin, (iend-ibegin)*sizeof(Real_type)

#define MEMCPY_BODY \
  y[i] = x[i];


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace algorithm
{

class MEMCPY : public KernelBase
{
public:

  MEMCPY(const RunParams& params);

  ~MEMCPY();

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
  void runOpenMPTargetVariant(VariantID vid);

  template < size_t block_size >
  void runCudaVariantBlock(VariantID vid);
  void runCudaVariantLibrary(VariantID vid);

  template < size_t block_size >
  void runHipVariantBlock(VariantID vid);
  void runHipVariantLibrary(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Real_ptr m_y;
};

} // end namespace algorithm
} // end namespace rajaperf

#endif // closing endif for header file include guard
