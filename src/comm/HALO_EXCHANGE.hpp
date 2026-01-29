//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// HALO_EXCHANGE kernel reference implementation:
///
/// // post a recv for each neighbor
/// for (Index_type l = 0; l < num_neighbors; ++l) {
///   Index_type len = unpack_index_list_lengths[l];
///   MPI_Irecv(recv_buffers[l], len*num_vars, Real_MPI_type,
///       mpi_ranks[l], recv_tags[l], MPI_COMM_WORLD, &unpack_mpi_requests[l]);
/// }
///
/// // pack a buffer for each neighbor
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
///   // send buffer to neighbor
///   MPI_Isend(send_buffers[l], len*num_vars, Real_MPI_type,
///       mpi_ranks[l], send_tags[l], MPI_COMM_WORLD, &pack_mpi_requests[l]);
/// }
///
/// // unpack a buffer for each neighbor
/// for (Index_type l = 0; l < num_neighbors; ++l) {
///   // receive buffer from neighbor
///   MPI_Wait(&unpack_mpi_requests[l], MPI_STATUS_IGNORE);
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
/// // wait for all sends to complete
/// MPI_Waitall(num_neighbors, pack_mpi_requests.data(), MPI_STATUSES_IGNORE);
///


#ifndef RAJAPerf_Comm_HALO_EXCHANGE_HPP
#define RAJAPerf_Comm_HALO_EXCHANGE_HPP

#define HALO_EXCHANGE_DATA_SETUP \
  HALO_BASE_DATA_SETUP \
  \
  Index_type num_vars = m_num_vars; \
  Real_ptr_ptr vars = m_vars; \
  \
  Int_ptr mpi_ranks = m_mpi_ranks; \
  \
  std::vector<MPI_Request> pack_mpi_requests(num_neighbors); \
  std::vector<MPI_Request> unpack_mpi_requests(num_neighbors); \
  \
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy); \
  \
  Real_ptr_ptr pack_buffers = m_pack_buffers; \
  Real_ptr_ptr unpack_buffers = m_unpack_buffers; \
  \
  Real_ptr_ptr send_buffers = m_send_buffers; \
  Real_ptr_ptr recv_buffers = m_recv_buffers;


#include "HALO_base.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_PERFSUITE_ENABLE_MPI)

#include <vector>
#include <array>

namespace rajaperf
{
namespace comm
{

class HALO_EXCHANGE : public HALO_base
{
public:

  HALO_EXCHANGE(const RunParams& params);

  ~HALO_EXCHANGE();

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
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  int m_mpi_size = -1;
  int m_my_mpi_rank = -1;
  std::array<int, 3> m_mpi_dims = {-1, -1, -1};

  Index_type m_num_vars;
  Index_type m_var_size;

  Real_ptr_ptr m_vars;
};

} // end namespace comm
} // end namespace rajaperf

#endif
#endif // closing endif for header file include guard
