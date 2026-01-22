//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_base.hpp"

#include "RAJA/RAJA.hpp"

#include <utility>
#include <cmath>
#include <map>

namespace rajaperf
{
namespace comm
{

Index_type HALO_base::s_grid_dims_default[3] {100, 100, 100};

HALO_base::HALO_base(KernelID kid, const RunParams& params)
  : KernelBase(kid, params)
{
  setDefaultProblemSize( s_grid_dims_default[0] *
                         s_grid_dims_default[1] *
                         s_grid_dims_default[2] );

  m_halo_width = params.getHaloWidth();
}

void HALO_base::setSize_base(Index_type target_size, Index_type target_reps)
{
  double cbrt_run_size = std::cbrt(target_size) + std::cbrt(3)-1;

  m_grid_dims[0] = cbrt_run_size;
  m_grid_dims[1] = cbrt_run_size;
  m_grid_dims[2] = cbrt_run_size;

  m_grid_plus_halo_dims[0] = m_grid_dims[0] + 2*m_halo_width;
  m_grid_plus_halo_dims[1] = m_grid_dims[1] + 2*m_halo_width;
  m_grid_plus_halo_dims[2] = m_grid_dims[2] + 2*m_halo_width;
  m_grid_plus_halo_size = m_grid_plus_halo_dims[0] *
                          m_grid_plus_halo_dims[1] *
                          m_grid_plus_halo_dims[2] ;

  setActualProblemSize( m_grid_dims[0] * m_grid_dims[1] * m_grid_dims[1] );
  setRunReps( target_reps );
}

HALO_base::~HALO_base()
{
}

void HALO_base::setUp_base(const int my_mpi_rank, const int* mpi_dims,
                           const Index_type num_vars,
                           VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(DataSpace::Host, m_mpi_ranks, s_num_neighbors, -1);
  allocAndInitDataConst(DataSpace::Host, m_send_tags, s_num_neighbors, -1);
  allocAndInitDataConst(DataSpace::Host, m_pack_index_lists, s_num_neighbors, nullptr);
  allocAndInitDataConst(DataSpace::Host, m_pack_index_list_lengths, s_num_neighbors, 0);
  allocAndInitDataConst(DataSpace::Host, m_recv_tags, s_num_neighbors, -1);
  allocAndInitDataConst(DataSpace::Host, m_unpack_index_lists, s_num_neighbors, nullptr);
  allocAndInitDataConst(DataSpace::Host, m_unpack_index_list_lengths, s_num_neighbors, 0);
  create_lists(my_mpi_rank, mpi_dims, m_mpi_ranks,
      m_send_tags, m_pack_index_lists, m_pack_index_list_lengths,
      m_recv_tags, m_unpack_index_lists, m_unpack_index_list_lengths,
      m_halo_width, m_grid_dims,
      s_num_neighbors, vid);
  create_buffers(m_pack_index_list_lengths, m_pack_buffers, m_send_buffers,
                 s_num_neighbors, num_vars, vid);
  create_buffers(m_unpack_index_list_lengths, m_unpack_buffers, m_recv_buffers,
                 s_num_neighbors, num_vars, vid);
}

void HALO_base::tearDown_base(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  destroy_buffers(m_unpack_buffers, m_recv_buffers,
                  s_num_neighbors, vid);
  destroy_buffers(m_pack_buffers, m_send_buffers,
                  s_num_neighbors, vid);
  destroy_lists(m_pack_index_lists, m_unpack_index_lists, s_num_neighbors, vid);
  deallocData(DataSpace::Host, m_unpack_index_list_lengths);
  deallocData(DataSpace::Host, m_unpack_index_lists);
  deallocData(DataSpace::Host, m_recv_tags);
  deallocData(DataSpace::Host, m_pack_index_list_lengths);
  deallocData(DataSpace::Host, m_pack_index_lists);
  deallocData(DataSpace::Host, m_send_tags);
  deallocData(DataSpace::Host, m_mpi_ranks);
}


const int HALO_base::s_boundary_offsets[HALO_base::s_num_neighbors][3]{

  // faces
  {-1,  0,  0},
  { 1,  0,  0},
  { 0, -1,  0},
  { 0,  1,  0},
  { 0,  0, -1},
  { 0,  0,  1},

  // edges
  {-1, -1,  0},
  {-1,  1,  0},
  { 1, -1,  0},
  { 1,  1,  0},
  {-1,  0, -1},
  {-1,  0,  1},
  { 1,  0, -1},
  { 1,  0,  1},
  { 0, -1, -1},
  { 0, -1,  1},
  { 0,  1, -1},
  { 0,  1,  1},

  // corners
  {-1, -1, -1},
  {-1, -1,  1},
  {-1,  1, -1},
  {-1,  1,  1},
  { 1, -1, -1},
  { 1, -1,  1},
  { 1,  1, -1},
  { 1,  1,  1}

};

HALO_base::Extent HALO_base::make_boundary_extent(
    const HALO_base::message_type msg_type,
    const int (&boundary_offset)[3],
    const Index_type halo_width, const Index_type* grid_dims)
{
  if (msg_type != message_type::send &&
      msg_type != message_type::recv) {
    throw std::runtime_error("make_boundary_extent: Invalid message type");
  }
  auto get_bounds = [&](int offset, Index_type dim_size) {
    std::pair<Index_type, Index_type> bounds;
    switch (offset) {
    case -1:
      if (msg_type == message_type::send) {
        bounds.first  = halo_width;
        bounds.second = halo_width + halo_width;
      } else { // (msg_type == message_type::recv)
        bounds.first  = 0;
        bounds.second = halo_width;
      }
      break;
    case 0:
      bounds.first  = halo_width;
      bounds.second = halo_width + dim_size;
      break;
    case 1:
      if (msg_type == message_type::send) {
        bounds.first  = halo_width + dim_size - halo_width;
        bounds.second = halo_width + dim_size;
      } else { // (msg_type == message_type::recv)
        bounds.first  = halo_width + dim_size;
        bounds.second = halo_width + dim_size + halo_width;
      }
      break;
    default:
      throw std::runtime_error("make_extent: Invalid location");
    }
    return bounds;
  };
  auto x_bounds = get_bounds(boundary_offset[0], grid_dims[0]);
  auto y_bounds = get_bounds(boundary_offset[1], grid_dims[1]);
  auto z_bounds = get_bounds(boundary_offset[2], grid_dims[2]);
  return {x_bounds.first, x_bounds.second,
          y_bounds.first, y_bounds.second,
          z_bounds.first, z_bounds.second};
}


//
// Function to generate mpi decomposition and index lists for packing and unpacking.
//
void HALO_base::create_lists(
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
    VariantID vid)
{
  int my_mpi_idx[3]{-1,-1,-1};
  my_mpi_idx[2] = my_mpi_rank / (mpi_dims[0]*mpi_dims[1]);
  my_mpi_idx[1] = (my_mpi_rank - my_mpi_idx[2]*(mpi_dims[0]*mpi_dims[1])) / mpi_dims[0];
  my_mpi_idx[0] = my_mpi_rank - my_mpi_idx[2]*(mpi_dims[0]*mpi_dims[1]) - my_mpi_idx[1]*mpi_dims[0];

  auto get_boundary_idx = [&](const int (&boundary_offset)[3]) {
    return (boundary_offset[0]+1) + 3*(boundary_offset[1]+1) + 9*(boundary_offset[2]+1);
  };

  std::map<int, int> boundary_idx_to_tag;
  for (Index_type l = 0; l < num_neighbors; ++l) {
    boundary_idx_to_tag[get_boundary_idx(s_boundary_offsets[l])] = l;
  }

  const Index_type grid_i_stride = 1;
  const Index_type grid_j_stride = grid_dims[0] + 2*halo_width;
  const Index_type grid_k_stride = grid_j_stride * (grid_dims[1] + 2*halo_width);

  for (Index_type l = 0; l < num_neighbors; ++l) {

    const int (&boundary_offset)[3] = s_boundary_offsets[l];

    int neighbor_boundary_offset[3]{-1, -1, -1};
    for (int dim = 0; dim < 3; ++dim) {
      neighbor_boundary_offset[dim] = -boundary_offset[dim];
    }

    int neighbor_mpi_idx[3] = {my_mpi_idx[0]+boundary_offset[0],
                               my_mpi_idx[1]+boundary_offset[1],
                               my_mpi_idx[2]+boundary_offset[2]};

    // fix neighbor mpi index on periodic boundaries
    for (int dim = 0; dim < 3; ++dim) {
      if (neighbor_mpi_idx[dim] >= mpi_dims[dim]) {
        neighbor_mpi_idx[dim] = 0;
      } else if (neighbor_mpi_idx[dim] < 0) {
        neighbor_mpi_idx[dim] = mpi_dims[dim]-1;
      }
    }

    mpi_ranks[l] = neighbor_mpi_idx[0] + mpi_dims[0]*(neighbor_mpi_idx[1] + mpi_dims[1]*neighbor_mpi_idx[2]);

    {
      // pack and send
      send_tags[l] = boundary_idx_to_tag[get_boundary_idx(boundary_offset)];
      Extent extent = make_boundary_extent(message_type::send,
                                           boundary_offset,
                                           halo_width, grid_dims);

      pack_index_list_lengths[l] = (extent.i_max - extent.i_min) *
                                   (extent.j_max - extent.j_min) *
                                   (extent.k_max - extent.k_min) ;

      auto reset_list = allocAndInitDataForInit(pack_index_lists[l], pack_index_list_lengths[l], vid);

      Int_ptr pack_list = pack_index_lists[l];

      Index_type list_idx = 0;
      for (Index_type kk = extent.k_min; kk < extent.k_max; ++kk) {
        for (Index_type jj = extent.j_min; jj < extent.j_max; ++jj) {
          for (Index_type ii = extent.i_min; ii < extent.i_max; ++ii) {

            Index_type pack_idx = ii * grid_i_stride +
                                  jj * grid_j_stride +
                                  kk * grid_k_stride ;

            pack_list[list_idx] = pack_idx;

            list_idx += 1;
          }
        }
      }
    }

    {
      // receive and unpack
      recv_tags[l] = boundary_idx_to_tag[get_boundary_idx(neighbor_boundary_offset)];
      Extent extent = make_boundary_extent(message_type::recv,
                                           boundary_offset,
                                           halo_width, grid_dims);

      unpack_index_list_lengths[l] = (extent.i_max - extent.i_min) *
                                     (extent.j_max - extent.j_min) *
                                     (extent.k_max - extent.k_min) ;

      auto reset_list = allocAndInitDataForInit(unpack_index_lists[l], unpack_index_list_lengths[l], vid);

      Int_ptr unpack_list = unpack_index_lists[l];

      Index_type list_idx = 0;
      for (Index_type kk = extent.k_min; kk < extent.k_max; ++kk) {
        for (Index_type jj = extent.j_min; jj < extent.j_max; ++jj) {
          for (Index_type ii = extent.i_min; ii < extent.i_max; ++ii) {

            Index_type unpack_idx = ii * grid_i_stride +
                                    jj * grid_j_stride +
                                    kk * grid_k_stride ;

            unpack_list[list_idx] = unpack_idx;

            list_idx += 1;
          }
        }
      }
    }
  }
}

//
// Function to destroy packing and unpacking index lists.
//
void HALO_base::destroy_lists(
    Int_ptr_ptr& pack_index_lists,
    Int_ptr_ptr& unpack_index_lists,
    const Index_type num_neighbors,
    VariantID vid)
{
  for (Index_type l = 0; l < num_neighbors; ++l) {
    deallocData(pack_index_lists[l], vid);
  }
  for (Index_type l = 0; l < num_neighbors; ++l) {
    deallocData(unpack_index_lists[l], vid);
  }
}


void HALO_base::create_buffers(Index_ptr const& index_list_lengths,
                               Real_ptr_ptr& our_buffers,
                               Real_ptr_ptr& mpi_buffers,
                               const Index_type num_neighbors,
                               const Index_type num_vars,
                               VariantID vid)
{
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy);

