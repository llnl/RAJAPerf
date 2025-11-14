//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Uncomment to add compiler directives loop unrolling
//#define USE_RAJAPERF_UNROLL

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_SYCL)

#include "common/SyclDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template < size_t work_group_size >
void MASSVEC3DPA::runSyclVariantImpl(VariantID vid) {
  const Index_type run_reps = getRunReps();

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  MASSVEC3DPA_DATA_SETUP;

  const ::sycl::range<3> workGroupSize(1, MVPA_Q1D, MVPA_Q1D);
  const ::sycl::range<3> gridSize(1, MVPA_Q1D, MVPA_Q1D*NE);

  switch (vid) {

  case Base_SYCL: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      qu->submit([&](::sycl::handler& h) {

        constexpr Index_type MQ1 = MVPA_Q1D;
        constexpr Index_type MD1 = MVPA_D1D;
        constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;

        auto smB_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MQ1 * MD1), h);
        auto smBt_vec = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MD1 * MQ1), h);
        auto sm0_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);
        auto sm1_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);

        h.parallel_for
          (::sycl::nd_range<3>(gridSize, workGroupSize),
           [=] (::sycl::nd_item<3> itm) {

             const Index_type e = itm.get_group(2);

             Real_ptr smB = smB_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr smBt = smBt_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr sm0 = sm0_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr sm1 = sm1_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();

             Real_type(*Bsmem)[MD1] = (Real_type(*)[MD1])sDQ;
             Real_type(*Btsmem)[MQ1] = (Real_type(*)[MQ1])sDQ;

             Real_type(*smX)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;
             Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;
             Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;
             Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;
             Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;
             Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

             SYCL_FOREACH_THREAD(dy, 1, MVPA_D1D) {
               SYCL_FOREACH_THREAD(dx, 2, MVPA_D1D){
                 MASS3DPA_1
               }
               SYCL_FOREACH_THREAD(dx, 2, MVPA_Q1D) {
                 MASS3DPA_2
               }
             }
             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(dy, 1, MVPA_D1D) {
               SYCL_FOREACH_THREAD(qx, 2, MVPA_Q1D) {
                 MASS3DPA_3
               }
             }
             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(qy, 1, MVPA_Q1D) {
               SYCL_FOREACH_THREAD(qx, 2, MVPA_Q1D) {
                 MASS3DPA_4
               }
             }
             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(qy, 1, MVPA_Q1D) {
               SYCL_FOREACH_THREAD(qx, 2, MVPA_Q1D) {
                 MASS3DPA_5
               }
             }

             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(d, 1, MVPA_D1D) {
               SYCL_FOREACH_THREAD(q, 2, MVPA_Q1D) {
                 MASS3DPA_6
               }
             }

             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(qy, 1, MVPA_Q1D) {
               SYCL_FOREACH_THREAD(dx, 2, MVPA_D1D) {
                 MASS3DPA_7
               }
             }
             itm.barrier(::sycl::access::fence_space::local_space);

             SYCL_FOREACH_THREAD(dy, 1, MVPA_D1D) {
               SYCL_FOREACH_THREAD(dx, 2, MVPA_D1D) {
                 MASS3DPA_8
               }
             }

             itm.barrier(::sycl::access::fence_space::local_space);
             SYCL_FOREACH_THREAD(dy, 1, MVPA_D1D) {
               SYCL_FOREACH_THREAD(dx, 2, MVPA_D1D) {
                 MASS3DPA_9
               }
             }

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
      constexpr int MQ1 = MVPA_Q1D;
      constexpr int MD1 = MVPA_D1D;
      constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;

      constexpr int no_mats = 2;
      shmem += MQ1 * MD1 * no_mats * MDQ * MDQ * MDQ * sizeof(double);
    }

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                           RAJA::Threads(MVPA_Q1D, MVPA_Q1D), shmem),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](int e) {


            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASSVEC3DPA : Unknown Sycl variant id = " << vid << std::endl;
    break;
  }
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(MASSVEC3DPA, Sycl)

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_SYCL
