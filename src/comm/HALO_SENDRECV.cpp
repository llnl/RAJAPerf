//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_SENDRECV.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_PERFSUITE_ENABLE_MPI)

namespace rajaperf
{
namespace comm
{

HALO_SENDRECV::HALO_SENDRECV(const RunParams& params)
  : HALO_base(rajaperf::Comm_HALO_SENDRECV, params)
{
  setDefaultReps(200);

  m_mpi_size = params.getMPISize();
  m_my_mpi_rank = params.getMPIRank();
  m_mpi_dims = params.getMPI3DDivision();
  m_num_vars = params.getHaloNumVars();

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent);
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N_to_the_two_thirds);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(3);

  setUsesFeature(Forall);
  setUsesFeature(MPI);

  if (params.validMPI3DDivision()) {
    addVariantTunings();
  }
}

void HALO_SENDRECV::setSize(Index_type target_size, Index_type target_reps)
{
  setSize_base(target_size, target_reps);

  m_var_size = m_grid_plus_halo_size ;
  const Size_type halo_size = m_var_size - getActualProblemSize();

  setItsPerRep( 0 );
  setKernelsPerRep( 0 );
  setBytesReadPerRep( 1*sizeof(Real_type) * m_num_vars * halo_size ); // send_buffers (MPI)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_num_vars * halo_size ); // recv_buffers (MPI)
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

HALO_SENDRECV::~HALO_SENDRECV()
{
}

void HALO_SENDRECV::setUp(VariantID vid, size_t tune_idx)
{
  setUp_base(m_my_mpi_rank, m_mpi_dims.data(), m_num_vars, vid, tune_idx);
}

void HALO_SENDRECV::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const bool separate_buffers = (getMPIDataSpace(vid) == DataSpace::Copy);

  for (Index_type l = 0; l < s_num_neighbors; ++l) {
    Index_type buffer_len = m_num_vars * m_unpack_index_list_lengths[l];
    if (separate_buffers) {
      addToChecksum(DataSpace::Host, m_recv_buffers[l], buffer_len);
    } else {
      addToChecksum(getMPIDataSpace(vid), m_recv_buffers[l], buffer_len);
    }
  }
}

void HALO_SENDRECV::tearDown(VariantID vid, size_t tune_idx)
{
  tearDown_base(vid, tune_idx);
}

} // end namespace comm
} // end namespace rajaperf

#endif