  Size_type combined_buffer_nbytes = 0;
  for (Index_type l = 0; l < num_neighbors; ++l) {
    Index_type buffer_len = num_vars * index_list_lengths[l];
    Size_type buffer_nbytes = getNBytesPaddedToDataAlignment(buffer_len*sizeof(Real_type));
    combined_buffer_nbytes += buffer_nbytes;
  }

  allocAndInitDataConst(DataSpace::Host, our_buffers, num_neighbors, nullptr);
  allocAndInitDataConst(DataSpace::Host, mpi_buffers, num_neighbors, nullptr);

  if (num_neighbors > 0) {
    if (separate_buffers) {
      allocAndInitData(getDataSpace(vid), our_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
      allocAndInitData(DataSpace::Host, mpi_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
    } else {
      allocAndInitData(getMPIDataSpace(vid), our_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
      mpi_buffers[0] = our_buffers[0];
    }

    for (Index_type l = 1; l < num_neighbors; ++l) {
      Index_type last_buffer_len = num_vars * index_list_lengths[l-1];
      Size_type last_buffer_nbytes = getNBytesPaddedToDataAlignment(last_buffer_len*sizeof(Real_type));
      our_buffers[l] = offsetPointer(our_buffers[l-1], last_buffer_nbytes);
      mpi_buffers[l] = offsetPointer(mpi_buffers[l-1], last_buffer_nbytes);
    }
  }
}

void HALO_base::destroy_buffers(Real_ptr_ptr& our_buffers,
                                Real_ptr_ptr& mpi_buffers,
                                const Index_type RAJA_UNUSED_ARG(num_neighbors),
                                VariantID vid)
{
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy);

  if (mpi_buffers != nullptr) {
    if (separate_buffers) {
      deallocData(DataSpace::Host, mpi_buffers[0]);
      deallocData(getDataSpace(vid), our_buffers[0]);
    } else {
      deallocData(getMPIDataSpace(vid), our_buffers[0]);
    }
  }
  deallocData(DataSpace::Host, mpi_buffers);
  deallocData(DataSpace::Host, our_buffers);
}

} // end namespace comm
} // end namespace rajaperf
