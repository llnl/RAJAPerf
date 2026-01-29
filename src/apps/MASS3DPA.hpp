//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Element-wise action of a 3D finite element mass matrix via partial assembly
/// and sum factorization
///
/// Based on MFEM's/CEED algorithms.
/// Reference implementation - MFEM-v4.9
/// https://github.com/mfem/mfem/blob/v4.9/fem/integ/bilininteg_mass_kernels.hpp#L809
///
/// for (Index_type e = 0; e < NE; ++e) {
///
///   constexpr Index_type MQ1 = mpa::Q1D;
///   constexpr Index_type MD1 = mpa::D1D;
///   constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;
///   Real_type sDQ[MQ1 * MD1];
///   Real_type(*Bsmem)[MD1] = (Real_type(*)[MD1])sDQ;
///   Real_type(*Btsmem)[MQ1] = (Real_type(*)[MQ1])sDQ;
///   Real_type sm0[MDQ * MDQ * MDQ];
///   Real_type sm1[MDQ * MDQ * MDQ];
///   Real_type(*Xsmem)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;
///   Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;
///   Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;
///   Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;
///   Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;
///   Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;
///
///   for(Index_type dy=0; dy<mpa::D1D; ++dy) {
///     for(Index_type dx=0; dx<mpa::D1D; ++dx) {
///       for (Index_type dz = 0; dz< mpa::D1D; ++dz) {
///         Xsmem[dz][dy][dx] = MPA_X(dx, dy, dz, e);
///       }
///     }
///     for(Index_type dx=0; dx<mpa::Q1D; ++dx) {
///      Bsmem[dx][dy] = MPA_B(dx, dy);
///     }
///   }
///
///   for(Index_type dy=0; dy<mpa::D1D; ++dy) {
///     for(Index_type dx=0; dx<mpa::Q1D; ++dx) {
///       Real_type u[mpa::D1D];
///       for (Index_type dz = 0; dz < mpa::D1D; dz++) {
///           u[dz] = 0;
///       }
///       for (Index_type dx = 0; dx < mpa::D1D; ++dx) {
///         for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///           u[dz] += Xsmem[dz][dy][dx] * Bsmem[qx][dx];
///          }
///       }
///       for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///         DDQ[dz][dy][qx] = u[dz];
///       }
///     }
///   }
///
///   for(Index_type qy=0; qy<mpa::Q1D; ++qy) {
///     for(Index_type qx=0; qx<mpa::Q1D; ++qx) {
///       Real_type u[mpa::D1D];
///       for (Index_type dz = 0; dz < mpa::D1D; dz++) {
///         u[dz] = 0;
///       }
///       for (Index_type dy = 0; dy < mpa::D1D; ++dy) {
///         for (Index_type dz = 0; dz < mpa::D1D; dz++) {
///           u[dz] += DDQ[dz][dy][qx] * Bsmem[qy][dy];
///         }
///       }
///       for (Index_type dz = 0; dz < mpa::D1D; dz++) {
///         DQQ[dz][qy][qx] = u[dz];
///       }
///     }
///   }
///
///   for(Index_type qy=0; qy<mpa::Q1D; ++qy) {
///     for(Index_type qx=0; qx<mpa::Q1D; ++qx) {
///       Real_type u[mpa::Q1D];
///       for (Index_type qz = 0; qz < mpa::Q1D; qz++) {
///         u[qz] = 0;
///       }
///       for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///         for (Index_type qz = 0; qz < mpa::Q1D; qz++) {
///            u[qz] += DQQ[dz][qy][qx] * Bsmem[qz][dz];
///          }
///       }
///       for (Index_type qz = 0; qz < mpa::Q1D; qz++) {
///         QQQ[qz][qy][qx] = u[qz] * MPA_D(qx, qy, qz, e);
///       }
///     }
///   }
///
///   for(Index_type d=0; d<mpa::D1D; ++d) {
///     for(Index_type q=0; q<mpa::Q1D; ++q) {
///       Btsmem[d][q] = MPA_Bt(q, d);
///     }
///   }
///
///   for(Index_type qy=0; qy<mpa::Q1D; ++qy) {
///     for(Index_type dx=0; dx<mpa::D1D; ++dx) {
///       Real_type u[mpa::Q1D];
///       for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///         u[qz] = 0;
///       }
///       for (Index_type qx = 0; qx < mpa::Q1D; ++qx) {
///         for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///           u[qz] += QQQ[qz][qy][qx] * Btsmem[dx][qx];
///         }
///       }
///       for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///          QQD[qz][qy][dx] = u[qz];
///       }
///     }
///   }
///
///   for(Index_type dy=0; dy<mpa::D1D; ++dy) {
///     for(Index_type dx=0; dx<mpa::D1D; ++dx) {
///       Real_type u[mpa::Q1D];
///       for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///          u[qz] = 0;
///       }
///       for (Index_type qy = 0; qy < mpa::Q1D; ++qy) {
///         for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///           u[qz] += QQD[qz][qy][dx] * Btsmem[dy][qy];
///          }
///       }
///       for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///         QDD[qz][dy][dx] = u[qz];
///       }
///     }
///   }
///
///   for(Index_type dy=0; dy<mpa::D1D; ++dy) {
///     for(Index_type dx=0; dx<mpa::D1D; ++dx) {
///       Real_type u[mpa::D1D];
///       for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///        u[dz] = 0;
///       }
///       for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {
///         for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///            u[dz] += QDD[qz][dy][dx] * Btsmem[dz][qz];
///          }
///       }
///       for (Index_type dz = 0; dz < mpa::D1D; ++dz) {
///         MPA_Y(dx, dy, dz, e) += u[dz];
///       }
///     }
///   }
///
/// } // element loop
///

