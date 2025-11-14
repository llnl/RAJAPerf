//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_BLOCKDIM_LOOP_INC(const Real_ptr B, const Real_ptr Bt,
                                   const Real_ptr D, const Real_ptr X,
                                   Real_ptr Y)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D(q, d, MVPA_Q1D, MVPA_D1D) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_ARG_LOOP_INC(const Real_ptr B, const Real_ptr Bt,
                              const Real_ptr D, const Real_ptr X,
                              Real_ptr Y,
                              const Index_type runtime_block_size)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, MVPA_Q1D, MVPA_D1D, runtime_block_size) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D,
                           runtime_block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D,
                           runtime_block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D,
                           runtime_block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D,
                           runtime_block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D,
                           runtime_block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_COMPILE_LOOP_INC(const Real_ptr B, const Real_ptr Bt,
                                  const Real_ptr D, const Real_ptr X,
                                  Real_ptr Y)
{

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_LOOP_2D_INC(q, d, MVPA_Q1D, MVPA_D1D, block_size) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D,
                           block_size) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D,
                           block_size) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D,
                           block_size) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D,
                           block_size) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D,
                           block_size) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D,
                           block_size) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_LOOP_3D_INC(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D,
                           block_size) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
__launch_bounds__(block_size) __global__
void MassVec3DPA_DIRECT(const Real_ptr B, const Real_ptr Bt,
                        const Real_ptr D, const Real_ptr X, Real_ptr Y) {

  const Index_type e = blockIdx.x;

  MASSVEC3DPA_0_GPU;

  GPU_SHARED_DIRECT_2D(q, d, MVPA_Q1D, MVPA_D1D) {
    MASSVEC3DPA_1;
  }

  for (Index_type c = 0; c < 3; ++c) {
    GPU_SHARED_DIRECT_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_2;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, dy, dz, MVPA_Q1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_3;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, dz, MVPA_Q1D, MVPA_Q1D, MVPA_D1D) {
      MASSVEC3DPA_4;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(qx, qy, qz, MVPA_Q1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_5;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, qy, qz, MVPA_D1D, MVPA_Q1D, MVPA_Q1D) {
      MASSVEC3DPA_6;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, qz, MVPA_D1D, MVPA_D1D, MVPA_Q1D) {
      MASSVEC3DPA_7;
    }
    __syncthreads();

    GPU_SHARED_DIRECT_3D(dx, dy, dz, MVPA_D1D, MVPA_D1D, MVPA_D1D) {
      MASSVEC3DPA_8;
    }
    __syncthreads();

  } // (c) dimension loop
}

