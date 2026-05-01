//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Action of a 3D finite element mass matrix on elements with shared DOFs
/// via partial assembly and sum factorization
///
/// for (Index_type e = 0; e < NE; ++e) {
///
///   constexpr Index_type MQ1 = mpa_at::Q1D;
///   constexpr Index_type MD1 = mpa_at::D1D;
///
///   constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;
///
///   Real_type sm_B[MQ1][MD1];
///   Real_type sm_Bt[MD1][MQ1];
///
///   Real_type sm0[MDQ * MDQ * MDQ];
///   Real_type sm1[MDQ * MDQ * MDQ];
///   Real_type(*sm_X)[MD1][MD1]   = (Real_type(*)[MD1][MD1])sm0;
///   Real_type(*DDQ)[MD1][MQ1]    = (Real_type(*)[MD1][MQ1])sm1;
///   Real_type(*DQQ)[MQ1][MQ1]    = (Real_type(*)[MQ1][MQ1])sm0;
///   Real_type(*QQQ)[MQ1][MQ1]    = (Real_type(*)[MQ1][MQ1])sm1;
///   Real_type(*QQD)[MQ1][MD1]    = (Real_type(*)[MQ1][MD1])sm0;
///   Real_type(*QDD)[MD1][MD1]    = (Real_type(*)[MD1][MD1])sm1;
///
///   Index_type thread_dofs[MD1 * MD1 * MD1];
///
///   for(Index_type dz=0; dz<mpa_at::D1D; ++dz) {
///     for(Index_type dy=0; dy<mpa_at::D1D; ++dy) {
///       for(Index_type dx=0; dx<mpa_at::D1D; ++dx) {
///         Index_type j = dx + mpa_at::D1D * (dy + dz * mpa_at::D1D);
///         //missing dof_map for lexicographical ordering
///         thread_dofs[j] = ElemToDoF[j + mpa_at::D1D * mpa_at::D1D * mpa_at::D1D * e];
///         sm_X[dz][dy][dx]  = X[thread_dofs[j]];
///       }
///     }
///   }
///
///   for(Index_type d=0; d<mpa_at::D1D; ++d) {
///     for(Index_type q=0; q<mpa_at::Q1D; ++q) {
///       sm_B[q][d]  = MPAT_B(q, d);
///       sm_Bt[d][q] = sm_B[q][d];
///     }
///   }
///
///
///   for(Index_type dz=0; dz<mpa_at::D1D; ++dz) {
///     for(Index_type dy=0; dy<mpa_at::D1D; ++dy) {
///       for(Index_type qx=0; qx<mpa_at::Q1D; ++qx) {
///         Real_type u = 0.0;
///         for (Index_type dx = 0; dx < mpa_at::D1D; ++dx)
///         {
///           u += sm_X[dz][dy][dx] * sm_B[qx][dx];
///         }
///         DDQ[dz][dy][qx] = u;
///       }
///     }
///   }
///
///   for(Index_type dz=0; dz<mpa_at::D1D; ++dz) {
///     for(Index_type qy=0; qy<mpa_at::Q1D; ++qy) {
///       for(Index_type qx=0; qx<mpa_at::Q1D; ++qx) {
///
///         Real_type u = 0.0;
///         for (Index_type dy = 0; dy < mpa_at::D1D; ++dy)
///         {
///           u += DDQ[dz][dy][qx] * sm_B[qy][dy];
///         }
///         DQQ[dz][qy][qx] = u;
///       }
///     }
///   }
///
///   for(Index_type qz=0; qz<mpa_at::Q1D; ++qz) {
///     for(Index_type qy=0; qy<mpa_at::Q1D; ++qy) {
///       for(Index_type qx=0; qx<mpa_at::Q1D; ++qx) {
///         Real_type u = 0.0;
///         for (Index_type dz = 0; dz < mpa_at::D1D; ++dz)
///         {
///           u += DQQ[dz][qy][qx] * sm_B[qz][dz];
///         }
///         QQQ[qz][qy][qx] = u * MPAT_D(qx, qy, qz, e);
///       }
///     }
///   }
///
///   for(Index_type qz=0; qz<mpa_at::Q1D; ++qz) {
///     for(Index_type qy=0; qy<mpa_at::Q1D; ++qy) {
///       for(Index_type dx=0; dx<mpa_at::D1D; ++dx) {
///         Real_type u = 0.0;
///         for (Index_type qx = 0; qx < mpa_at::Q1D; ++qx)
///         {
///           u += QQQ[qz][qy][qx] * sm_Bt[dx][qx];
///         }
///         QQD[qz][qy][dx] = u;
///       }
///     }
///   }
///
///   for(Index_type qz=0; qz<mpa_at::Q1D; ++qz) {
///     for(Index_type dy=0; dy<mpa_at::D1D; ++dy) {
///       for(Index_type dx=0; dx<mpa_at::D1D; ++dx) {
///         Real_type u = 0.0;
///         for (Index_type qy = 0; qy<mpa_at::Q1D; ++qy)
///         {
///         u += QQD[qz][qy][dx] * sm_Bt[dy][qy];
///         }
///         QDD[qz][dy][dx] = u;
///       }
///     }
///   }
///
///   for(Index_type dz=0; dz<mpa_at::D1D; ++dz) {
///     for(Index_type dy=0; dy<mpa_at::D1D; ++dy) {
///       for(Index_type dx=0; dx<mpa_at::D1D; ++dx) {
///         Real_type u = 0.0;
///         for (Index_type qz = 0; qz < mpa_at::Q1D; ++qz)
///         {
///            u += QDD[qz][dy][dx] * sm_Bt[dz][qz];
///         }
///         const Index_type j = dx + mpa_at::D1D * (dy + dz * mpa_at::D1D);
///         Y[thread_dofs[j]] += u; //atomic add
///       }
///     }
///   }
///
/// } // element loop
///

