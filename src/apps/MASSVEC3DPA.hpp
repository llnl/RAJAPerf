//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Action of 3D mass matrix via partial assembly on a block vector (3 blocks)
///
/// for (int e = 0; e < NE; ++e) {
///
///  double B[MQ1][MD1];
///  double Bt[MD1][MQ1];
///
///  double sm0[MDQ * MDQ * MDQ];
///  double sm1[MDQ * MDQ * MDQ];
///  double(*X)[MD1][MD1] = (double(*)[MD1][MD1])sm0;
///  double(*DDQ)[MD1][MQ1] = (double(*)[MD1][MQ1])sm1;
///  double(*DQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm0;
///  double(*QQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm1;
///  double(*QQD)[MQ1][MD1] = (double(*)[MQ1][MD1])sm0;
///  double(*QDD)[MD1][MD1] = (double(*)[MD1][MD1])sm1;
///
///  for (int d = 0; d < MVPA_D1D; ++d) {
///    for (int q = 0; q < MVPA_Q1D; ++q) {
///      double basis = b(q, d);
///      B[q][d] = basis;
///      Bt[d][q] = basis;
///    }
///  }
///
///  for (int c = 0; c < 3; ++c) {
///
///    for (int dz = 0; dz < MVPA_D1D; ++dz) {
///      for (int dy = 0; dy < MVPA_D1D; ++dy) {
///        for (int dx = 0; dx < MVPA_D1D; ++dx) {
///
///          smX[dz][dy][dx] = mvpaX_(dx, dy, dz, c, e);
///        }
///      }
///    }
///
///    for (int dz = 0; dz < MVPA_D1D; ++dz) {
///      for (int dy = 0; dy < MVPA_D1D; ++dy) {
///        for (int qx = 0; qx < MVPA_Q1D; ++qx) {
///
///          double u = 0.0;
///          for (int dx = 0; dx < MVPA_D1D; ++dx) {
///            u += X[dz][dy][dx] * B[qx][dx];
///          }
///          DDQ[dz][dy][qx] = u;
///        }
///      }
///    }
///
///    for (int dz = 0; dz < MVPA_D1D; ++dz) {
///      for (int qy = 0; qy < MVPA_Q1D; ++qy) {
///        for (int qx = 0; qx < MVPA_Q1D; ++qx) {
///
///          double u = 0.0;
///          for (int dy = 0; dy < MVPA_D1D; ++dy) {
///            u += DDQ[dz][dy][qx] * B[qy][dy];
///          }
///          DQQ[dz][qy][qx] = u;
///        }
///      }
///    }
///
///    for (int qz = 0; qz < MVPA_Q1D; ++qz) {
///      for (int qy = 0; qy < MVPA_Q1D; ++qy) {
///        for (int qx = 0; qx < MVPA_Q1D; ++qx) {
///
///          double u = 0.0;
///          for (int dz = 0; dz < MVPA_D1D; ++dz) {
///            u += DQQ[dz][qy][qx] * B[qz][dz];
///          }
///          QQQ[qz][qy][qx] = u * D(qx, qy, qz, e);
///        }
///      }
///    }
///
///    for (int qz = 0; qz < MVPA_Q1D; ++qz) {
///      for (int qy = 0; qy < MVPA_Q1D; ++qy) {
///        for (int dx = 0; dx < MVPA_D1D; ++dx) {
///
///          double u = 0.0;
///          for (int qx = 0; qx < MVPA_Q1D; ++qx) {
///            u += QQQ[qz][qy][qx] * Bt[dx][qx];
///          }
///          QQD[qz][qy][dx] = u;
///        }
///      }
///    }
///
///    for (int qz = 0; qz < MVPA_Q1D; ++qz) {
///      for (int dy = 0; dy < MVPA_D1D; ++dy) {
///        for (int dx = 0; dx < MVPA_D1D; ++dx) {
///
///          double u = 0.0;
///          for (int qy = 0; qy < MVPA_Q1D; ++qy) {
///            u += QQD[qz][qy][dx] * Bt[dy][qy];
///          }
///          QDD[qz][dy][dx] = u;
///        }
///      }
///    }
///
///    for (int dz = 0; dz < MVPA_D1D; ++dz) {
///      for (int dy = 0; dy < MVPA_D1D; ++dy) {
///        for (int dx = 0; dx < MVPA_D1D; ++dx) {
///
///          double u = 0.0;
///          for (int qz = 0; qz < MVPA_Q1D; ++qz) {
///            u += QDD[qz][dy][dx] * Bt[dz][qz];
///          }
///          mvpaY_(dx, dy, dz, c, e) = u;
///        }
///      }
///    }
///
///  } // element loop
///

#ifndef RAJAPerf_Apps_MASSVEC3DPA_HPP
#define RAJAPerf_Apps_MASSVEC3DPA_HPP

#define MASSVEC3DPA_DATA_SETUP                                                 \
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
#define MVPA_D1D 3
#define MVPA_Q1D 4
#define MVPA_DIM 3
#define MVPA_B(x, y) B[x + MVPA_Q1D * y]
#define MVPA_Bt(x, y) Bt[x + MVPA_D1D * y]
#define MVPA_X(dx, dy, dz, c, e)                                               \
  X[dx + MVPA_D1D * dy + MVPA_D1D * MVPA_D1D * dz +                            \
    MVPA_D1D * MVPA_D1D * MVPA_D1D * c +                                       \
    MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_DIM * e]
