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

#if defined(RAJA_ENABLE_SYCL)

#include "common/SyclDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t work_group_size >
void MASS3DPA_ATOMIC::runSyclVariantImpl(VariantID vid) {
  setBlockSize(work_group_size);

  const Index_type run_reps = getRunReps();

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  MASS3DPA_ATOMIC_DATA_SETUP;

  const ::sycl::range<3> workGroupSize(1, MPA_Q1D, MPA_Q1D);
  const ::sycl::range<3> gridSize(1, MPA_Q1D, MPA_Q1D*NE);

  switch (vid) {

  case Base_SYCL: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      qu->submit([&](::sycl::handler& h) {

        constexpr Index_type MQ1 = MPA_Q1D;
        constexpr Index_type MD1 = MPA_D1D;
        constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;

        auto sDQ_vec = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MQ1 * MD1), h);
        auto sm0_vec = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);
        auto sm1_vec = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);

        h.parallel_for
          (::sycl::nd_range<3>(gridSize, workGroupSize),
           [=] (::sycl::nd_item<3> itm) {

             const Index_type e = itm.get_group(2);

           });
      });

    }
    stopTimer();

    break;
  }

  case RAJA_SYCL: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::sycl_launch_t<async>>;

    using outer_x = RAJA::LoopPolicy<RAJA::sycl_group_2_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::sycl_local_2_direct>;

    using inner_y = RAJA::LoopPolicy<RAJA::sycl_local_1_direct>;

    //Caclulate amount of shared memory needed
    size_t shmem = 0;
    {
      constexpr Index_type MQ1 = MPA_Q1D;
      constexpr Index_type MD1 = MPA_D1D;
      constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;

      constexpr Index_type no_mats = 2;
      shmem += MQ1 * MD1 * no_mats * MDQ * MDQ * MDQ * sizeof(Real_type);
    }

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                           RAJA::Threads(MPA_Q1D, MPA_Q1D), shmem),
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

    getCout() << "\n MASS3DPA_ATOMIC : Unknown Sycl variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASS3DPA_ATOMIC, Sycl, Base_SYCL, RAJA_SYCL)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_SYCL
