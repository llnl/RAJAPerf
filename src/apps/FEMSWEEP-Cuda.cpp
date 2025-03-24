//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-24, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

template < size_t block_size >
void FEMSWEEP::runCudaVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case RAJA_CUDA : {

      constexpr bool async = true;

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, 128>>;

      using outer_x =
          RAJA::LoopPolicy<RAJA::cuda_global_thread_x>;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

         RAJA::launch<launch_policy>( res,
             RAJA::LaunchParams(RAJA::Teams(na),
                                RAJA::Threads(ng)),
             [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
             RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, na * ng),
               [&](int ag) {
                 FEMSWEEP_KERNEL;
               });  // ag loop
         });  // RAJA Launch

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown variant id = " << vid << std::endl;
    }

  }

}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(FEMSWEEP, Cuda)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
