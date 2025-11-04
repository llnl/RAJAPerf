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

  setItsPerRep( 2 * m_num_vars * m_halo_size );
  setKernelsPerRep( 2 );

  setBytesAllocatedPerRep( 2*sizeof(Int_type) * m_halo_size + // pack_index_lists, unpack_index_lists
                           2*sizeof(Real_type) * m_num_vars * m_halo_size + // pack_buffers, unpack_buffers (ignore send_buffers, unpack_buffers)
                           1*sizeof(Real_type) * m_num_vars * m_var_size );  // vars
  setBytesReadPerRep( 1*sizeof(Int_type) * m_num_vars * m_halo_size +   // pack_index_lists
                      1*sizeof(Real_type) * m_num_vars * m_halo_size +  // vars

                      1*sizeof(Int_type) * m_num_vars * m_halo_size +   // unpack_index_lists
                      1*sizeof(Real_type) * m_num_vars * m_halo_size ); // unpack_buffers
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_num_vars * m_halo_size +  // pack_buffers

                         1*sizeof(Real_type) * m_num_vars * m_halo_size ); // vars
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


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void HALO_PACKING_FUSED::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    HALO_PACKING_FUSED_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_CODE_WRAPPER(
    HALO_PACKING_FUSED_MANUAL_FUSER_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      Index_type pack_index = 0;
      );

      RAJAPERF_COUNTERS_OUTER_LOOP(for (Index_type l = 0; l < num_neighbors; ++l)) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        Real_ptr buffer = pack_buffers[l];
        Int_ptr list = pack_index_lists[l];
        Index_type len = pack_index_list_lengths[l];
        );
        RAJAPERF_COUNTERS_OUTER_LOOP(for (Index_type v = 0; v < num_vars; ++v)) {
          RAJAPERF_COUNTERS_LOOP_BODY(
          Real_ptr var = vars[v];
          pack_ptr_holders[pack_index] = ptr_holder{buffer, list, var};
          pack_lens[pack_index]        = len;
          pack_index += 1;
          buffer += len;
          );
        }
      }
      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = 0; j < pack_index; j++)) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        ptr_holder pack_ptrs = pack_ptr_holders[j];
        Real_ptr   buffer = pack_ptrs.buffer;
        Int_ptr    list   = pack_ptrs.list;
        Real_ptr   var    = pack_ptrs.var;
        Index_type len    = pack_lens[j];
        );
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < len; i++)) {
          RAJAPERF_COUNTERS_LOOP_BODY(HALO_PACK_BODY);
        }
      }
      RAJAPERF_COUNTERS_IF(if (separate_buffers)) {
        RAJAPERF_COUNTERS_OUTER_LOOP(for (Index_type l = 0; l < num_neighbors; ++l)) {
          RAJAPERF_COUNTERS_LOOP_BODY(
          Index_type len = pack_index_list_lengths[l];
          Real_ptr send_buffer = send_buffers[l];
          Real_ptr buffer = pack_buffers[l];
          );
          RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < len*num_vars; i++)) {
            RAJAPERF_COUNTERS_LOOP_BODY(send_buffer[i] = buffer[i]);
          }
        }
      }
      RAJAPERF_COUNTERS_PAR_SYNC();

      RAJAPERF_COUNTERS_CODE_WRAPPER(
      Index_type unpack_index = 0;
      );

      RAJAPERF_COUNTERS_OUTER_LOOP(for (Index_type l = 0; l < num_neighbors; ++l)) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        Real_ptr buffer = unpack_buffers[l];
        Int_ptr list = unpack_index_lists[l];
        Index_type len = unpack_index_list_lengths[l];
        );
        RAJAPERF_COUNTERS_IF(if (separate_buffers)) {
          RAJAPERF_COUNTERS_LOOP_BODY(
          Real_ptr recv_buffer = recv_buffers[l];
          );
          RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < len*num_vars; i++)) {
            RAJAPERF_COUNTERS_LOOP_BODY(buffer[i] = recv_buffer[i]);
          }
        }

        RAJAPERF_COUNTERS_OUTER_LOOP(for (Index_type v = 0; v < num_vars; ++v)) {
          RAJAPERF_COUNTERS_LOOP_BODY(
          Real_ptr var = vars[v];
          unpack_ptr_holders[unpack_index] = ptr_holder{buffer, list, var};
          unpack_lens[unpack_index]        = len;
          unpack_index += 1;
          buffer += len;
          );
        }
      }
      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type j = 0; j < unpack_index; j++)) {
        RAJAPERF_COUNTERS_LOOP_BODY(
        ptr_holder unpack_ptrs = unpack_ptr_holders[j];
        Real_ptr   buffer = unpack_ptrs.buffer;
        Int_ptr    list   = unpack_ptrs.list;
        Real_ptr   var    = unpack_ptrs.var;
        Index_type len    = unpack_lens[j];
        );
        RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type i = 0; i < len; i++)) {
          RAJAPERF_COUNTERS_LOOP_BODY(HALO_UNPACK_BODY);
        }
      }
      RAJAPERF_COUNTERS_PAR_SYNC();

    }

    RAJAPERF_COUNTERS_CODE_WRAPPER(
    HALO_PACKING_FUSED_MANUAL_FUSER_TEARDOWN;
    );

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, 0);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace comm
} // end namespace rajaperf
