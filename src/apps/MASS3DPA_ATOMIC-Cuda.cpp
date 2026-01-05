//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASS3DPA_ATOMIC.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t block_size >
  __launch_bounds__(block_size)
__global__ void Mass3DPA(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const Index_type e = blockIdx.x;



}

template < size_t block_size >
void MASS3DPA_ATOMIC::runCudaVariantImpl(VariantID vid) {
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  MASS3DPA_ATOMIC_DATA_SETUP;

  switch (vid) {

  case Base_CUDA: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      dim3 nthreads_per_block(MPA_Q1D, MPA_Q1D, 1);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (Mass3DPA<block_size>),
                          NE, nthreads_per_block,
                          shmem, res.get_stream(),
                          B, Bt, D, X, Y );
    }
    stopTimer();

    break;
  }

  case RAJA_CUDA: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, MPA_Q1D*MPA_Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_size_x_loop<MPA_Q1D>>;

    using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_size_y_loop<MPA_Q1D>>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                         RAJA::Threads(MPA_Q1D, MPA_Q1D, 1)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {



            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASS3DPA_ATOMIC : Unknown Cuda variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASS3DPA_ATOMIC, Cuda, Base_CUDA, RAJA_CUDA)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
