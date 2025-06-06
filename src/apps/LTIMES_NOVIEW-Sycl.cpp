//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES_NOVIEW.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_SYCL)

#include <iostream>

#include "common/SyclDataUtils.hpp"

namespace rajaperf 
{
namespace apps
{

//
// Define work-group shape for SYCL execution
//
#define m_wg_sz (32)
#define g_wg_sz (integer::greater_of_squarest_factor_pair(work_group_size/m_wg_sz))
#define z_wg_sz (integer::lesser_of_squarest_factor_pair(work_group_size/m_wg_sz))

template <size_t work_group_size >
void LTIMES_NOVIEW::runSyclVariantImpl(VariantID vid, size_t tune_idx)
{
  const Index_type run_reps = getRunReps();

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  LTIMES_NOVIEW_DATA_SETUP;

  if ( vid == Base_SYCL ) {

    sycl::range<3> global_dim(z_wg_sz * RAJA_DIVIDE_CEILING_INT(num_z, z_wg_sz),
                              g_wg_sz * RAJA_DIVIDE_CEILING_INT(num_g, g_wg_sz),
                              m_wg_sz * RAJA_DIVIDE_CEILING_INT(num_m, m_wg_sz));
    sycl::range<3> wkgroup_dim(z_wg_sz, g_wg_sz, m_wg_sz);

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      qu->submit([&] (sycl::handler& h) {
        h.parallel_for(sycl::nd_range<3> ( global_dim, wkgroup_dim),
                       [=] (sycl::nd_item<3> item) {

          Index_type m = item.get_global_id(2);
          Index_type g = item.get_global_id(1);
          Index_type z = item.get_global_id(0);

          if (m < num_m && g < num_g && z < num_z) {
            for (Index_type d = 0; d < num_d; ++d) {
              LTIMES_NOVIEW_BODY;
            } 
          }

        });
      });

    }
    stopTimer();

  } else if ( vid == RAJA_SYCL ) {

    if (tune_idx == 0) {

      using EXEC_POL =
        RAJA::KernelPolicy<
          RAJA::statement::SyclKernelAsync<
            RAJA::statement::For<1, RAJA::sycl_global_0<z_wg_sz>,      //z
              RAJA::statement::For<2, RAJA::sycl_global_1<g_wg_sz>,    //g
                RAJA::statement::For<3, RAJA::sycl_global_2<m_wg_sz>,  //m
                  RAJA::statement::For<0, RAJA::seq_exec,              //d
                    RAJA::statement::Lambda<0>
                  >
                >
              >
            >
          >
        >;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::kernel_resource<EXEC_POL>(
          RAJA::make_tuple(RAJA::RangeSegment(0, num_d),
                           RAJA::RangeSegment(0, num_z),
                           RAJA::RangeSegment(0, num_g),
                           RAJA::RangeSegment(0, num_m)),
          res,
          [=] (Index_type d, Index_type z, Index_type g, Index_type m) {
          LTIMES_NOVIEW_BODY;
        });

      }
      stopTimer();

    } else if (tune_idx == 1) {

      constexpr bool async = true;

      using launch_policy = RAJA::LaunchPolicy<RAJA::sycl_launch_t<async>>;

      using z_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_0>;

      using g_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_1>;

      using m_policy = RAJA::LoopPolicy<RAJA::sycl_global_item_2>;

      using d_policy = RAJA::LoopPolicy<RAJA::seq_exec>;

      const size_t z_grid_sz = RAJA_DIVIDE_CEILING_INT(num_z, z_wg_sz);

      const size_t g_grid_sz = RAJA_DIVIDE_CEILING_INT(num_g, g_wg_sz);

      const size_t m_grid_sz = RAJA_DIVIDE_CEILING_INT(num_m, m_wg_sz);

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

        RAJA::launch<launch_policy>( res,
            RAJA::LaunchParams(RAJA::Teams(m_grid_sz, g_grid_sz, z_grid_sz),
                               RAJA::Threads(m_wg_sz, g_wg_sz, z_wg_sz)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<z_policy>(ctx, RAJA::RangeSegment(0, num_z),
                [&](Index_type z) {
                  RAJA::loop<g_policy>(ctx, RAJA::RangeSegment(0, num_g),
                    [&](Index_type g) {
                      RAJA::loop<m_policy>(ctx, RAJA::RangeSegment(0, num_m),
                        [&](Index_type m) {
                          RAJA::loop<d_policy>(ctx, RAJA::RangeSegment(0, num_d),
                            [&](Index_type d) {
                              LTIMES_NOVIEW_BODY
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
     std::cout << "\n LTIMES_NOVIEW : Unknown Sycl variant id = " << vid << std::endl;
  }
}

void LTIMES_NOVIEW::runSyclVariant(VariantID vid, size_t tune_idx)
{
  size_t t = 0;

  seq_for(gpu_block_sizes_type{}, [&](auto work_group_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(work_group_size)) {

      if (vid == RAJA_SYCL) {

        if (tune_idx == t) {
          setBlockSize(work_group_size);
          runSyclVariantImpl<work_group_size>(vid, 0);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(work_group_size);
          runSyclVariantImpl<work_group_size>(vid, 1);

        }

        t += 1;

      } else {

        if (tune_idx == t) {
          setBlockSize(work_group_size);
          runSyclVariantImpl<work_group_size>(vid, 0);

        }

        t += 1;
      }

    }

  });
}

void LTIMES_NOVIEW::setSyclTuningDefinitions(VariantID vid)
{

  seq_for(gpu_block_sizes_type{}, [&](auto work_group_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(work_group_size)) {

      if (vid == RAJA_SYCL) {
        addVariantTuningName(vid, "kernel_"+std::to_string(work_group_size));
        addVariantTuningName(vid, "launch_"+std::to_string(work_group_size));
      } else {
        addVariantTuningName(vid, "block_"+std::to_string(work_group_size));
      }

    }

  });

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_SYCL
