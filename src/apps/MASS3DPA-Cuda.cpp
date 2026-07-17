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

#include "MASS3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template <Index_type D1D, Index_type Q1D, Index_type TBATCH>
  __launch_bounds__(Q1D * Q1D * TBATCH)
__global__ void Mass3DPA(const Real_ptr B, const Real_ptr Bt,
                         const Real_ptr D, const Real_ptr X, Real_ptr Y,
                         Index_type NE) {
  constexpr Index_type MD1 = D1D;
  constexpr Index_type MQ1 = Q1D;

  const Index_type zbatch = threadIdx.z;
  const Index_type e = blockIdx.x * blockDim.z + zbatch;
  const bool valid_e = e < NE;

  MASS3DPA_GPU_SMEM_DECL(TBATCH)
  MASS3DPA_GPU_SMEM_SLICE(zbatch)

  if (valid_e) {
    GPU_FOREACH_THREAD_INC(dy, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(dx, x, MD1, MQ1){
        MASS3DPA_1
      }
      GPU_FOREACH_THREAD_INC(dx, x, MQ1, MQ1) {
        MASS3DPA_2
      }
    }
  }
  if (threadIdx.z == 0) {
    GPU_FOREACH_THREAD_INC(dy, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(dx, x, MQ1, MQ1) {
        MASS3DPA_2
      }
    }
  }
  __syncthreads();
  if (valid_e) {
    GPU_FOREACH_THREAD_INC(dy, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(qx, x, MQ1, MQ1) {
        MASS3DPA_3
      }
    }
  }
  __syncthreads();
  if (valid_e) {
    GPU_FOREACH_THREAD_INC(qy, y, MQ1, MQ1) {
      GPU_FOREACH_THREAD_INC(qx, x, MQ1, MQ1) {
        MASS3DPA_4
      }
    }
  }
  __syncthreads();
  if (valid_e) {
    GPU_FOREACH_THREAD_INC(qy, y, MQ1, MQ1) {
      GPU_FOREACH_THREAD_INC(qx, x, MQ1, MQ1) {
        MASS3DPA_5
      }
    }
  }

  __syncthreads();
  if (threadIdx.z == 0) {
    GPU_FOREACH_THREAD_INC(d, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(q, x, MQ1, MQ1) {
        MASS3DPA_6
      }
    }
  }

  __syncthreads();
  if (valid_e) {
    GPU_FOREACH_THREAD_INC(qy, y, MQ1, MQ1) {
      GPU_FOREACH_THREAD_INC(dx, x, MD1, MQ1) {
        MASS3DPA_7
      }
    }
  }
  __syncthreads();

  if (valid_e) {
    GPU_FOREACH_THREAD_INC(dy, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(dx, x, MD1, MQ1) {
        MASS3DPA_8
      }
    }
  }

  __syncthreads();
  if (valid_e) {
    GPU_FOREACH_THREAD_INC(dy, y, MD1, MQ1) {
      GPU_FOREACH_THREAD_INC(dx, x, MD1, MQ1) {
        MASS3DPA_9
      }
    }
  }
}

