//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_SYCL)

#include <iostream>

#include "common/SyclDataUtils.hpp"

namespace rajaperf 
{
namespace apps
{

using namespace ltimes_idx;

//
// Define work-group shape for SYCL execution
//
#define g_wg_sz (1)
#define z_wg_sz (1)

template <size_t tune_idx >
void LTIMES::runSyclVariantImpl(VariantID vid)
{
  setBlockSize(m_num_m);

  const Index_type run_reps = getRunReps();

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  LTIMES_DATA_SETUP;

  if ( vid == Base_SYCL ) {

    sycl::range<3> global_dim(*num_z,
                              *num_g,
                              *num_m);
    sycl::range<3> wkgroup_dim(z_wg_sz, g_wg_sz, static_cast<size_t>(*num_m));

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      qu->submit([&] (sycl::handler& h) {
        h.parallel_for(sycl::nd_range<3> ( global_dim, wkgroup_dim),
                       [=] (sycl::nd_item<3> item) {

          IM m(item.get_global_id(2));
          IG g(item.get_global_id(1));
          IZ z(item.get_global_id(0));

          if (m < num_m && g < num_g && z < num_z) {
            for (ID d(0); d < num_d; ++d) {
              LTIMES_BODY;
            }
          }

        });
      });

    }
    stopTimer();

  } else if ( vid == RAJA_SYCL ) {

    if constexpr (tune_idx == 0) {

      using EXEC_POL =
        RAJA::KernelPolicy<
          RAJA::statement::SyclKernelAsync<
            RAJA::statement::For<1, RAJA::sycl_group_0_direct,         //z
              RAJA::statement::For<2, RAJA::sycl_group_1_direct,       //g
                RAJA::statement::For<3, RAJA::sycl_local_2_loop,       //m
                  RAJA::statement::For<0, RAJA::seq_exec,              //d
                    RAJA::statement::Lambda<0>
                  >
                >
              >
            >
          >
        >;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::kernel_resource<EXEC_POL>( 
          RAJA::make_tuple(IDRange(0, *num_d),
                           IZRange(0, *num_z),
                           IGRange(0, *num_g),
                           IMRange(0, *num_m)),
          res,
          [=] (ID d, IZ z, IG g, IM m) {
          LTIMES_BODY;
        });

      }
      stopTimer();

    } else if constexpr (tune_idx == 1) {

      constexpr bool async = true;

      using launch_policy = RAJA::LaunchPolicy<RAJA::sycl_launch_t<async>>;

      using z_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_0>;

      using g_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_1>;

      using m_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_2>;

      using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

      const size_t z_grid_sz = *num_z;

      const size_t g_grid_sz = *num_g;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(RAJA::Teams(1, g_grid_sz, z_grid_sz),
                               RAJA::Threads(*num_m, g_wg_sz, z_wg_sz)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<z_policy>(ctx, IZRange(0, *num_z),
                [&](IZ z) {
                  RAJA::loop<g_policy>(ctx, IGRange(0, *num_g),
                    [&](IG g) {
                      RAJA::loop<m_policy>(ctx, IMRange(0, *num_m),
                        [&](IM m) {
                          RAJA::loop<d_policy>(ctx, IDRange(0, *num_d),
                            [&](ID d) {
                              LTIMES_BODY
                            }
                          ); // RAJA::loop<d_policy>
                        }
                      ); // RAJA::loop<m_policy>
                    }
                  ); // RAJA::loop<g_policy>
                }
              ); // RAJA::loop<z_policy>

            } // outer lambda (ctx)
        );    // RAJA::launch

      } // loop over kernel reps
      stopTimer();
    }

  } else {
     std::cout << "\n LTIMES : Unknown Sycl variant id = " << vid << std::endl;
  }
}


void LTIMES::defineSyclVariantTunings()
{

  for (VariantID vid : {Base_SYCL, RAJA_SYCL}) {

    const size_t m_work_group_size = static_cast<size_t>(m_num_m);

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(m_work_group_size)) {

      if (vid == RAJA_SYCL) {

        addVariantTuning<&LTIMES::runSyclVariantImpl<0>>(
            vid, "kernel_m_"+std::to_string(m_work_group_size));

        addVariantTuning<&LTIMES::runSyclVariantImpl<1>>(
            vid, "launch_m_"+std::to_string(m_work_group_size));

      } else {

        addVariantTuning<&LTIMES::runSyclVariantImpl<0>>(
            vid, "block_m_"+std::to_string(m_work_group_size));

      }

    }

  }

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_SYCL
