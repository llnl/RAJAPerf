//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Element-wise action of a 3D finite element mass matrix via partial
/// assembly and sum factorization on a block vector
///
/// for (Index_type e = 0; e < NE; ++e) {
///
///  Real_type B[MQ1][MD1];
///
///  Real_type sm0[MDQ * MDQ * MDQ];
///  Real_type sm1[MDQ * MDQ * MDQ];
///  Real_type(*X)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;
///  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;
///  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;
///  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;
///  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;
///  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;
///
///  for (Index_type d = 0; d < mvpa::D1D; ++d) {
///    for (Index_type q = 0; q < mvpa::Q1D; ++q) {
///      Real_type basis = b(q, d);
///      B[q][d] = basis;
///      Bt[d][q] = basis;
///    }
///  }
///
///  for (Index_type c = 0; c < 3; ++c) {
///
///    for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {
///      for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {
///        for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {
///
///          smX[dz][dy][dx] = mvpaX_(dx, dy, dz, c, e);
///        }
///      }
///    }
///
///    for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {
///      for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {
///        for (Index_type qx = 0; qx < mvpa::Q1D; ++qx) {
///
///          Real_type u = 0.0;
///          for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {
///            u += X[dz][dy][dx] * B[qx][dx];
///          }
///          DDQ[dz][dy][qx] = u;
///        }
///      }
///    }
///
///    for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {
///      for (Index_type qy = 0; qy < mvpa::Q1D; ++qy) {
///        for (Index_type qx = 0; qx < mvpa::Q1D; ++qx) {
///
///          Real_type u = 0.0;
///          for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {
///            u += DDQ[dz][dy][qx] * B[qy][dy];
///          }
///          DQQ[dz][qy][qx] = u;
///        }
///      }
///    }
///
///    for (Index_type qz = 0; qz < mvpa::Q1D; ++qz) {
///      for (Index_type qy = 0; qy < mvpa::Q1D; ++qy) {
///        for (Index_type qx = 0; qx < mvpa::Q1D; ++qx) {
///
///          Real_type u = 0.0;
///          for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {
///            u += DQQ[dz][qy][qx] * B[qz][dz];
///          }
///          QQQ[qz][qy][qx] = u * D(qx, qy, qz, e);
///        }
///      }
///    }
///
///    for (Index_type qz = 0; qz < mvpa::Q1D; ++qz) {
///      for (Index_type qy = 0; qy < mvpa::Q1D; ++qy) {
///        for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {
///
///          Real_type u = 0.0;
///          for (Index_type qx = 0; qx < mvpa::Q1D; ++qx) {
///            u += QQQ[qz][qy][qx] * Bt[dx][qx];
///          }
///          QQD[qz][qy][dx] = u;
///        }
///      }
///    }
///
///    for (Index_type qz = 0; qz < mvpa::Q1D; ++qz) {
///      for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {
///        for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {
///
///          Real_type u = 0.0;
///          for (Index_type qy = 0; qy < mvpa::Q1D; ++qy) {
///            u += QQD[qz][qy][dx] * Bt[dy][qy];
///          }
///          QDD[qz][dy][dx] = u;
///        }
///      }
///    }
///
///    for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {
///      for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {
///        for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {
///
///          Real_type u = 0.0;
///          for (Index_type qz = 0; qz < mvpa::Q1D; ++qz) {
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
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace mvpa {
constexpr RAJA::Index_type D1D = 3;
constexpr RAJA::Index_type Q1D = 4;
constexpr RAJA::Index_type DIM = 3;
} // namespace mvpa
#define MVPA_B(x, y) B[x + mvpa::Q1D * y]
#define MVPA_X(dx, dy, dz, c, e)                                               \
  X[dx + mvpa::D1D * dy + mvpa::D1D * mvpa::D1D * dz +                         \
    mvpa::D1D * mvpa::D1D * mvpa::D1D * c +                                    \
    mvpa::D1D * mvpa::D1D * mvpa::D1D * mvpa::DIM * e]