#ifndef RAJAPerf_Apps_MASS3DPA_ATOMIC_HPP
#define RAJAPerf_Apps_MASS3DPA_ATOMIC_HPP

#define MASS3DPA_ATOMIC_DATA_SETUP                                             \
  Real_ptr B = m_B;                                                            \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Index_ptr ElemToDoF = m_ElemToDoF;                                           \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace mpa_at {
constexpr RAJA::Index_type D1D = 3;
constexpr RAJA::Index_type Q1D = 4;
} // namespace mpa_at

#define MPAT_B(x, y) B[x + mpa_at::Q1D * y]
#define MPAT_D(qx, qy, qz, e)                                                  \
  D[qx + mpa_at::Q1D * qy + mpa_at::Q1D * mpa_at::Q1D * qz +                   \
    mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D * e]

#define MASS3DPA_ATOMIC_0_CPU                                                  \
  constexpr Index_type MQ1 = mpa_at::Q1D;                                      \
  constexpr Index_type MD1 = mpa_at::D1D;                                      \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
  Real_type sm_B[MQ1][MD1];                                                    \
  Real_type sm_Bt[MD1][MQ1];                                                   \
  Real_type sm0[MDQ * MDQ * MDQ];                                              \
  Real_type sm1[MDQ * MDQ * MDQ];                                              \
  Real_type(*sm_X)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                    \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;                     \
  Index_type thread_dofs[MD1 * MD1 * MD1];

#define MASS3DPA_ATOMIC_0_GPU                                                  \
  constexpr Index_type MQ1 = mpa_at::Q1D;                                      \
  constexpr Index_type MD1 = mpa_at::D1D;                                      \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                          \
  RAJA_TEAM_SHARED Real_type sm_B[MQ1][MD1];                                   \
  RAJA_TEAM_SHARED Real_type sm_Bt[MD1][MQ1];                                  \
  RAJA_TEAM_SHARED Real_type sm0[MDQ * MDQ * MDQ];                             \
  RAJA_TEAM_SHARED Real_type sm1[MDQ * MDQ * MDQ];                             \
  Real_type(*sm_X)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0;                    \
  Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;                     \
  Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;                     \
  Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;                     \
  Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;                     \
  Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;                     \
  RAJA_TEAM_SHARED Index_type thread_dofs[MD1 * MD1 * MD1];

