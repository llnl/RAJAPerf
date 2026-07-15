//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
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

template <Index_type D1D, Index_type Q1D, Index_type TBATCH>
void MASSVEC3DPA::runSyclVariantImpl(VariantID vid)
{
  static_assert(TBATCH == 1, "MASSVEC3DPA SYCL does not use z-batching");

  constexpr Index_type MD1 = D1D;
  constexpr Index_type MQ1 = Q1D;
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;
  constexpr size_t work_group_size = MQ1 * MQ1 * MQ1;
  setBlockSize(work_group_size);

  const Index_type run_reps = getRunReps();

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  MASSVEC3DPA_DATA_SETUP;

  const ::sycl::range<3> workGroupSize(MQ1, MQ1, MQ1);
  const ::sycl::range<3> gridSize(MQ1, MQ1, MQ1*NE);

  switch (vid) {

  case Base_SYCL: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      qu->submit([&](::sycl::handler& h) {

        auto smB_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MQ1 * MD1), h);
        auto smBt_vec = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MD1 * MQ1), h);
        auto sm0_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);
        auto sm1_vec  = ::sycl::local_accessor<Real_type, 1>(::sycl::range<1>(MDQ * MDQ * MDQ), h);

        h.parallel_for
          (::sycl::nd_range<3>(gridSize, workGroupSize),
           [=] (::sycl::nd_item<3> itm) {

             const Index_type e = itm.get_group(2);

             Real_ptr smB_arr = smB_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr smBt_arr = smBt_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr sm0 = sm0_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();
             Real_ptr sm1 = sm1_vec.get_multi_ptr<::sycl::access::decorated::yes>().get();

             Real_type(*smB)[MD1] = (Real_type(*)[MD1])smB_arr;
             Real_type(*smBt)[MQ1] = (Real_type(*)[MQ1])smBt_arr;

             Real_type(*smX)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;
             Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;
             Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;
             Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;
             Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;
             Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

             SYCL_SHARED_LOOP_2D(q, d, MQ1, MD1) {
               MASSVEC3DPA_1;
             }

             for (int c = 0; c < 3; ++c) {
               SYCL_SHARED_LOOP_3D(dx, dy, dz, MD1, MD1, MD1) {

                 MASSVEC3DPA_2;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(qx, dy, dz, MQ1, MD1, MD1) {

                 MASSVEC3DPA_3;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(qx, qy, dz, MQ1, MQ1, MD1) {
                 MASSVEC3DPA_4;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(qx, qy, qz, MQ1, MQ1, MQ1) {
                 MASSVEC3DPA_5;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(dx, qy, qz, MD1, MQ1, MQ1) {
                 MASSVEC3DPA_6;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(dx, dy, qz, MD1, MD1, MQ1) {
                 MASSVEC3DPA_7;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

               SYCL_SHARED_LOOP_3D(dx, dy, dz, MD1, MD1, MD1) {
                 MASSVEC3DPA_8;
               }
               itm.barrier(::sycl::access::fence_space::local_space);

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

    using inner_z = RAJA::LoopPolicy<RAJA::sycl_local_0_direct>;

    //Caclulate amount of shared memory needed
    size_t shmem = 0;
    {
      constexpr Index_type no_mats = 2;
      shmem += MQ1 * MD1 * no_mats * MDQ * MDQ * MDQ * sizeof(Real_type);
    }

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      //clang-format off
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(NE),
                           RAJA::Threads(MQ1, MQ1, MQ1), shmem),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
            [&](Index_type e) {

              Real_ptr smB_arr  = ctx.getSharedMemory<Real_type>(MQ1 * MD1);
              Real_ptr smBt_arr = ctx.getSharedMemory<Real_type>(MQ1 * MD1);
              Real_ptr sm0 = ctx.getSharedMemory<Real_type>(MDQ * MDQ * MDQ);
              Real_ptr sm1 = ctx.getSharedMemory<Real_type>(MDQ * MDQ * MDQ);

              Real_type(*smB)[MD1] = (Real_type(*)[MD1])smB_arr;
              Real_type(*smBt)[MQ1] = (Real_type(*)[MQ1])smBt_arr;

              Real_type(*smX)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;
              Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;
              Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;
              Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;
              Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;
              Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;


            //3 loops to remain consistent with the GPU versions
            //Masking out of the z-dimension thread is done with GPU versions
            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1), [&](int ) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1), [&](int d) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1), [&](int q) {
                MASSVEC3DPA_1;
                });
              });
            });

            for (int c = 0; c < 3; ++c) {

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MD1), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1), [&](int dx) {
                  MASSVEC3DPA_2;
                });
              });
            });

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MD1), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qx) {
                  MASSVEC3DPA_3;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MD1), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qx) {
                MASSVEC3DPA_4;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qx) {

                MASSVEC3DPA_5;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1), [&](int dx) {

                MASSVEC3DPA_6;
                });
              });
            });

            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MQ1), [&](int qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1), [&](int dx) {

                MASSVEC3DPA_7;
                });
              });
            });
            ctx.teamSync();

            RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MD1), [&](int dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1), [&](int dy) {
                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1), [&](int dx) {
                  MASSVEC3DPA_8;
                });
              });
            });

            ctx.teamSync();

            } //c - dim loop

            }  // lambda (e)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch
      //clang-format on

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

void MASSVEC3DPA::defineSyclVariantTunings()
{
  for (VariantID vid : {Base_SYCL, RAJA_SYCL}) {
#define MASSVEC3DPA_SYCL_TUNING(name, tuning_name, d1d, q1d)                  \
    {                                                                         \
      constexpr Index_type TBATCH = 1;                                        \
      addVariantTuning<&MASSVEC3DPA::runSyclVariantImpl<                     \
          d1d, q1d, TBATCH>>(vid, tuning_name);                               \
    }
    MASSVEC3DPA_GEOMETRIES(MASSVEC3DPA_SYCL_TUNING)
#undef MASSVEC3DPA_SYCL_TUNING
  }
}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_SYCL