#define MVPA_Y(dx, dy, dz, c, e)                                               \
  Y[dx + mvpa::D1D * dy + mvpa::D1D * mvpa::D1D * dz +                         \
    mvpa::D1D * mvpa::D1D * mvpa::D1D * c +                                    \
    mvpa::D1D * mvpa::D1D * mvpa::D1D * mvpa::DIM * e]
#define MVPA_D(qx, qy, qz, e)                                                  \
  D[qx + mvpa::Q1D * qy + mvpa::Q1D * mvpa::Q1D * qz +                         \
    mvpa::Q1D * mvpa::Q1D * mvpa::Q1D * e]

#define MASSVEC3DPA_0_CPU                                                      \
  constexpr Index_type MQ1 = mvpa::Q1D;                                        \
  constexpr Index_type MD1 = mvpa::D1D;                                        \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
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
  constexpr Index_type MQ1 = mvpa::Q1D;                                        \
  constexpr Index_type MD1 = mvpa::D1D;                                        \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
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

// 2 * mvpa::D1D * mvpa::Q1D * mvpa::D1D * mvpa::D1D
#define MASSVEC3DPA_3                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dx = 0; dx < mvpa::D1D; ++dx) {                              \
    u += smX[dz][dy][dx] * smB[qx][dx];                                        \
  }                                                                            \
  DDQ[dz][dy][qx] = u;

// 2 * mvpa::D1D * mvpa::Q1D * mvpa::Q1D * mvpa::D1D
#define MASSVEC3DPA_4                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dy = 0; dy < mvpa::D1D; ++dy) {                              \
    u += DDQ[dz][dy][qx] * smB[qy][dy];                                        \
  }                                                                            \
  DQQ[dz][qy][qx] = u;

// 2 * mvpa::D1D * mvpa::Q1D * mvpa::Q1D * mvpa::Q1D + mvpa::Q1D * mvpa::Q1D *
// mvpa::Q1D
#define MASSVEC3DPA_5                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type dz = 0; dz < mvpa::D1D; ++dz) {                              \
    u += DQQ[dz][qy][qx] * smB[qz][dz];                                        \
  }                                                                            \
  QQQ[qz][qy][qx] = u * MVPA_D(qx, qy, qz, e);

// 2 * mvpa::Q1D * mvpa::D1D * mvpa::Q1D * mvpa::Q1D
#define MASSVEC3DPA_6                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qx = 0; qx < mvpa::Q1D; ++qx) {                              \
    u += QQQ[qz][qy][qx] * smBt[dx][qx];                                       \
  }                                                                            \
  QQD[qz][qy][dx] = u;

// 2 * mvpa::Q1D * mvpa::D1D * mvpa::D1D * mvpa::Q1D
#define MASSVEC3DPA_7                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qy = 0; qy < mvpa::Q1D; ++qy) {                              \
    u += QQD[qz][qy][dx] * smBt[dy][qy];                                       \
  }                                                                            \
  QDD[qz][dy][dx] = u;

// 2 * mvpa::Q1D * mvpa::D1D * mvpa::D1D * mvpa::D1D
#define MASSVEC3DPA_8                                                          \
  Real_type u = 0.0;                                                           \
  for (Index_type qz = 0; qz < mvpa::Q1D; ++qz) {                              \
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

  template <size_t block_size, size_t tune_idx>
  void runCudaVariantImpl(VariantID vid);
  template <size_t block_size, size_t tune_idx>
  void runHipVariantImpl(VariantID vid);
  template <size_t work_group_size> void runSyclVariantImpl(VariantID vid);

  template <typename inner_x, typename inner_y, typename inner_z,
            typename RESOURCE>
  void runRAJAImpl(RESOURCE &res);

private:
  static const size_t default_gpu_block_size =
      mvpa::Q1D * mvpa::Q1D * mvpa::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_NE;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