#define MASS3DPA_ATOMIC_1                                                      \
  Index_type j = dx + mpa_at::D1D * (dy + dz * mpa_at::D1D);                   \
  thread_dofs[j] = ElemToDoF[j + mpa_at::D1D * mpa_at::D1D * mpa_at::D1D * e]; \
  sm_X[dz][dy][dx] =                                                           \
      X[thread_dofs[j]]; // missing dof_map for lexicographical ordering

#define MASS3DPA_ATOMIC_2                                                      \
  sm_B[q][d] = MPAT_B(q, d);                                                   \
  sm_Bt[d][q] = sm_B[q][d];

// flop counts
// 2 * D1D
#define MASS3DPA_ATOMIC_3                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type dx = 0; dx < mpa_at::D1D; ++dx) {                            \
    u += sm_X[dz][dy][dx] * sm_B[qx][dx];                                      \
  }                                                                            \
  DDQ[dz][dy][qx] = u;

// 2 * D1D
#define MASS3DPA_ATOMIC_4                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type dy = 0; dy < mpa_at::D1D; ++dy) {                            \
    u += DDQ[dz][dy][qx] * sm_B[qy][dy];                                       \
  }                                                                            \
  DQQ[dz][qy][qx] = u;

// 2 * D1D + 1
#define MASS3DPA_ATOMIC_5                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type dz = 0; dz < mpa_at::D1D; ++dz) {                            \
    u += DQQ[dz][qy][qx] * sm_B[qz][dz];                                       \
  }                                                                            \
  QQQ[qz][qy][qx] = u * MPAT_D(qx, qy, qz, e);

// 2 * Q1D
#define MASS3DPA_ATOMIC_6                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type qx = 0; qx < mpa_at::Q1D; ++qx) {                            \
    u += QQQ[qz][qy][qx] * sm_Bt[dx][qx];                                      \
  }                                                                            \
  QQD[qz][qy][dx] = u;

// 2 * Q1D
#define MASS3DPA_ATOMIC_7                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type qy = 0; qy < mpa_at::Q1D; ++qy) {                            \
    u += QQD[qz][qy][dx] * sm_Bt[dy][qy];                                      \
  }                                                                            \
  QDD[qz][dy][dx] = u;

// 2 * Q1D + 1
#define MASS3DPA_ATOMIC_8                                                      \
  Real_type u = 0.0;                                                           \
  for (Index_type qz = 0; qz < mpa_at::Q1D; ++qz) {                            \
    u += QDD[qz][dy][dx] * sm_Bt[dz][qz];                                      \
  }                                                                            \
  const Index_type j = dx + mpa_at::D1D * (dy + dz * mpa_at::D1D);

#define MASS3DPA_ATOMIC_9(atomicAdd)                                           \
  atomicAdd(Y[thread_dofs[j]], u); // atomic add

namespace rajaperf {
class RunParams;

namespace apps {

class MASS3DPA_ATOMIC : public KernelBase {
public:
  MASS3DPA_ATOMIC(const RunParams &params);

  ~MASS3DPA_ATOMIC();

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
  static const size_t default_gpu_block_size =
      mpa_at::Q1D * mpa_at::Q1D * mpa_at::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_Nx;       // zones in x dimension
  Index_type m_Ny;       // zones in y dimension
  Index_type m_Nz;       // zones in z dimension
  Index_type m_P;        // polynomial order
  Index_type m_Tot_Dofs; // total number of dofs

  Index_ptr m_ElemToDoF;

  Index_type m_NE;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
