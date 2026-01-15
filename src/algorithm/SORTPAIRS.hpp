//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// SORTPAIRS kernel reference implementation:
///
/// std::sort(x+ibegin, x+iend);
///

#ifndef RAJAPerf_Algorithm_SORTPAIRS_HPP
#define RAJAPerf_Algorithm_SORTPAIRS_HPP

#define SORTPAIRS_DATA_SETUP \
  Real_ptr x = m_x;          \
  Real_ptr i = m_i;

#define RAJA_SORTPAIRS_ARGS  \
  RAJA::make_span(x + iend*irep + ibegin, iend - ibegin), \
  RAJA::make_span(i + iend*irep + ibegin, iend - ibegin)


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace algorithm
{

class SORTPAIRS : public KernelBase
{
public:

  SORTPAIRS(const RunParams& params);

  ~SORTPAIRS();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runCudaVariant(VariantID vid);
  void runHipVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid)
  {
    getCout() << "\n  SORTPAIRS : Unknown OMP Target variant id = " << vid << std::endl;
  }

private:
  static const size_t default_gpu_block_size = 0;

  Real_ptr m_x;
  Real_ptr m_i;
};

} // end namespace algorithm
} // end namespace rajaperf

#endif // closing endif for header file include guard
