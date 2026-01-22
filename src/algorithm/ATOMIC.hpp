//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// ATOMIC kernel reference implementation:
/// Test atomic throughput with an amount of replication known at compile time.
///
/// for (Index_type i = 0; i < N; ++i ) {
///   atomic[i%replication] += 1;
/// }
///

#ifndef RAJAPerf_Algorithm_ATOMIC_HPP
#define RAJAPerf_Algorithm_ATOMIC_HPP

#define ATOMIC_DATA_SETUP(replication) \
  Real_type init = m_init; \
  Real_ptr atomic; \
  allocAndInitDataConst(atomic, replication, init, vid);

#define ATOMIC_DATA_TEARDOWN(replication) \
  { \
    Real_ptr atomic_host = atomic; \
    DataSpace ds = getDataSpace(vid); \
    DataSpace hds = rajaperf::hostCopyDataSpace(ds); \
    if (ds != hds) { \
      allocData(hds, atomic_host, replication); \
      copyData(hds, atomic_host, ds, atomic, replication); \
    } \
    m_final = init; \
    for (size_t r = 0; r < replication; ++r ) { \
      m_final += atomic_host[r]; \
    } \
    if (ds != hds) { \
      deallocData(hds, atomic_host); \
    } \
  } \
  deallocData(atomic, vid);

#define ATOMIC_VALUE 1.0

#define ATOMIC_BODY(ATOMIC_ADD, i, val) \
  ATOMIC_ADD(atomic[(i)%replication], (val))


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace algorithm
{

class ATOMIC : public KernelBase
{
public:

  ATOMIC(const RunParams& params);

  ~ATOMIC();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineOpenMPTargetVariantTunings();

  template < size_t replication >
  void runSeqVariantReplicate(VariantID vid);

  template < size_t replication >
  void runOpenMPVariantReplicate(VariantID vid);

  template < size_t block_size, size_t replication >
  void runCudaVariantReplicateGlobal(VariantID vid);
  template < size_t block_size, size_t replication >
  void runHipVariantReplicateGlobal(VariantID vid);
  template < size_t block_size, size_t replication >

  void runCudaVariantReplicateWarp(VariantID vid);
  template < size_t block_size, size_t replication >
  void runHipVariantReplicateWarp(VariantID vid);

  template < size_t block_size, size_t replication >
  void runCudaVariantReplicateBlock(VariantID vid);
  template < size_t block_size, size_t replication >
  void runHipVariantReplicateBlock(VariantID vid);

  template < size_t replication >
  void runOpenMPTargetVariantReplicate(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;
  static const size_t default_cpu_atomic_replication = 64;
  using cpu_atomic_replications_type = integer::make_atomic_replication_list_type<default_cpu_atomic_replication>;
  static const size_t default_atomic_replication = 4096;
  using gpu_atomic_replications_type = integer::make_atomic_replication_list_type<default_atomic_replication>;

  Real_type m_init;
  Real_type m_final;
};

} // end namespace algorithm
} // end namespace rajaperf

#endif // closing endif for header file include guard