template <Index_type D1D, Index_type Q1D, Index_type TBATCH>
void MASS3DPA::runCudaVariantImpl(VariantID vid) {
  constexpr Index_type MD1 = D1D;
  constexpr Index_type MQ1 = Q1D;
  constexpr size_t block_size = Q1D * Q1D * TBATCH;
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  MASS3DPA_DATA_SETUP;
  const Index_type num_elem_blocks =
      RAJA_DIVIDE_CEILING_INT(NE, static_cast<Index_type>(TBATCH));

  switch (vid) {

  case Base_CUDA: {

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      dim3 nthreads_per_block(MQ1, MQ1, TBATCH);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (Mass3DPA<D1D, Q1D, TBATCH>),
                          num_elem_blocks, nthreads_per_block,
                          shmem, res.get_stream(),
                          B, Bt, D, X, Y, NE );
    }
    stopTimer();

    break;
  }

  case RAJA_CUDA: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<RAJA::cuda_launch_t<async, block_size>>;

    using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

    using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_size_x_loop<MQ1>>;

    using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_size_y_loop<MQ1>>;

    using inner_z = RAJA::LoopPolicy<RAJA::cuda_thread_size_z_direct<TBATCH>>;

    startTimer();
    // Loop counter increment uses macro to quiet C++20 compiler warning
    for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

      //clang-format off
      RAJA::launch<launch_policy>( res,
        RAJA::LaunchParams(RAJA::Teams(num_elem_blocks),
                         RAJA::Threads(MQ1, MQ1, TBATCH)),
        [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

          RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, num_elem_blocks),
            [&](Index_type elem_block) {

              MASS3DPA_GPU_SMEM_DECL(TBATCH)

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                      [&](Index_type dy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                          [&](Index_type dx) {
                            MASS3DPA_1
                          }
                        );  // RAJA::loop<inner_x>

                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                          [&](Index_type dx) {
                            MASS3DPA_2
                          }
                        );  // RAJA::loop<inner_x>
                      }  // lambda (dy)
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                [&](Index_type RAJA_UNUSED_ARG(zbatch)) {
                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type dy) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                        [&](Index_type dx) {
                          MASS3DPA_2
                        }
                      );  // RAJA::loop<inner_x>
                    }
                  );  // RAJA::loop<inner_y>
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                      [&](Index_type dy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                          [&](Index_type qx) {
                            MASS3DPA_3
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                      [&](Index_type qy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                          [&](Index_type qx) {
                            MASS3DPA_4
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                      [&](Index_type qy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                          [&](Index_type qx) {
                            MASS3DPA_5
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                [&](Index_type zbatch) {
                  MASS3DPA_GPU_SMEM_SLICE(zbatch)

                  RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                    [&](Index_type d) {
                      RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MQ1),
                        [&](Index_type q) {
                          MASS3DPA_6
                        }
                      );  // RAJA::loop<inner_x>
                    }
                  );  // RAJA::loop<inner_y>
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MQ1),
                      [&](Index_type qy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                          [&](Index_type dx) {
                            MASS3DPA_7
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                      [&](Index_type dy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                          [&](Index_type dx) {
                            MASS3DPA_8
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

              ctx.teamSync();

              RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, TBATCH),
                [&](Index_type zbatch) {
                  const Index_type e = elem_block * TBATCH + zbatch;
                  const bool valid_e = e < NE;
                  if (valid_e) {
                    MASS3DPA_GPU_SMEM_SLICE(zbatch)

                    RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MD1),
                      [&](Index_type dy) {
                        RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MD1),
                          [&](Index_type dx) {
                            MASS3DPA_9
                          }
                        );  // RAJA::loop<inner_x>
                      }
                    );  // RAJA::loop<inner_y>
                  }
                }
              );  // RAJA::loop<inner_z>

            }  // lambda (elem_block)
          );  // RAJA::loop<outer_x>

        }  // outer lambda (ctx)
      );  // RAJA::launch
      //clang-format on

    }  // loop over kernel reps
    stopTimer();

    break;
  }

  default: {

    getCout() << "\n MASS3DPA : Unknown Cuda variant id = " << vid << std::endl;
    break;
  }
  }
}

void MASS3DPA::defineCudaVariantTunings()
{
  for (VariantID vid : {Base_CUDA, RAJA_CUDA}) {
    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {
      constexpr size_t block_size_value = decltype(block_size)::value;
      constexpr Index_type TBATCH =
          static_cast<Index_type>(block_size_value / (mpa::Q1D * mpa::Q1D));

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size_value)) {
        addVariantTuning<&MASS3DPA::runCudaVariantImpl<
            mpa::D1D, mpa::Q1D, TBATCH>>(
            vid, "block_" + std::to_string(block_size_value) +
                     "_batch_" + std::to_string(TBATCH));
      }
    });
  }
}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
