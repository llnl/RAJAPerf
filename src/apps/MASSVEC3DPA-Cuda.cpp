//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_BLOCKDIM_LOOP_INC(const Real_ptr B,
                                   const Real_ptr D, const Real_ptr X,
                                   Real_ptr Y)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D(q, d, mvpa::Q1D, mvpa::D1D) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, dy, dz, mvpa::Q1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, dz, mvpa::Q1D, mvpa::Q1D, mvpa::D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, qz, mvpa::Q1D, mvpa::Q1D, mvpa::Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, qy, qz, mvpa::D1D, mvpa::Q1D, mvpa::Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, qz, mvpa::D1D, mvpa::D1D, mvpa::Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_ARGUMENT_LOOP_INC(const Real_ptr B,
                                   const Real_ptr D, const Real_ptr X,
                                   Real_ptr Y,
                                   const Index_type runtime_block_size)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, mvpa::Q1D, mvpa::D1D, runtime_block_size) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D,
                           runtime_block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, mvpa::Q1D, mvpa::D1D, mvpa::D1D,
                           runtime_block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, mvpa::Q1D, mvpa::Q1D, mvpa::D1D,
                           runtime_block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, mvpa::Q1D, mvpa::Q1D, mvpa::Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, mvpa::D1D, mvpa::Q1D, mvpa::Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, mvpa::D1D, mvpa::D1D, mvpa::Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D,
                           runtime_block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_COMPILE_LOOP_INC(const Real_ptr B,
                                  const Real_ptr D, const Real_ptr X,
                                  Real_ptr Y)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, mvpa::Q1D, mvpa::D1D, block_size) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D,
                           block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, mvpa::Q1D, mvpa::D1D, mvpa::D1D,
                           block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, mvpa::Q1D, mvpa::Q1D, mvpa::D1D,
                           block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, mvpa::Q1D, mvpa::Q1D, mvpa::Q1D,
                           block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, mvpa::D1D, mvpa::Q1D, mvpa::Q1D,
                           block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, mvpa::D1D, mvpa::D1D, mvpa::Q1D,
                           block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D,
                           block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_DIRECT(const Real_ptr B,
                        const Real_ptr D, const Real_ptr X,
                        Real_ptr Y)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_DIRECT_2D(q, d, mvpa::Q1D, mvpa::D1D) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_DIRECT_3D(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, dy, dz, mvpa::Q1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, dz, mvpa::Q1D, mvpa::Q1D, mvpa::D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, qz, mvpa::Q1D, mvpa::Q1D, mvpa::Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, qy, qz, mvpa::D1D, mvpa::Q1D, mvpa::Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, qz, mvpa::D1D, mvpa::D1D, mvpa::Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, dz, mvpa::D1D, mvpa::D1D, mvpa::D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template<typename inner_x, typename inner_y, typename inner_z, typename RESOURCE>
void MASSVEC3DPA::runRAJAImpl(RESOURCE &res)
{

  MASSVEC3DPA_DATA_SETUP;

  constexpr bool async = true;

  using launch_policy = RAJA::LaunchPolicy<
  RAJA::cuda_launch_t<async, mvpa::Q1D * mvpa::Q1D * mvpa::Q1D>>;

  using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

  //clang-format off
  RAJA::launch<launch_policy>(
    res,
    RAJA::LaunchParams(RAJA::Teams(NE),
    RAJA::Threads(mvpa::Q1D, mvpa::Q1D, mvpa::Q1D)),
    [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

      RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
        [&](Index_type e) {

          MASSVEC3DPA_0_GPU

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
            [&](Index_type) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                [&](Index_type d) {

                RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                  [&](Index_type q) {
                    MASSVEC3DPA_1;
                  } // lambda (q)
                ); // RAJA::loop<inner_x>
                } // lambda (d)
              ); // RAJA::loop<inner_y>
            } // lambda ()
          ); // RAJA::loop<inner_z>

          for (Index_type c = 0; c < 3; ++c) {

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
            [&](Index_type dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                    [&](Index_type dx) {
                      MASSVEC3DPA_2;
                    } // lambda (dx)
                  ); // RAJA::loop<inner_x>
                } // lambda (dy)
              ); // RAJA::loop<inner_y>
            } // lambda (dz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
            [&](Index_type dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                    [&](Index_type qx) {
                      MASSVEC3DPA_3;
                    } // lambda (qx)
                  ); // RAJA::loop<inner_x>
                } // lambda (dy)
              ); // RAJA::loop<inner_y>
            } // lambda (dz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
            [&](Index_type dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                    [&](Index_type qx) {
                      MASSVEC3DPA_4;
                    } // lambda (qx)
                  ); // RAJA::loop<inner_x>
                } // lambda (qy)
              ); // RAJA::loop<inner_y>
            } // lambda (dz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
            [&](Index_type qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                  [&](Index_type qx) {
                  MASSVEC3DPA_5;
                  } // lambda (qx)
                  ); // RAJA::loop<inner_x>
                } // lambda (qy)
              ); // RAJA::loop<inner_y>
            } // lambda (qz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
            [&](Index_type qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
                [&](Index_type qy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                    [&](Index_type dx) {
                      MASSVEC3DPA_6;
                    } // lambda (dx)
                  ); // RAJA::loop<inner_x>
                } // lambda (qy)
              ); // RAJA::loop<inner_y>
            } // lambda (qz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::Q1D),
            [&](Index_type qz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                    [&](Index_type dx) {
                    MASSVEC3DPA_7;
                    } // lambda (dx)
                  ); // RAJA::loop<inner_x>
                } // lambda (dy)
              ); // RAJA::loop<inner_y>
            } // lambda (qz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
            [&](Index_type dz) {
              RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                [&](Index_type dy) {
                  RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, mvpa::D1D),
                  [&](Index_type dx) {
                  MASSVEC3DPA_8;
                  } // lambda (dx)
                  ); // RAJA::loop<inner_x>
                } // lambda (dy)
              ); // RAJA::loop<inner_y>
            } // lambda (dz)
          ); // RAJA::loop<inner_z>

          ctx.teamSync();

          } // c - dim loop
        }   // lambda (e)
      );      // RAJA::loop<outer_x>
    }         // outer lambda (ctx)
  );            // RAJA::launch
  //clang-format on

}

