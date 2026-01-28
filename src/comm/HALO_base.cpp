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

  m_grid_size = m_grid_dims[0] *
                m_grid_dims[1] *
                m_grid_dims[2] ;
  m_grid_plus_halo_size = m_grid_plus_halo_dims[0] *
                          m_grid_plus_halo_dims[1] *
                          m_grid_plus_halo_dims[2] ;
  m_halo_size = m_grid_plus_halo_size - m_grid_size;

  setActualProblemSize( m_grid_size );
  setRunReps( target_reps );

  set_list_sizes();
}

HALO_base::~HALO_base()
{
}

void HALO_base::setUp_base(const int my_mpi_rank, const int* mpi_dims,
                           const Index_type num_vars,
                           VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  create_lists(my_mpi_rank, mpi_dims, vid);
  create_buffers(m_pack_index_list_lengths, m_pack_buffers, m_send_buffers, num_vars, vid);
  create_buffers(m_unpack_index_list_lengths, m_unpack_buffers, m_recv_buffers, num_vars, vid);
}

void HALO_base::tearDown_base(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  destroy_buffers(m_unpack_buffers, m_recv_buffers, vid);
  destroy_buffers(m_pack_buffers, m_send_buffers, vid);
  destroy_lists(vid);
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
// Function to set list extents and sizes for packing and unpacking.
//
void HALO_base::set_list_sizes()
{
  auto get_boundary_idx = [&](const int (&boundary_offset)[3]) {
    return (boundary_offset[0]+1) + 3*(boundary_offset[1]+1) + 9*(boundary_offset[2]+1);
  };

  std::map<int, int> boundary_idx_to_tag;
  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    boundary_idx_to_tag[get_boundary_idx(s_boundary_offsets[l])] = l;
  }

  for (Index_type l = 0; l < s_num_neighbors; ++l) {

    const int (&boundary_offset)[3] = s_boundary_offsets[l];

    int neighbor_boundary_offset[3]{-1, -1, -1};
    for (int dim = 0; dim < 3; ++dim) {
      neighbor_boundary_offset[dim] = -boundary_offset[dim];
    }

    {
      // pack and send
      m_send_tags[l] = boundary_idx_to_tag[get_boundary_idx(boundary_offset)];
      m_pack_extents[l] = make_boundary_extent(message_type::send,
                                               boundary_offset,
                                               m_halo_width, m_grid_dims);

      m_pack_index_list_lengths[l] = (m_pack_extents[l].i_max - m_pack_extents[l].i_min) *
                                     (m_pack_extents[l].j_max - m_pack_extents[l].j_min) *
                                     (m_pack_extents[l].k_max - m_pack_extents[l].k_min) ;
    }

    {
      // receive and unpack
      m_recv_tags[l] = boundary_idx_to_tag[get_boundary_idx(neighbor_boundary_offset)];
      m_unpack_extents[l] = make_boundary_extent(message_type::recv,
                                                 boundary_offset,
                                                 m_halo_width, m_grid_dims);

      m_unpack_index_list_lengths[l] = (m_unpack_extents[l].i_max - m_unpack_extents[l].i_min) *
                                       (m_unpack_extents[l].j_max - m_unpack_extents[l].j_min) *
                                       (m_unpack_extents[l].k_max - m_unpack_extents[l].k_min) ;
    }
  }
}

