//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_EXCHANGE_FUSED.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_PERFSUITE_ENABLE_MPI)

namespace rajaperf
{
namespace comm
{

HALO_EXCHANGE_FUSED::HALO_EXCHANGE_FUSED(const RunParams& params)
  : HALO_base(rajaperf::Comm_HALO_EXCHANGE_FUSED, params)
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

  setUsesFeature(Workgroup);
  setUsesFeature(MPI);

  if (params.validMPI3DDivision()) {
    addVariantTunings();
  }
}

void HALO_EXCHANGE_FUSED::setSize(Index_type target_size, Index_type target_reps)
{
  setSize_base(target_size, target_reps);

  m_var_size = m_grid_plus_halo_size ;
  const Size_type halo_size = m_var_size - getActualProblemSize();

  setItsPerRep( 2 * m_num_vars * halo_size );
  setKernelsPerRep( 2 );
  setBytesReadPerRep( 1*sizeof(Int_type) * m_num_vars * halo_size +   // pack_index_lists
                      1*sizeof(Real_type) * m_num_vars * halo_size +  // vars

                      1*sizeof(Real_type) * m_num_vars * halo_size +  // (pack|send)_buffers (MPI)

                      1*sizeof(Int_type) * m_num_vars * halo_size +   // unpack_index_lists
                      1*sizeof(Real_type) * m_num_vars * halo_size ); // unpack_buffers
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_num_vars * halo_size +  // pack_buffers

                         1*sizeof(Real_type) * m_num_vars * halo_size +  // (recv|unpack)_buffers (MPI)

                         1*sizeof(Real_type) * m_num_vars * halo_size ); // vars
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);
}

HALO_EXCHANGE_FUSED::~HALO_EXCHANGE_FUSED()
{
}

void HALO_EXCHANGE_FUSED::setUp(VariantID vid, size_t tune_idx)
{
  setUp_base(m_my_mpi_rank, m_mpi_dims.data(), m_num_vars, vid, tune_idx);

  allocAndInitDataConst(DataSpace::Host, m_vars, m_num_vars, nullptr);
  for (Index_type v = 0; v < m_num_vars; ++v) {
    auto reset_var = allocAndInitDataForInit(m_vars[v], m_var_size, vid);

    Real_ptr var = m_vars[v];

    for (Index_type i = 0; i < m_var_size; i++) {
      var[i] = i + v;
    }
  }
}

void HALO_EXCHANGE_FUSED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  for (Index_type v = 0; v < m_num_vars; ++v) {
    addToChecksum(m_vars[v], m_var_size, vid);
  }
}

void HALO_EXCHANGE_FUSED::tearDown(VariantID vid, size_t tune_idx)
{
  for (int v = 0; v < m_num_vars; ++v) {
    deallocData(m_vars[v], vid);
  }
  deallocData(DataSpace::Host, m_vars);

  tearDown_base(vid, tune_idx);
}

} // end namespace comm
} // end namespace rajaperf

#endif
