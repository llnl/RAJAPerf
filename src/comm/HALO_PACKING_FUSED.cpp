//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_PACKING_FUSED.hpp"

#include "RAJA/RAJA.hpp"

namespace rajaperf
{
namespace comm
{

HALO_PACKING_FUSED::HALO_PACKING_FUSED(const RunParams& params)
  : HALO_base(rajaperf::Comm_HALO_PACKING_FUSED, params)
{
  setDefaultReps(200);

  m_num_vars = params.getHaloNumVars();

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent);
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N_to_the_two_thirds);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(3);

  setUsesFeature(Workgroup);

  addVariantTunings();
}

void HALO_PACKING_FUSED::setSize(Index_type target_size, Index_type target_reps)
{
  setSize_base(target_size, target_reps);

  m_var_size = m_grid_plus_halo_size ;
  const Size_type halo_size = m_var_size - getActualProblemSize();

  setItsPerRep( 2 * m_num_vars * halo_size );
  setKernelsPerRep( 2 );
  setBytesReadPerRep( 1*sizeof(Int_type) * m_num_vars * halo_size +   // pack_index_lists
                      1*sizeof(Real_type) * m_num_vars * halo_size +  // vars

                      1*sizeof(Int_type) * m_num_vars * halo_size +   // unpack_index_lists
                      1*sizeof(Real_type) * m_num_vars * halo_size ); // unpack_buffers
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_num_vars * halo_size +  // pack_buffers

                         1*sizeof(Real_type) * m_num_vars * halo_size ); // vars
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

HALO_PACKING_FUSED::~HALO_PACKING_FUSED()
{
}

void HALO_PACKING_FUSED::setUp(VariantID vid, size_t tune_idx)
{
  int my_mpi_rank = 0;
  const int mpi_dims[3] = {1,1,1};
  setUp_base(my_mpi_rank, mpi_dims, m_num_vars, vid, tune_idx);

  allocAndInitDataConst(DataSpace::Host, m_vars, m_num_vars, nullptr);
  for (Index_type v = 0; v < m_num_vars; ++v) {
    auto reset_var = allocAndInitDataForInit(m_vars[v], m_var_size, vid);

    Real_ptr var = m_vars[v];

    for (Index_type i = 0; i < m_var_size; i++) {
      var[i] = i + v;
    }
  }
}

void HALO_PACKING_FUSED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  for (Index_type v = 0; v < m_num_vars; ++v) {
    addToChecksum(m_vars[v], m_var_size, vid);
  }

  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy);

  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    Index_type buffer_len = m_num_vars * m_pack_index_list_lengths[l];
    if (separate_buffers) {
      addToChecksum(DataSpace::Host, m_send_buffers[l], buffer_len);
    } else {
      addToChecksum(getMPIDataSpace(vid), m_send_buffers[l], buffer_len);
    }
  }
}

void HALO_PACKING_FUSED::tearDown(VariantID vid, size_t tune_idx)
{
  for (int v = 0; v < m_num_vars; ++v) {
    deallocData(m_vars[v], vid);
  }
  deallocData(DataSpace::Host, m_vars);

  tearDown_base(vid, tune_idx);
}

} // end namespace comm
} // end namespace rajaperf
