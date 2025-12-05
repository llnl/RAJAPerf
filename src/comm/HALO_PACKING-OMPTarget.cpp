//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HALO_PACKING.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_TARGET_OPENMP)

#include "common/OpenMPTargetDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace comm
{

  //
  // Define threads per team for target execution
  //
  const size_t threads_per_team = 256;


void HALO_PACKING::runOpenMPTargetVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();

  HALO_PACKING_DATA_SETUP;

  if ( vid == Base_OpenMPTarget ) {

    startTimer();
    // Awkward expression for loop counter quiets C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

      for (Index_type l = 0; l < num_neighbors; ++l) {
        Real_ptr buffer = pack_buffers[l];
        Int_ptr list = pack_index_lists[l];
        Index_type len = pack_index_list_lengths[l];
        for (Index_type v = 0; v < num_vars; ++v) {
          Real_ptr var = vars[v];
          #pragma omp target is_device_ptr(buffer, list, var) device( did )
          #pragma omp teams distribute parallel for schedule(static, 1)
          for (Index_type i = 0; i < len; i++) {
            HALO_PACK_BODY;
          }
          buffer += len;
        }

        if (separate_buffers) {
          omp_target_memcpy(send_buffers[l], pack_buffers[l],
                            len*num_vars*sizeof(Real_type),
                            0, 0, did, hid);
        }
      }

      for (Index_type l = 0; l < num_neighbors; ++l) {
        Real_ptr buffer = unpack_buffers[l];
        Int_ptr list = unpack_index_lists[l];
        Index_type len = unpack_index_list_lengths[l];
        if (separate_buffers) {
          omp_target_memcpy(unpack_buffers[l], recv_buffers[l],
                            len*num_vars*sizeof(Real_type),
                            0, 0, hid, did);
        }

        for (Index_type v = 0; v < num_vars; ++v) {
          Real_ptr var = vars[v];
          #pragma omp target is_device_ptr(buffer, list, var) device( did )
          #pragma omp teams distribute parallel for schedule(static, 1)
          for (Index_type i = 0; i < len; i++) {
            HALO_UNPACK_BODY;
          }
          buffer += len;
        }
      }

    }
    stopTimer();

  } else if ( vid == RAJA_OpenMPTarget ) {

    auto res{getOmpTargetResource()}; 

    using EXEC_POL = RAJA::omp_target_parallel_for_exec<threads_per_team>;

    startTimer();
    // Awkward expression for loop counter quiets C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; ((irep = irep + 1), 0)) {

      for (Index_type l = 0; l < num_neighbors; ++l) {
        Real_ptr buffer = pack_buffers[l];
        Int_ptr list = pack_index_lists[l];
        Index_type len = pack_index_list_lengths[l];
        for (Index_type v = 0; v < num_vars; ++v) {
          Real_ptr var = vars[v];
          auto halo_packing_pack_base_lam = [=](Index_type i) {
                HALO_PACK_BODY;
              };
          RAJA::forall<EXEC_POL>( res,
              RAJA::TypedRangeSegment<Index_type>(0, len),
              halo_packing_pack_base_lam );
          buffer += len;
        }

        if (separate_buffers) {
          res.memcpy(send_buffers[l], pack_buffers[l], len*num_vars*sizeof(Real_type));
        }
      }

      for (Index_type l = 0; l < num_neighbors; ++l) {
        Real_ptr buffer = unpack_buffers[l];
        Int_ptr list = unpack_index_lists[l];
        Index_type len = unpack_index_list_lengths[l];
        if (separate_buffers) {
          res.memcpy(unpack_buffers[l], recv_buffers[l], len*num_vars*sizeof(Real_type));
        }

        for (Index_type v = 0; v < num_vars; ++v) {
          Real_ptr var = vars[v];
          auto halo_packing_unpack_base_lam = [=](Index_type i) {
                HALO_UNPACK_BODY;
              };
          RAJA::forall<EXEC_POL>( res,
              RAJA::TypedRangeSegment<Index_type>(0, len),
              halo_packing_unpack_base_lam );
          buffer += len;
        }
      }

    }
    stopTimer();

  } else {
     getCout() << "\n HALO_PACKING : Unknown OMP Target variant id = " << vid << std::endl;
  }
}

} // end namespace comm
} // end namespace rajaperf

#endif  // RAJA_ENABLE_TARGET_OPENMP