template <size_t block_size>
void MASSVEC3DPA::runHipVariantImpl(VariantID vid, size_t tune_idx)
{

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  MASSVEC3DPA_DATA_SETUP;

  switch (vid) {

  case Base_HIP: {

    if (tune_idx == 0) {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
        constexpr size_t shmem = 0;

        RPlaunchHipKernel((MassVec3DPA_BLOCKDIM_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, Bt, D,
                           X, Y);
      }
      stopTimer();

      // Loop constants
    } else if (tune_idx == 1) {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
        constexpr size_t shmem = 0;

        RPlaunchHipKernel((MassVec3DPA_ARG_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, Bt, D,
                           X, Y, static_cast<Index_type>(MVPA_Q1D));
      }
      stopTimer();

    } else if (tune_idx == 2) {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
        constexpr size_t shmem = 0;

        RPlaunchHipKernel((MassVec3DPA_COMPILE_LOOP_INC<block_size>), NE,
                           nthreads_per_block, shmem, res.get_stream(), B, Bt, D,
                           X, Y);
      }
      stopTimer();

    } else if (tune_idx == 3) {

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        dim3 nthreads_per_block(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D);
        constexpr size_t shmem = 0;

        RPlaunchHipKernel((MassVec3DPA_DIRECT<block_size>), NE,
                          nthreads_per_block, shmem, res.get_stream(), B, Bt, D,
                          X, Y);
      }
      stopTimer();
    }

    break;
  }

  case RAJA_HIP: {

    constexpr bool async = true;

    using launch_policy = RAJA::LaunchPolicy<
        RAJA::hip_launch_t<async, MVPA_Q1D * MVPA_Q1D * MVPA_Q1D>>;

    using outer_x = RAJA::LoopPolicy<RAJA::hip_block_x_direct>;

    if (tune_idx == 0) {

      using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_x_loop>;

      using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_y_loop>;

      using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_z_loop>;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        RAJA::launch<launch_policy>(
            res,
            RAJA::LaunchParams(RAJA::Teams(NE),
                               RAJA::Threads(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
                [&](Index_type e) {

                  MASSVEC3DPA_0_GPU

                  RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                    [&](Index_type) {
                      RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type d) {
                          RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type q) {
                              MASSVEC3DPA_1;
                            } // lambda (q)
                          ); // RAJA::loop<inner_x>
                        } // lambda (d)
                      ); // RAJA::loop<inner_y>
                    } // lambda ()
                  ); // RAJA::loop<inner_z>

                    for (Index_type c = 0; c < 3; ++c) {

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_2;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_3;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_4;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_5;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_6;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_7;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
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

      } // loop over kernel reps
      stopTimer();
    }

    if (tune_idx == 1) {

      using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_size_x_loop<MVPA_Q1D>>;

      using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_size_y_loop<MVPA_Q1D>>;

      using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_size_z_loop<MVPA_Q1D>>;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        RAJA::launch<launch_policy>(
            res,
            RAJA::LaunchParams(RAJA::Teams(NE),
                               RAJA::Threads(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {

              RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
                [&](Index_type e) {

                  MASSVEC3DPA_0_GPU

                  RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                    [&](Index_type) {
                      RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type d) {
                          RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type q) {
                              MASSVEC3DPA_1;
                            } // lambda (q)
                          ); // RAJA::loop<inner_x>
                        } // lambda (d)
                      ); // RAJA::loop<inner_y>
                    } // lambda ()
                  ); // RAJA::loop<inner_z>

                    for (Index_type c = 0; c < 3; ++c) {

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_2;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_3;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_4;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_5;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_6;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_7;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
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

      } // loop over kernel reps
      stopTimer();
    }

    if (tune_idx == 2) {

      using inner_x = RAJA::LoopPolicy<RAJA::hip_thread_x_direct>;

      using inner_y = RAJA::LoopPolicy<RAJA::hip_thread_y_direct>;

      using inner_z = RAJA::LoopPolicy<RAJA::hip_thread_z_direct>;

      startTimer();
      for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1) {

        RAJA::launch<launch_policy>(
            res,
            RAJA::LaunchParams(RAJA::Teams(NE),
                               RAJA::Threads(MVPA_Q1D, MVPA_Q1D, MVPA_Q1D)),
            [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
              RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, NE),
                [&](Index_type e) {

                  MASSVEC3DPA_0_GPU

                  RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, 1),
                    [&](Index_type) {
                      RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type d) {
                          RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type q) {
                              MASSVEC3DPA_1;
                            } // lambda (q)
                          ); // RAJA::loop<inner_x>
                        } // lambda (d)
                      ); // RAJA::loop<inner_y>
                    } // lambda ()
                  ); // RAJA::loop<inner_z>

                    for (Index_type c = 0; c < 3; ++c) {

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_2;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_3;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_4;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (dz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                                [&](Index_type qx) {
                                  MASSVEC3DPA_5;
                                } // lambda (qx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                            [&](Index_type qy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_6;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (qy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_Q1D),
                        [&](Index_type qz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                                [&](Index_type dx) {
                                  MASSVEC3DPA_7;
                                } // lambda (dx)
                              ); // RAJA::loop<inner_x>
                            } // lambda (dy)
                          ); // RAJA::loop<inner_y>
                        } // lambda (qz)
                      ); // RAJA::loop<inner_z>

                      ctx.teamSync();

                      RAJA::loop<inner_z>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                        [&](Index_type dz) {
                          RAJA::loop<inner_y>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
                            [&](Index_type dy) {
                              RAJA::loop<inner_x>(ctx, RAJA::RangeSegment(0, MVPA_D1D),
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

      } // loop over kernel reps
      stopTimer();
    }

    break;
  }

  default: {

    getCout() << "\n MASSVEC3DPA : Unknown Hip variant id = " << vid
              << std::endl;
    break;
  }
  }
}

void MASSVEC3DPA::runHipVariant(VariantID vid, size_t tune_idx) {

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {
    setBlockSize(block_size);

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

        runHipVariantImpl<block_size>(vid, tune_idx);
    }

  });
}

void MASSVEC3DPA::setHipTuningDefinitions(VariantID vid) {

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {
    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (vid == Base_HIP) {
        addVariantTuningName(vid, "BLOCKDIM_LOOP_INC");
        addVariantTuningName(vid, "RUNTIME_LOOP_INC");
        addVariantTuningName(vid, "COMPILE_LOOP_INC");
        addVariantTuningName(vid, "DIRECT");
      }

      if (vid == RAJA_HIP) {
        addVariantTuningName(vid, "BLOCKDIM_LOOP_INC");
        addVariantTuningName(vid, "COMPILE_LOOP_INC");
        addVariantTuningName(vid, "DIRECT");
      }

    }
  });
}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_HIP
