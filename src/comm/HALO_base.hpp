//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// HALO_base provides a common starting point for the other HALO_ classes.
///

#ifndef RAJAPerf_Comm_HALO_BASE_HPP
#define RAJAPerf_Comm_HALO_BASE_HPP

#define HALO_BASE_DATA_SETUP \
  Index_type num_neighbors = s_num_neighbors; \
  Int_ptr send_tags = m_send_tags; \
  Int_ptr_ptr pack_index_lists = m_pack_index_lists; \
  Index_ptr pack_index_list_lengths = m_pack_index_list_lengths; \
  Int_ptr recv_tags = m_recv_tags; \
  Int_ptr_ptr unpack_index_lists = m_unpack_index_lists; \
  Index_ptr unpack_index_list_lengths = m_unpack_index_list_lengths; \
  RAJAPERF_UNUSED_VAR(send_tags); \
  RAJAPERF_UNUSED_VAR(pack_index_lists); \
  RAJAPERF_UNUSED_VAR(recv_tags); \
  RAJAPERF_UNUSED_VAR(unpack_index_lists);

#define HALO_PACK_BODY \
  buffer[i] = var[list[i]];

#define HALO_UNPACK_BODY \
  var[list[i]] = buffer[i];


#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

namespace rajaperf
{
class RunParams;

struct direct_dispatch_helper
{
  template < typename... Ts >
  using dispatch_policy = RAJA::direct_dispatch<Ts...>;
  static std::string get_name() { return "direct"; }
};

struct indirect_function_call_dispatch_helper
{
  template < typename... Ts >
  using dispatch_policy = RAJA::indirect_function_call_dispatch;
  static std::string get_name() { return "funcptr"; }
};

struct indirect_virtual_function_dispatch_helper
{
  template < typename... Ts >
  using dispatch_policy = RAJA::indirect_virtual_function_dispatch;
  static std::string get_name() { return "virtfunc"; }
};

using workgroup_dispatch_helpers = camp::list<
    direct_dispatch_helper,
    indirect_function_call_dispatch_helper,
    indirect_virtual_function_dispatch_helper >;

using hip_workgroup_dispatch_helpers = camp::list<
    direct_dispatch_helper
#ifdef RAJA_ENABLE_HIP_INDIRECT_FUNCTION_CALL
   ,indirect_function_call_dispatch_helper
   ,indirect_virtual_function_dispatch_helper
#endif
    >;

namespace comm
{

class HALO_base : public KernelBase
{
public:

  HALO_base(KernelID kid, const RunParams& params);

  ~HALO_base();

  void setSize_base(Index_type target_size, Index_type target_reps);
  void setUp_base(const int my_mpi_rank, const int* mpi_dims,
                  const Index_type num_vars,
                  VariantID vid, size_t tune_idx);
  void tearDown_base(VariantID vid, size_t tune_idx);

  struct Packer {
    Real_ptr buffer;
    Real_ptr var;
    Int_ptr list;
    RAJA_HOST_DEVICE void operator()(Index_type i) const {
      HALO_PACK_BODY;
    }
  };

  struct UnPacker {
    Real_ptr buffer;
    Real_ptr var;
    Int_ptr list;
    RAJA_HOST_DEVICE void operator()(Index_type i) const {
      HALO_UNPACK_BODY;
    }
  };

protected:
  enum struct message_type : int
  {
    send,
    recv
  };

  struct Extent
  {
    Index_type i_min;
    Index_type i_max;
    Index_type j_min;
    Index_type j_max;
    Index_type k_min;
    Index_type k_max;
  };

  static const int s_num_neighbors = 26;
  static const int s_boundary_offsets[s_num_neighbors][3];

  static Index_type s_grid_dims_default[3];

  Index_type m_grid_dims[3];
  Index_type m_halo_width;

  Index_type m_grid_plus_halo_dims[3];
  Index_type m_grid_plus_halo_size;

  Int_ptr m_mpi_ranks;

  Int_ptr m_send_tags;
  Int_ptr_ptr m_pack_index_lists;
  Index_ptr m_pack_index_list_lengths;
  Real_ptr_ptr m_pack_buffers;
  Real_ptr_ptr m_send_buffers;

  Int_ptr m_recv_tags;
  Int_ptr_ptr m_unpack_index_lists;
  Index_ptr m_unpack_index_list_lengths;
  Real_ptr_ptr m_unpack_buffers;
  Real_ptr_ptr m_recv_buffers;

  Extent make_boundary_extent(
    const message_type msg_type,
    const int (&boundary_offset)[3],
    const Index_type halo_width, const Index_type* grid_dims);

  void create_lists(
      int my_mpi_rank,
      const int* mpi_dims,
      Int_ptr& mpi_ranks,
      Int_ptr& send_tags,
      Int_ptr_ptr& pack_index_lists,
      Index_ptr& pack_index_list_lengths,
      Int_ptr& recv_tags,
      Int_ptr_ptr& unpack_index_lists,
      Index_ptr& unpack_index_list_lengths,
      const Index_type halo_width, const Index_type* grid_dims,
      const Index_type num_neighbors,
      VariantID vid);

  void destroy_lists(
      Int_ptr_ptr& pack_index_lists,
      Int_ptr_ptr& unpack_index_lists,
      const Index_type num_neighbors,
      VariantID vid);

  void create_buffers(
      Index_ptr const& index_list_lengths,
      Real_ptr_ptr& our_buffers,
      Real_ptr_ptr& mpi_buffers,
      const Index_type num_neighbors,
      const Index_type num_vars,
      VariantID vid);

  void destroy_buffers(
      Real_ptr_ptr& our_buffers,
      Real_ptr_ptr& mpi_buffers,
      const Index_type num_neighbors,
      VariantID vid);
};

} // end namespace comm
} // end namespace rajaperf

#endif // closing endif for header file include guard