//
// Function to generate mpi decomposition and index lists for packing and unpacking.
//
void HALO_base::create_lists(
    int my_mpi_rank,
    const int* mpi_dims,
    VariantID vid)
{
  if (do_extra_prints()) {
    getCout() << "HALO_base::create_lists" << std::endl ;
  }
  int my_mpi_idx[3]{-1,-1,-1};
  my_mpi_idx[2] = my_mpi_rank / (mpi_dims[0]*mpi_dims[1]);
  my_mpi_idx[1] = (my_mpi_rank - my_mpi_idx[2]*(mpi_dims[0]*mpi_dims[1])) / mpi_dims[0];
  my_mpi_idx[0] = my_mpi_rank - my_mpi_idx[2]*(mpi_dims[0]*mpi_dims[1]) - my_mpi_idx[1]*mpi_dims[0];

  const Index_type grid_i_stride = 1;
  const Index_type grid_j_stride = m_grid_dims[0] + 2*m_halo_width;
  const Index_type grid_k_stride = grid_j_stride * (m_grid_dims[1] + 2*m_halo_width);

  for (Index_type l = 0; l < s_num_neighbors; ++l) {

    const int (&boundary_offset)[3] = s_boundary_offsets[l];

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

    m_mpi_ranks[l] = neighbor_mpi_idx[0] +
                     mpi_dims[0]*(neighbor_mpi_idx[1] +
                                  mpi_dims[1]*neighbor_mpi_idx[2]);

    {
      // pack and send
      auto reset_pack_list = allocAndInitDataForInit(
          m_pack_index_lists[l], m_pack_index_list_lengths[l], vid);

      Extent pack_extent = m_pack_extents[l];
      Int_ptr pack_list = m_pack_index_lists[l];

      Index_type list_idx = 0;
      for (Index_type kk = pack_extent.k_min; kk < pack_extent.k_max; ++kk) {
        for (Index_type jj = pack_extent.j_min; jj < pack_extent.j_max; ++jj) {
          for (Index_type ii = pack_extent.i_min; ii < pack_extent.i_max; ++ii) {

            Index_type pack_idx = ii * grid_i_stride +
                                  jj * grid_j_stride +
                                  kk * grid_k_stride ;

            pack_list[list_idx] = pack_idx;

            list_idx += 1;
          }
        }
      }

      if (do_extra_prints()) {
        getCout() << "  m_pack_index_lists " << l << " (" << m_pack_index_list_lengths[l] << ", " << list_idx << ") " << m_pack_index_lists[l] << std::endl ;
        for (Index_type i = 0; i < m_pack_index_list_lengths[l]; ++i) {
          getCout() << "    " << m_pack_index_lists[l][i] << std::endl ;
        }
      }
    }

    if (do_extra_prints()) {
      getCout() << "  m_pack_index_lists " << l << " (" << m_pack_index_list_lengths[l] << ") " << m_pack_index_lists[l] << std::endl ;
      for (Index_type i = 0; i < m_pack_index_list_lengths[l]; ++i) {
        getCout() << "    " << m_pack_index_lists[l][i] << std::endl ;
      }
    }

    {
      // receive and unpack
      auto reset_unpack_list = allocAndInitDataForInit(
          m_unpack_index_lists[l], m_unpack_index_list_lengths[l], vid);

      Extent unpack_extent = m_unpack_extents[l];
      Int_ptr unpack_list = m_unpack_index_lists[l];

      Index_type list_idx = 0;
      for (Index_type kk = unpack_extent.k_min; kk < unpack_extent.k_max; ++kk) {
        for (Index_type jj = unpack_extent.j_min; jj < unpack_extent.j_max; ++jj) {
          for (Index_type ii = unpack_extent.i_min; ii < unpack_extent.i_max; ++ii) {

            Index_type unpack_idx = ii * grid_i_stride +
                                    jj * grid_j_stride +
                                    kk * grid_k_stride ;

            unpack_list[list_idx] = unpack_idx;

            list_idx += 1;
          }
        }
      }
      if (do_extra_prints()) {
        getCout() << "  m_unpack_index_lists " << l << " (" << m_unpack_index_list_lengths[l] << ", " << list_idx << ") " << m_unpack_index_lists[l] << std::endl ;
        for (Index_type i = 0; i < m_unpack_index_list_lengths[l]; ++i) {
          getCout() << "    " << m_unpack_index_lists[l][i] << std::endl ;
        }
      }
    }
    if (do_extra_prints()) {
      getCout() << "  m_unpack_index_lists " << l << " (" << m_unpack_index_list_lengths[l] << ") " << m_unpack_index_lists[l] << std::endl ;
      for (Index_type i = 0; i < m_unpack_index_list_lengths[l]; ++i) {
        getCout() << "    " << m_unpack_index_lists[l][i] << std::endl ;
      }
    }
  }
}

//
// Function to destroy packing and unpacking index lists.
//
void HALO_base::destroy_lists(
    VariantID vid)
{
  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    deallocData(m_pack_index_lists[l], vid);
  }
  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    deallocData(m_unpack_index_lists[l], vid);
  }
}


void HALO_base::create_buffers(Index_ptr index_list_lengths,
                               Real_ptr_ptr our_buffers,
                               Real_ptr_ptr mpi_buffers,
                               const Index_type num_vars,
                               VariantID vid)
{
  if (do_extra_prints()) {
    getCout() << "HALO_base::create_buffers" << std::endl;
  }
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy);

  Size_type combined_buffer_nbytes = 0;
  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    Index_type buffer_len = num_vars * index_list_lengths[l];
    Size_type buffer_nbytes = getNBytesPaddedToDataAlignment(buffer_len*sizeof(Real_type));
    combined_buffer_nbytes += buffer_nbytes;
  }

  if (s_num_neighbors > 0) {
    if (separate_buffers) {
      allocAndInitData(getDataSpace(vid), our_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
      allocAndInitData(DataSpace::Host, mpi_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
    } else {
      Index_type len = RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type));
      {
        auto reset_our = allocAndInitDataForInit(getMPIDataSpace(vid),
            our_buffers[0], len);

        if (do_extra_prints()) {
          getCout() << "  our_buffers[0] " << " (" << len << ", " << num_vars * index_list_lengths[0] << ") " << our_buffers[0] << std::endl ;
          for (Index_type i = 0; i < len; ++i) {
            getCout() << "    " << our_buffers[0][i] << std::endl ;
          }
        }
      }
      if (do_extra_prints()) {
        getCout() << "  our_buffers[0] " << " (" << len << ") " << our_buffers[0] << std::endl ;
        for (Index_type i = 0; i < len; ++i) {
          getCout() << "    " << our_buffers[0][i] << std::endl ;
        }
      }
      // allocAndInitData(getMPIDataSpace(vid), our_buffers[0], RAJA_DIVIDE_CEILING_INT(combined_buffer_nbytes, sizeof(Real_type)));
      mpi_buffers[0] = our_buffers[0];
    }

    for (Index_type l = 1; l < s_num_neighbors; ++l) {
      Index_type last_buffer_len = num_vars * index_list_lengths[l-1];
      Size_type last_buffer_nbytes = getNBytesPaddedToDataAlignment(last_buffer_len*sizeof(Real_type));
      our_buffers[l] = offsetPointer(our_buffers[l-1], last_buffer_nbytes);
      mpi_buffers[l] = offsetPointer(mpi_buffers[l-1], last_buffer_nbytes);

      if (do_extra_prints()) {
        getCout() << "  our_buffers " << l << " (" << num_vars * index_list_lengths[l] << ") " << our_buffers[l] << std::endl ;
      }
    }
  }
}

void HALO_base::destroy_buffers(Real_ptr_ptr our_buffers,
                                Real_ptr_ptr mpi_buffers,
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
    for (Index_type l = 1; l < s_num_neighbors; ++l) {
      our_buffers[l] = nullptr;
      mpi_buffers[l] = nullptr;
    }
  }
}

} // end namespace comm
} // end namespace rajaperf
