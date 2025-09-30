//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_TARGET_OPENMP)

#include "common/OpenMPTargetDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

using namespace ltimes_idx;

void LTIMES::runOpenMPTargetVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type run_reps = getRunReps();

  LTIMES_DATA_SETUP;

  if ( vid == Base_OpenMPTarget ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      #pragma omp target firstprivate(psi, ell, phi, num_z, num_g, num_m, num_d) device( did )
      #pragma omp teams distribute parallel for schedule(static, 1) collapse(3)
      for (RAJA::Index_type iz = 0; iz < *num_z; ++iz ) {
        for (RAJA::Index_type ig = 0; ig < *num_g; ++ig ) {
          for (RAJA::Index_type im = 0; im < *num_m; ++im ) {
            IZ z(iz);
            IG g(ig);
            IM m(im);
            for (ID d(0); d < num_d; ++d ) {
              LTIMES_BODY;
            }
          }
        }
      }

    }
    stopTimer();

  } else if ( vid == RAJA_OpenMPTarget ) {

    auto res{getOmpTargetResource()};

    using EXEC_POL =
      RAJA::KernelPolicy<
        RAJA::statement::Collapse<RAJA::omp_target_parallel_collapse_exec,
                                  RAJA::ArgList<1, 2, 3>, // z, g, m
          RAJA::statement::For<0, RAJA::seq_exec,         // d
            RAJA::statement::Lambda<0>
          >
        >
      >;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

      RAJA::kernel_resource<EXEC_POL>( RAJA::make_tuple(IDRange(0, *num_d),
                                                        IZRange(0, *num_z),
                                                        IGRange(0, *num_g),
                                                        IMRange(0, *num_m)),
        res,
        [=] (ID d, IZ z, IG g, IM m) {
        LTIMES_BODY;
      });

    }
    stopTimer();

  } else {
     getCout() << "\n LTIMES : Unknown OMP Target variant id = " << vid << std::endl;
  }
}

void LTIMES::setOpenMPTargetTuningDefinitions(VariantID vid)
{

  if (vid == RAJA_OpenMPTarget) {
    addVariantTuningName(vid, "kernel");
  } else {
    addVariantTuningName(vid, "default");
  }

}

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_TARGET_OPENMP
