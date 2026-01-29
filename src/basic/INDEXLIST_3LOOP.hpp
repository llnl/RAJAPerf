//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// INDEXLIST_3LOOP kernel reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   counts[i] = (x[i] < 0.0) ? 1 : 0;
/// }
///
/// Index_type count = 0;
/// for (Index_type i = ibegin; i < iend+1; ++i ) {
///   Index_type inc = counts[i];
///   counts[i] = count;
///   count += inc;
/// }
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   if (counts[i] != counts[i+1]) {
///     list[counts[i]] = i;
///   }
/// }
///
/// Index_type len = counts[iend];
///

#ifndef RAJAPerf_Basic_INDEXLIST_3LOOP_HPP
#define RAJAPerf_Basic_INDEXLIST_3LOOP_HPP

#define INDEXLIST_3LOOP_DATA_SETUP \
  Real_ptr x = m_x; \
  Int_ptr list = m_list;

#define INDEXLIST_3LOOP_COUNTS_SETUP(dataSpace) \
  Index_ptr counts = nullptr; \
  allocData((dataSpace), counts, iend+1);

#define INDEXLIST_3LOOP_COUNTS_TEARDOWN(dataSpace) \
  deallocData((dataSpace), counts);

#define INDEXLIST_3LOOP_CONDITIONAL \
  x[i] < 0.0

#define INDEXLIST_3LOOP_MAKE_LIST \
  if (counts[i] != counts[i+1]) { \
    list[counts[i]] = i ; \
  }


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class INDEXLIST_3LOOP : public KernelBase
{
public:

  INDEXLIST_3LOOP(const RunParams& params);

  ~INDEXLIST_3LOOP();

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

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_x;
  Int_ptr m_list;
  Index_type m_len;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