template <size_t block_size, size_t tune_idx>
void MASSVEC3DPA::runCudaVariantImpl(VariantID vid)
{

  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getCudaResource()};

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_CUDA: {

    if constexpr (tune_idx == 0) {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        dim3 nthreads_per_block(mvpa::Q1D, mvpa::Q1D, mvpa::Q1D);
        constexpr size_t shmem = 0;

        RPlaunchCudaKernel((MassVec3DPA_BLOCKDIM_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, D,
                           X, Y);
      }
      stopTimer();

      // Loop constants
    } else if constexpr (tune_idx == 1) {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        dim3 nthreads_per_block(mvpa::Q1D, mvpa::Q1D, mvpa::Q1D);
        constexpr size_t shmem = 0;

        RPlaunchCudaKernel((MassVec3DPA_ARGUMENT_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, D,
                           X, Y, static_cast<Index_type>(mvpa::Q1D));
      }
      stopTimer();

    } else if constexpr (tune_idx == 2) {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        dim3 nthreads_per_block(mvpa::Q1D, mvpa::Q1D, mvpa::Q1D);
        constexpr size_t shmem = 0;

        RPlaunchCudaKernel((MassVec3DPA_COMPILE_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, D,
                           X, Y);
      }
      stopTimer();

    } else if constexpr (tune_idx == 3) {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        dim3 nthreads_per_block(mvpa::Q1D, mvpa::Q1D, mvpa::Q1D);
        constexpr size_t shmem = 0;

        RPlaunchCudaKernel((MassVec3DPA_DIRECT<block_size>), NE,
                          nthreads_per_block, shmem, res.get_stream(), B, D,
                          X, Y);
      }
      stopTimer();
    }

    break;
  }

  case RAJA_CUDA: {


    using outer_x = RAJA::LoopPolicy<RAJA::cuda_block_x_direct>;

    if constexpr (tune_idx == 0) {

      using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_x_loop>;

      using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_y_loop>;

      using inner_z = RAJA::LoopPolicy<RAJA::cuda_thread_z_loop>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        runRAJAImpl<inner_x, inner_y, inner_z>(res);

      } // loop over kernel reps
      stopTimer();
    }

    if constexpr (tune_idx == 1) {

      using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_size_x_loop<mvpa::Q1D>>;

      using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_size_y_loop<mvpa::Q1D>>;

      using inner_z = RAJA::LoopPolicy<RAJA::cuda_thread_size_z_loop<mvpa::Q1D>>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        runRAJAImpl<inner_x, inner_y, inner_z>(res);

      } // loop over kernel reps
      stopTimer();
    }

    if constexpr (tune_idx == 2) {

      using inner_x = RAJA::LoopPolicy<RAJA::cuda_thread_x_direct>;

      using inner_y = RAJA::LoopPolicy<RAJA::cuda_thread_y_direct>;

      using inner_z = RAJA::LoopPolicy<RAJA::cuda_thread_z_direct>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

        runRAJAImpl<inner_x, inner_y, inner_z>(res);

      } // loop over kernel reps
      stopTimer();
    }

    break;
  }

  default: {

    getCout() << "\n MASSVEC3DPA : Unknown Cuda variant id = " << vid
              << std::endl;
    break;
  }
  }
}


void MASSVEC3DPA::defineCudaVariantTunings()
{

  for (VariantID vid : {Base_CUDA, RAJA_CUDA}) {

    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {

        if (vid == Base_CUDA) {

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 0>>(
              vid, "BLOCKDIM_LOOP_INC_"+std::to_string(block_size));

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 1>>(
              vid, "ARGUMENT_LOOP_INC_"+std::to_string(block_size));

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 2>>(
              vid, "COMPILE_LOOP_INC_"+std::to_string(block_size));

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 3>>(
              vid, "DIRECT_"+std::to_string(block_size));

        }

        if (vid == RAJA_CUDA) {

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 0>>(
              vid, "BLOCKDIM_LOOP_INC_"+std::to_string(block_size));

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 1>>(
              vid, "COMPILE_LOOP_INC_"+std::to_string(block_size));

          addVariantTuning<&MASSVEC3DPA::runCudaVariantImpl<block_size, 2>>(
              vid, "DIRECT_"+std::to_string(block_size));

        }

      }

    });

  }

}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_CUDA
