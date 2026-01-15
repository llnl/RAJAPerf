//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// INDEXLIST kernel reference implementation:
///
/// Index_type count = 0;
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   if (x[i] < 0.0) {
///     list[count++] = i ;
///   }
/// }
/// Index_type len = count;
///

#ifndef RAJAPerf_Basic_INDEXLIST_HPP
#define RAJAPerf_Basic_INDEXLIST_HPP

#define INDEXLIST_DATA_SETUP \
  Real_ptr x = m_x; \
  Int_ptr list = m_list;

#define INDEXLIST_CONDITIONAL  \
  x[i] < 0.0

#define INDEXLIST_BODY  \
  if (INDEXLIST_CONDITIONAL) { \
    list[count++] = i ; \
  }


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class INDEXLIST : public KernelBase
{
public:

  INDEXLIST(const RunParams& params);

  ~INDEXLIST();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineOpenMPTargetVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid);
  
  template < size_t block_size, size_t items_per_thread >
  void runCudaVariantCustom(VariantID vid);
  template < size_t block_size, size_t items_per_thread >
  void runHipVariantCustom(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Int_ptr m_list;
  Index_type m_len;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