#define MVPA_Y(dx, dy, dz, c, e)                                               \
  Y[dx + MVPA_D1D * dy + MVPA_D1D * MVPA_D1D * dz +                            \
    MVPA_D1D * MVPA_D1D * MVPA_D1D * c +                                       \
    MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_DIM * e]
#define MVPA_D(qx, qy, qz, e)                                                  \
  D[qx + MVPA_Q1D * qy + MVPA_Q1D * MVPA_Q1D * qz +                            \
    MVPA_Q1D * MVPA_Q1D * MVPA_Q1D * e]

#define MASSVEC3DPA_0_CPU                                                      \
  constexpr int MQ1 = MVPA_Q1D;                                                \
  constexpr int MD1 = MVPA_D1D;                                                \
  constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;                                 \
  /*RAJA_TEAM_SHARED*/ Real_type smB[MQ1][MD1];                                \
  /*RAJA_TEAM_SHARED*/ Real_type smBt[MD1][MQ1];                               \
  /*RAJA_TEAM_SHARED*/ Real_type sm0[MDQ * MDQ * MDQ];                         \
  /*RAJA_TEAM_SHARED*/ Real_type sm1[MDQ * MDQ * MDQ];                         \
  Real_type(*smX)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                     \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

#define MASSVEC3DPA_0_GPU                                                      \
  constexpr int MQ1 = MVPA_Q1D;                                                \
  constexpr int MD1 = MVPA_D1D;                                                \
  constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;                                 \
  RAJA_TEAM_SHARED Real_type smB[MQ1][MD1];                                    \
  RAJA_TEAM_SHARED Real_type smBt[MD1][MQ1];                                   \
  RAJA_TEAM_SHARED Real_type sm0[MDQ * MDQ * MDQ];                             \
  RAJA_TEAM_SHARED Real_type sm1[MDQ * MDQ * MDQ];                             \
  Real_type(*smX)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                     \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

#define MASSVEC3DPA_1                                                          \
  Real_type r_smB = MVPA_B(q, d);                                              \
  smB[q][d] = r_smB;                                                           \
  smBt[d][q] = r_smB;

#define MASSVEC3DPA_2 smX[dz][dy][dx] = MVPA_X(dx, dy, dz, c, e);

// 2 * MVPA_D1D * MVPA_Q1D * MVPA_D1D * MVPA_D1D
#define MASSVEC3DPA_3                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dx = 0; dx < MVPA_D1D; ++dx) {                               \
    u += smX[dz][dy][dx] * smB[qx][dx];                                        \
  }                                                                            \
  DDQ[dz][dy][qx] = u;

// 2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_D1D
#define MASSVEC3DPA_4                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dy = 0; dy < MVPA_D1D; ++dy) {                               \
    u += DDQ[dz][dy][qx] * smB[qy][dy];                                        \
  }                                                                            \
  DQQ[dz][qy][qx] = u;

// 2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D + MVPA_Q1D * MVPA_Q1D *
// MVPA_Q1D
#define MASSVEC3DPA_5                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dz = 0; dz < MVPA_D1D; ++dz) {                               \
    u += DQQ[dz][qy][qx] * smB[qz][dz];                                        \
  }                                                                            \
  QQQ[qz][qy][qx] = u * MVPA_D(qx, qy, qz, e);

// 2 * MVPA_Q1D * MVPA_D1D * MVPA_Q1D * MVPA_Q1D
#define MASSVEC3DPA_6                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qx = 0; qx < MVPA_Q1D; ++qx) {                               \
    u += QQQ[qz][qy][qx] * smBt[dx][qx];                                       \
  }                                                                            \
  QQD[qz][qy][dx] = u;

// 2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_Q1D
#define MASSVEC3DPA_7                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qy = 0; qy < MVPA_Q1D; ++qy) {                               \
    u += QQD[qz][qy][dx] * smBt[dy][qy];                                       \
  }                                                                            \
  QDD[qz][dy][dx] = u;

// 2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_D1D
#define MASSVEC3DPA_8                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qz = 0; qz < MVPA_Q1D; ++qz) {                               \
    u += QDD[qz][dy][dx] * smBt[dz][qz];                                       \
  }                                                                            \
  MVPA_Y(dx, dy, dz, c, e) = u;

namespace rajaperf {
class RunParams;

namespace apps {

class MASSVEC3DPA : public KernelBase {
public:
  MASSVEC3DPA(const RunParams &params);

  ~MASSVEC3DPA();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void runSeqVariant(VariantID vid, size_t tune_idx);
  void runOpenMPVariant(VariantID vid, size_t tune_idx);
  void runCudaVariant(VariantID vid, size_t tune_idx);
  void runHipVariant(VariantID vid, size_t tune_idx);
  void runOpenMPTargetVariant(VariantID vid, size_t tune_idx);
  void runSyclVariant(VariantID vid, size_t tune_idx);

  void setCudaTuningDefinitions(VariantID vid);
  void setHipTuningDefinitions(VariantID vid);
  void setSyclTuningDefinitions(VariantID vid);

  template <typename outer_x, typename inner_z, typename inner_y,
            typename inner_x>
  void runRAJATuning();

  template <size_t block_size>
  void runCudaVariantImpl(VariantID vid, size_t tune_idx);
  template <size_t block_size>
  void runHipVariantImpl(VariantID vid, size_t tune_idx);
  template <size_t work_group_size> void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = MVPA_Q1D * MVPA_Q1D * MVPA_Q1D;
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
