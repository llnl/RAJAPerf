//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_EXCHANGE.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_PERFSUITE_ENABLE_MPI)

namespace rajaperf
{
namespace comm
{

HALO_EXCHANGE::HALO_EXCHANGE(const RunParams& params)
  : HALO_base(rajaperf::Comm_HALO_EXCHANGE, params)
{
  m_mpi_size = params.getMPISize();
  m_my_mpi_rank = params.getMPIRank();
  m_mpi_dims = params.getMPI3DDivision();

  setDefaultReps(200);

  m_num_vars = params.getHaloNumVars();
  m_var_size = m_grid_plus_halo_size ;

  setItsPerRep( m_num_vars * (m_var_size - getActualProblemSize()) );
  setKernelsPerRep( 2 * s_num_neighbors * m_num_vars );
  setBytesReadPerRep( 1*sizeof(Int_type) * getItsPerRep() +   // pack
                      1*sizeof(Real_type) * getItsPerRep() +  // pack

                      1*sizeof(Real_type) * getItsPerRep() +  // send

                      1*sizeof(Int_type) * getItsPerRep() +   // unpack
                      1*sizeof(Real_type) * getItsPerRep() ); // unpack
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getItsPerRep() +  // pack

                         1*sizeof(Real_type) * getItsPerRep() +  // recv

                         1*sizeof(Real_type) * getItsPerRep() ); // unpack
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);

  setComplexity(Complexity::N_to_the_two_thirds);

  setMaxPerfectLoopDimensions(1);
  setMaxLoopDimensions(3);
  setMaxArrayDimensions(1);
  setNumArrays(5); // vars, pack_buffers, unpack_buffers, pack_index_lists, unpack_index_lists

  setUsesFeature(Forall);
  setUsesFeature(MPI);

  if (params.validMPI3DDivision()) {
    setVariantDefined( Base_Seq );
    setVariantDefined( Lambda_Seq );
    setVariantDefined( RAJA_Seq );

    setVariantDefined( Base_OpenMP );
    setVariantDefined( Lambda_OpenMP );
    setVariantDefined( RAJA_OpenMP );

    setVariantDefined( Base_OpenMPTarget );
    setVariantDefined( RAJA_OpenMPTarget );

    setVariantDefined( Base_CUDA );
    setVariantDefined( RAJA_CUDA );

    setVariantDefined( Base_HIP );
    setVariantDefined( RAJA_HIP );
  }
}

HALO_EXCHANGE::~HALO_EXCHANGE()
{
}

void HALO_EXCHANGE::setUp(VariantID vid, size_t tune_idx)
{
  setUp_base(m_my_mpi_rank, m_mpi_dims.data(), m_num_vars, vid, tune_idx);

  m_vars.resize(m_num_vars, nullptr);
  for (Index_type v = 0; v < m_num_vars; ++v) {
    auto reset_var = allocAndInitDataForInit(m_vars[v], m_var_size, vid);

    Real_ptr var = m_vars[v];

    for (Index_type i = 0; i < m_var_size; i++) {
      var[i] = i + v;
    }
  }
}

void HALO_EXCHANGE::updateChecksum(VariantID vid, size_t tune_idx)
{
  for (Real_ptr var : m_vars) {
    checksum[vid][tune_idx] += calcChecksum(var, m_var_size, vid);
  }
}

void HALO_EXCHANGE::tearDown(VariantID vid, size_t tune_idx)
{
  for (int v = 0; v < m_num_vars; ++v) {
    deallocData(m_vars[v], vid);
  }
  m_vars.clear();

  tearDown_base(vid, tune_idx);
}

} // end namespace comm
} // end namespace rajaperf

#endif
