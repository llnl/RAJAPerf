//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// FIR kernel reference implementation:
///
/// #define FIR_COEFFLEN (16)
///
/// Real_type coeff[FIR_COEFFLEN] = { 3.0, -1.0, -1.0, -1.0,
///                                  -1.0, 3.0, -1.0, -1.0,
///                                  -1.0, -1.0, 3.0, -1.0,
///                                  -1.0, -1.0, -1.0, 3.0 };
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   Real_type sum = 0.0;
///   for (Index_type j = 0; j < coefflen; ++j ) {
///     sum += coeff[j]*in[i+j];
///   }
///   out[i] = sum;
/// }
///

#ifndef RAJAPerf_Apps_FIR_HPP
#define RAJAPerf_Apps_FIR_HPP


#define FIR_COEFFLEN (16)

#define FIR_DATA_SETUP \
  Real_ptr in = m_in; \
  Real_ptr out = m_out; \
\
  const Index_type coefflen = m_coefflen;

#define FIR_COEFF \
  Real_type coeff_array[FIR_COEFFLEN] = { 3.0, -1.0, -1.0, -1.0, \
                                         -1.0, 3.0, -1.0, -1.0, \
                                         -1.0, -1.0, 3.0, -1.0, \
                                         -1.0, -1.0, -1.0, 3.0 };

#define FIR_BODY(coeff) \
  Real_type sum = 0.0; \
\
  for (Index_type j = 0; j < coefflen; ++j ) { \
    sum += (coeff)[j]*in[i+j]; \
  } \
  out[i] = sum;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace apps
{

class FIR : public KernelBase
{
public:

  FIR(const RunParams& params);

  ~FIR();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void runSeqVariant(VariantID vid, size_t tune_idx);
  void runOpenMPVariant(VariantID vid, size_t tune_idx);
  void runCudaVariant(VariantID vid, size_t tune_idx);
  void runHipVariant(VariantID vid, size_t tune_idx);
  void runOpenMPTargetVariant(VariantID vid, size_t tune_idx);
  void runSyclVariant(VariantID vid, size_t tune_idx);

  void setCudaTuningDefinitions(VariantID vid);
  void setHipTuningDefinitions(VariantID vid);
  void setSyclTuningDefinitions(VariantID vid);

  template < size_t block_size >
  void runCudaVariantParam(VariantID vid);
  template < size_t block_size >
  void runCudaVariantConst(VariantID vid);
  template < size_t block_size >
  void runCudaVariantShared(DataSpace dataSpace, VariantID vid);
  template < size_t block_size >
  void runCudaVariantMemory(DataSpace dataSpace, VariantID vid);
  template < size_t block_size >
  void runCudaVariantParamGridConst(VariantID vid);
  template < size_t block_size >
  void runHipVariantParam(VariantID vid);
  template < size_t block_size >
  void runHipVariantConst(VariantID vid);
  template < size_t block_size >
  void runHipVariantShared(DataSpace dataSpace, VariantID vid);
  template < size_t block_size >
  void runHipVariantMemory(DataSpace dataSpace, VariantID vid);
  template < size_t block_size >
  void runHipVariantParamReorder(size_t reorder_num, VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_ptr m_in;
  Real_ptr m_out;

  Index_type m_coefflen;
};

struct FIR_Array {
  Real_type array[FIR_COEFFLEN];

  template < size_t ... Indices >
  FIR_Array(Real_type (&array_)[FIR_COEFFLEN],
                      camp::int_seq<size_t, Indices...>)
    : array{array_[Indices]...}
  { }

  FIR_Array(Real_type (&array_)[FIR_COEFFLEN])
    : FIR_Array(array_, camp::make_int_seq_t<size_t, FIR_COEFFLEN>{})
  { }
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
