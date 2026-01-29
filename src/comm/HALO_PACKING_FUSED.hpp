//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// HALO_PACKING_FUSED kernel reference implementation:
///
/// // pack buffers for neighbors
/// for (Index_type l = 0; l < num_neighbors; ++l) {
///   Real_ptr buffer = pack_buffers[l];
///   Int_ptr list = pack_index_lists[l];
///   Index_type len = pack_index_list_lengths[l];
///   // pack part of each variable
///   for (Index_type v = 0; v < num_vars; ++v) {
///     Real_ptr var = vars[v];
///     for (Index_type i = 0; i < len; i++) {
///       buffer[i] = var[list[i]];
///     }
///     buffer += len;
///   }
/// }
///
/// // unpack buffers for neighbors
/// for (Index_type l = 0; l < num_neighbors; ++l) {
///   Real_ptr buffer = unpack_buffers[l];
///   Int_ptr list = unpack_index_lists[l];
///   Index_type len = unpack_index_list_lengths[l];
///   // unpack part of each variable
///   for (Index_type v = 0; v < num_vars; ++v) {
///     Real_ptr var = vars[v];
///     for (Index_type i = 0; i < len; i++) {
///       var[list[i]] = buffer[i];
///     }
///     buffer += len;
///   }
/// }
///

#ifndef RAJAPerf_Comm_HALO_PACKING_FUSED_HPP
#define RAJAPerf_Comm_HALO_PACKING_FUSED_HPP

#define HALO_PACKING_FUSED_DATA_SETUP \
  HALO_BASE_DATA_SETUP \
  \
  Index_type num_vars = m_num_vars; \
  Real_ptr_ptr vars = m_vars; \
  \
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy); \
  \
  Real_ptr_ptr pack_buffers = m_pack_buffers; \
  Real_ptr_ptr unpack_buffers = m_unpack_buffers; \
  \
  Real_ptr_ptr send_buffers = m_send_buffers; \
  Real_ptr_ptr recv_buffers = m_recv_buffers;

#define HALO_PACKING_FUSED_MANUAL_FUSER_SETUP \
  ptr_holder* pack_ptr_holders = nullptr; \
  Index_ptr pack_lens = nullptr; \
  ptr_holder* unpack_ptr_holders = nullptr; \
  Index_ptr unpack_lens = nullptr; \
  allocData(DataSpace::Host, pack_ptr_holders, num_neighbors * num_vars); \
  allocData(DataSpace::Host, pack_lens, num_neighbors * num_vars); \
  allocData(DataSpace::Host, unpack_ptr_holders, num_neighbors * num_vars); \
  allocData(DataSpace::Host, unpack_lens, num_neighbors * num_vars);

#define HALO_PACKING_FUSED_MANUAL_FUSER_TEARDOWN \
  deallocData(DataSpace::Host, pack_ptr_holders); \
  deallocData(DataSpace::Host, pack_lens); \
  deallocData(DataSpace::Host, unpack_ptr_holders); \
  deallocData(DataSpace::Host, unpack_lens);


#define HALO_PACKING_FUSED_MANUAL_LAMBDA_FUSER_SETUP \
  auto make_pack_lambda = [](Real_ptr buffer, Int_ptr list, Real_ptr var) { \
    return [=](Index_type i) { \
      HALO_PACK_BODY; \
    }; \
  }; \
  using pack_lambda_type = decltype(make_pack_lambda(Real_ptr(), Int_ptr(), Real_ptr())); \
  pack_lambda_type* pack_lambdas = nullptr; \
  Index_ptr pack_lens = nullptr; \
  allocData(DataSpace::Host, pack_lambdas, num_neighbors * num_vars); \
  allocData(DataSpace::Host, pack_lens, num_neighbors * num_vars); \
  auto make_unpack_lambda = [](Real_ptr buffer, Int_ptr list, Real_ptr var) { \
    return [=](Index_type i) { \
      HALO_UNPACK_BODY; \
    }; \
  }; \
  using unpack_lambda_type = decltype(make_unpack_lambda(Real_ptr(), Int_ptr(), Real_ptr())); \
  unpack_lambda_type* unpack_lambdas = nullptr; \
  Index_ptr unpack_lens = nullptr; \
  allocData(DataSpace::Host, unpack_lambdas, num_neighbors * num_vars); \
  allocData(DataSpace::Host, unpack_lens, num_neighbors * num_vars);

#define HALO_PACKING_FUSED_MANUAL_LAMBDA_FUSER_TEARDOWN \
  deallocData(DataSpace::Host, pack_lambdas); \
  deallocData(DataSpace::Host, pack_lens); \
  deallocData(DataSpace::Host, unpack_lambdas); \
  deallocData(DataSpace::Host, unpack_lens);


#include "HALO_base.hpp"

#include "RAJA/RAJA.hpp"

namespace rajaperf
{
class RunParams;

namespace comm
{

class HALO_PACKING_FUSED : public HALO_base
{
public:
  struct ptr_holder {
    Real_ptr buffer;
    Int_ptr  list;
    Real_ptr var;
  };

  HALO_PACKING_FUSED(const RunParams& params);

  ~HALO_PACKING_FUSED();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineOpenMPTargetVariantTunings();

  void runSeqVariantDirect(VariantID vid);
  void runOpenMPVariantDirect(VariantID vid);
  void runOpenMPTargetVariantDirect(VariantID vid);
  template < size_t block_size >
  void runCudaVariantDirect(VariantID vid);
  template < size_t block_size >
  void runHipVariantDirect(VariantID vid);

  template < typename dispatch_helper >
  void runSeqVariantWorkGroup(VariantID vid);
  template < typename dispatch_helper >
  void runOpenMPVariantWorkGroup(VariantID vid);
  template < typename dispatch_helper >
  void runOpenMPTargetVariantWorkGroup(VariantID vid);
  template < size_t block_size, typename dispatch_helper >
  void runCudaVariantWorkGroup(VariantID vid);
  template < size_t block_size, typename dispatch_helper >
  void runHipVariantWorkGroup(VariantID vid);

private:
  static const size_t default_gpu_block_size = 1024;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Index_type m_num_vars;
  Index_type m_var_size;

  Real_ptr_ptr m_vars;
};

} // end namespace comm
} // end namespace rajaperf

#endif // closing endif for header file include guard
