//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// PI_ATOMIC kernel reference implementation:
///
/// const int N = ...;  -- num [0, 1] sub-intervals used in Riemann integration
/// const double dx = 1.0 / double(num_bins);
///
/// double pi = 0.0;
/// for (Index_type i = 0; i < N; ++i ) {
///   double x = (double(i) + 0.5) * dx;
///   pi += dx / (1.0 + x * x);
/// }
/// pi *= 4.0;
///

#ifndef RAJAPerf_Basic_PI_ATOMIC_HPP
#define RAJAPerf_Basic_PI_ATOMIC_HPP

#define PI_ATOMIC_DATA_SETUP \
  Real_type dx = m_dx; \
  Real_ptr pi; \
  allocData(getReductionDataSpace(vid), pi, 1);

#define PI_ATOMIC_DATA_TEARDOWN \
  deallocData(pi, vid);

#define PI_ATOMIC_GPU_DATA_SETUP \
  Real_type dx = m_dx;

#define PI_ATOMIC_BODY(atomicAdd) \
  Real_type x = (Real_type(i) + 0.5) * dx; \
  atomicAdd(*pi, dx / (1.0 + x * x));


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class PI_ATOMIC : public KernelBase
{
public:

  PI_ATOMIC(const RunParams& params);

  ~PI_ATOMIC();

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

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid);
  void runKokkosVariant(VariantID vid);

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_type m_dx;
  Real_type m_pi_init;
  Real_type m_pi_final;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