#ifndef RAJAPerf_Apps_MASS3DPA_HPP
#define RAJAPerf_Apps_MASS3DPA_HPP

#define MASS3DPA_DATA_SETUP                                                    \
  Real_ptr B = m_B;                                                            \
  Real_ptr Bt = m_Bt;                                                          \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace mpa {
constexpr RAJA::Index_type D1D = 4;
constexpr RAJA::Index_type Q1D = 5;
} // namespace mpa
#define MPA_B(x, y) B[x + mpa::Q1D * y]
#define MPA_Bt(x, y) Bt[x + mpa::D1D * y]
#define MPA_X(dx, dy, dz, e)                                                   \
  X[dx + mpa::D1D * dy + mpa::D1D * mpa::D1D * dz +                            \
    mpa::D1D * mpa::D1D * mpa::D1D * e]
#define MPA_Y(dx, dy, dz, e)                                                   \
  Y[dx + mpa::D1D * dy + mpa::D1D * mpa::D1D * dz +                            \
    mpa::D1D * mpa::D1D * mpa::D1D * e]
#define MPA_D(qx, qy, qz, e)                                                   \
  D[qx + mpa::Q1D * qy + mpa::Q1D * mpa::Q1D * qz +                            \
    mpa::Q1D * mpa::Q1D * mpa::Q1D * e]

#define MASS3DPA_0_CPU                                                         \
  constexpr Index_type MQ1 = mpa::Q1D;                                         \
  constexpr Index_type MD1 = mpa::D1D;                                         \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
  Real_type sDQ[MQ1 * MD1];                                                    \
  Real_type(*Bsmem)[MD1] = (Real_type(*)[MD1])sDQ;                             \
  Real_type(*Btsmem)[MQ1] = (Real_type(*)[MQ1])sDQ;                            \
  Real_type sm0[MDQ * MDQ * MDQ];                                              \
  Real_type sm1[MDQ * MDQ * MDQ];                                              \
  Real_type(*Xsmem)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                   \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

#define MASS3DPA_0_GPU                                                         \
  constexpr Index_type MQ1 = mpa::Q1D;                                         \
  constexpr Index_type MD1 = mpa::D1D;                                         \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
  RAJA_TEAM_SHARED Real_type sDQ[MQ1 * MD1];                                   \
  Real_type(*Bsmem)[MD1] = (Real_type(*)[MD1])sDQ;                             \
  Real_type(*Btsmem)[MQ1] = (Real_type(*)[MQ1])sDQ;                            \
  RAJA_TEAM_SHARED Real_type sm0[MDQ * MDQ * MDQ];                             \
  RAJA_TEAM_SHARED Real_type sm1[MDQ * MDQ * MDQ];                             \
  Real_type(*Xsmem)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                   \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

#define MASS3DPA_1                                                             \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                               \
    Xsmem[dz][dy][dx] = MPA_X(dx, dy, dz, e);                                  \
  }

