//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// DIFF_PREDICT kernel reference implementation:
///
/// Index_type offset = iend - ibegin;
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   ar                  = cx[i + offset * 4];
///   br                  = ar - px[i + offset * 4];
///   px[i + offset * 4]  = ar;
///   cr                  = br - px[i + offset * 5];
///   px[i + offset * 5]  = br;
///   ar                  = cr - px[i + offset * 6];
///   px[i + offset * 6]  = cr;
///   br                  = ar - px[i + offset * 7];
///   px[i + offset * 7]  = ar;
///   cr                  = br - px[i + offset * 8];
///   px[i + offset * 8]  = br;
///   ar                  = cr - px[i + offset * 9];
///   px[i + offset * 9]  = cr;
///   br                  = ar - px[i + offset * 10];
///   px[i + offset * 10] = ar;
///   cr                  = br - px[i + offset * 11];
///   px[i + offset * 11] = br;
///   px[i + offset * 13] = cr - px[i + offset * 12];
///   px[i + offset * 12] = cr;
/// }
///

#ifndef RAJAPerf_Lcals_DIFF_PREDICT_HPP
#define RAJAPerf_Lcals_DIFF_PREDICT_HPP


#define DIFF_PREDICT_DATA_SETUP \
  Real_ptr px = m_px; \
  Real_ptr cx = m_cx; \
  const Index_type offset = m_offset;

#define DIFF_PREDICT_BODY  \
  Real_type ar, br, cr; \
\
  ar                  = cx[i + offset * 4];       \
  br                  = ar - px[i + offset * 4];  \
  px[i + offset * 4]  = ar;                       \
  cr                  = br - px[i + offset * 5];  \
  px[i + offset * 5]  = br;                       \
  ar                  = cr - px[i + offset * 6];  \
  px[i + offset * 6]  = cr;                       \
  br                  = ar - px[i + offset * 7];  \
  px[i + offset * 7]  = ar;                       \
  cr                  = br - px[i + offset * 8];  \
  px[i + offset * 8]  = br;                       \
  ar                  = cr - px[i + offset * 9];  \
  px[i + offset * 9]  = cr;                       \
  br                  = ar - px[i + offset * 10]; \
  px[i + offset * 10] = ar;                       \
  cr                  = br - px[i + offset * 11]; \
  px[i + offset * 11] = br;                       \
  px[i + offset * 13] = cr - px[i + offset * 12]; \
  px[i + offset * 12] = cr;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace lcals
{

class DIFF_PREDICT : public KernelBase
{
public:

  DIFF_PREDICT(const RunParams& params);

  ~DIFF_PREDICT();

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

  Real_ptr m_px;
  Real_ptr m_cx;

  Index_type m_array_length;
  Index_type m_offset;
};

} // end namespace lcals
} // end namespace rajaperf

#endif // closing endif for header file include guard