#define MASS3DPA_2 Bsmem[dx][dy] = MPA_B(dx, dy);

// 2 * mpa::D1D * mpa::D1D * mpa::D1D * mpa::Q1D
#define MASS3DPA_3                                                             \
  Real_type u[mpa::D1D];                                                       \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; dz++) {                               \
    u[dz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dx = 0; dx < mpa::D1D; ++dx) {                               \
    RAJAPERF_UNROLL(MD1)                                                       \
    for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                             \
      u[dz] += Xsmem[dz][dy][dx] * Bsmem[qx][dx];                              \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                               \
    DDQ[dz][dy][qx] = u[dz];                                                   \
  }

// 2 * mpa::D1D * mpa::D1D * mpa::Q1D * mpa::Q1D
#define MASS3DPA_4                                                             \
  Real_type u[mpa::D1D];                                                       \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; dz++) {                               \
    u[dz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dy = 0; dy < mpa::D1D; ++dy) {                               \
    RAJAPERF_UNROLL(MD1)                                                       \
    for (Index_type dz = 0; dz < mpa::D1D; dz++) {                             \
      u[dz] += DDQ[dz][dy][qx] * Bsmem[qy][dy];                                \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; dz++) {                               \
    DQQ[dz][qy][qx] = u[dz];                                                   \
  }

// 2 * mpa::D1D * mpa::Q1D * mpa::Q1D * mpa::Q1D + mpa::Q1D * mpa::Q1D *
// mpa::Q1D
#define MASS3DPA_5                                                             \
  Real_type u[mpa::Q1D];                                                       \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; qz++) {                               \
    u[qz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                               \
    RAJAPERF_UNROLL(MQ1)                                                       \
    for (Index_type qz = 0; qz < mpa::Q1D; qz++) {                             \
      u[qz] += DQQ[dz][qy][qx] * Bsmem[qz][dz];                                \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; qz++) {                               \
    QQQ[qz][qy][qx] = u[qz] * MPA_D(qx, qy, qz, e);                            \
  }

#define MASS3DPA_6 Btsmem[d][q] = MPA_Bt(q, d);

// 2 * mpa::Q1D * mpa::Q1D * mpa::Q1D * mpa::D1D
#define MASS3DPA_7                                                             \
  Real_type u[mpa::Q1D];                                                       \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                               \
    u[qz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qx = 0; qx < mpa::Q1D; ++qx) {                               \
    RAJAPERF_UNROLL(MQ1)                                                       \
    for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                             \
      u[qz] += QQQ[qz][qy][qx] * Btsmem[dx][qx];                               \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                               \
    QQD[qz][qy][dx] = u[qz];                                                   \
  }

// 2 * mpa::Q1D * mpa::Q1D * mpa::D1D * mpa::D1D
#define MASS3DPA_8                                                             \
  Real_type u[mpa::Q1D];                                                       \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                               \
    u[qz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qy = 0; qy < mpa::Q1D; ++qy) {                               \
    RAJAPERF_UNROLL(MQ1)                                                       \
    for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                             \
      u[qz] += QQD[qz][qy][dx] * Btsmem[dy][qy];                               \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                               \
    QDD[qz][dy][dx] = u[qz];                                                   \
  }

// 2 * mpa::Q1D * mpa::D1D * mpa::D1D * mpa::D1D + mpa::D1D * mpa::D1D *
// mpa::D1D
#define MASS3DPA_9                                                             \
  Real_type u[mpa::D1D];                                                       \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                               \
    u[dz] = 0;                                                                 \
  }                                                                            \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < mpa::Q1D; ++qz) {                               \
    RAJAPERF_UNROLL(MD1)                                                       \
    for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                             \
      u[dz] += QDD[qz][dy][dx] * Btsmem[dz][qz];                               \
    }                                                                          \
  }                                                                            \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < mpa::D1D; ++dz) {                               \
    MPA_Y(dx, dy, dz, e) += u[dz];                                             \
  }

namespace rajaperf {
class RunParams;

namespace apps {

class MASS3DPA : public KernelBase {
public:
  MASS3DPA(const RunParams &params);

  ~MASS3DPA();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);

  template <size_t block_size> void runCudaVariantImpl(VariantID vid);
  template <size_t block_size> void runHipVariantImpl(VariantID vid);
  template <size_t work_group_size> void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = mpa::Q1D * mpa::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_NE;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
