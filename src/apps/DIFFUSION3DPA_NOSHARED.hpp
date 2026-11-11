//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Element-wise action of the 3D finite element volume diffusion operator
/// via partial assembly and sum factorization, with the per-element scratch
/// arrays held in global memory rather than in team shared memory.
///
/// This is the same algorithm as DIFFUSION3DPA; the two differ only in where
/// the scratch arrays sBG / sm0 / sm1 live, so that comparing them isolates
/// the cost of the scratch storage location.
///
/// Based on MFEM's/CEED algorithms.
/// Reference implementation - MFEM-v4.9
/// https://github.com/mfem/mfem/blob/v4.9/fem/integ/bilininteg_diffusion_kernels.hpp
///
///
/// for (Index_type e = 0; e < NE; ++e) {
///
///   constexpr Index_type MQ1 = diff_ns::Q1D;
///   constexpr Index_type MD1 = diff_ns::D1D;
///   constexpr Index_type MDQ = diff_ns::MDQ;
///
///   // sBG, sm0, and sm1 are carved out of a per-element slice of a global
///   // Workspace array. This is the only difference from DIFFUSION3DPA,
///   // which declares them RAJA_TEAM_SHARED.
///   Real_ptr sBG = Workspace + diff_ns::SCRATCH * e;
///   Real_type (*sm0)[MDQ*MDQ*MDQ] =
///       (Real_type (*)[MDQ*MDQ*MDQ])(sBG + MQ1*MD1);
///   Real_type (*sm1)[MDQ*MDQ*MDQ] = sm0 + 3;
///
///   Real_type (*B)[MD1] = (Real_type (*)[MD1]) sBG;
///   Real_type (*G)[MD1] = (Real_type (*)[MD1]) sBG;
///   Real_type (*Bt)[MQ1] = (Real_type (*)[MQ1]) sBG;
///   Real_type (*Gt)[MQ1] = (Real_type (*)[MQ1]) sBG;
///   Real_type (*s_X)[MD1][MD1]    = (Real_type (*)[MD1][MD1]) (sm0+2);
///   Real_type (*DDQ0)[MD1][MQ1] = (Real_type (*)[MD1][MQ1]) (sm0+0);
///   Real_type (*DDQ1)[MD1][MQ1] = (Real_type (*)[MD1][MQ1]) (sm0+1);
///   Real_type (*DQQ0)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm1+0);
///   Real_type (*DQQ1)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm1+1);
///   Real_type (*DQQ2)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm1+2);
///   Real_type (*QQQ0)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm0+0);
///   Real_type (*QQQ1)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm0+1);
///   Real_type (*QQQ2)[MQ1][MQ1] = (Real_type (*)[MQ1][MQ1]) (sm0+2);
///   Real_type (*QQD0)[MQ1][MD1] = (Real_type (*)[MQ1][MD1]) (sm1+0);
///   Real_type (*QQD1)[MQ1][MD1] = (Real_type (*)[MQ1][MD1]) (sm1+1);
///   Real_type (*QQD2)[MQ1][MD1] = (Real_type (*)[MQ1][MD1]) (sm1+2);
///   Real_type (*QDD0)[MD1][MD1] = (Real_type (*)[MD1][MD1]) (sm0+0);
///   Real_type (*QDD1)[MD1][MD1] = (Real_type (*)[MD1][MD1]) (sm0+1);
///   Real_type (*QDD2)[MD1][MD1] = (Real_type (*)[MD1][MD1]) (sm0+2);
///
///   // ... same sum-factorization stages as DIFFUSION3DPA ...
///
/// } // element loop
///

#ifndef RAJAPerf_Apps_DIFFUSION3DPA_NOSHARED_HPP
#define RAJAPerf_Apps_DIFFUSION3DPA_NOSHARED_HPP

#define DIFFUSION3DPA_NOSHARED_DATA_SETUP                                      \
  Real_ptr Basis = m_B;                                                        \
  Real_ptr dBasis = m_G;                                                       \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Real_ptr Workspace = m_Workspace;                                            \
  Index_type NE = m_NE;                                                        \
  const bool symmetric = true;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace diff_ns {
constexpr RAJA::Index_type D1D = 3;
constexpr RAJA::Index_type Q1D = 4;
constexpr RAJA::Index_type DPA_SYM = 6;
constexpr RAJA::Index_type MDQ = (Q1D > D1D) ? Q1D : D1D;
// Per-element global scratch footprint: the same sBG + sm0[3] + sm1[3]
// values that DIFFUSION3DPA holds in team shared memory.
constexpr RAJA::Index_type SCRATCH =
    Q1D * D1D + 6 * MDQ * MDQ * MDQ;
} // namespace diff_ns

#define DPANS_b(x, y) Basis[x + diff_ns::Q1D * y]
#define DPANS_g(x, y) dBasis[x + diff_ns::Q1D * y]
#define DPANS_X(dx, dy, dz, e)                                                 \
  X[dx + diff_ns::D1D * dy + diff_ns::D1D * diff_ns::D1D * dz +                \
    diff_ns::D1D * diff_ns::D1D * diff_ns::D1D * e]
#define DPANS_Y(dx, dy, dz, e)                                                 \
  Y[dx + diff_ns::D1D * dy + diff_ns::D1D * diff_ns::D1D * dz +                \
    diff_ns::D1D * diff_ns::D1D * diff_ns::D1D * e]
#define DPANS_d(qx, qy, qz, s, e)                                              \
  D[qx + diff_ns::Q1D * qy + diff_ns::Q1D * diff_ns::Q1D * qz +                \
    diff_ns::Q1D * diff_ns::Q1D * diff_ns::Q1D * s +                           \
    diff_ns::Q1D * diff_ns::Q1D * diff_ns::Q1D * diff_ns::DPA_SYM * e]

#define DIFFUSION3DPA_NOSHARED_0                                               \
  constexpr Index_type MQ1 = diff_ns::Q1D;                                     \
  constexpr Index_type MD1 = diff_ns::D1D;                                     \
  constexpr Index_type MDQ = diff_ns::MDQ;                                     \
  Real_ptr sBG = Workspace + diff_ns::SCRATCH * e;                             \
  Real_type(*sm0)[MDQ * MDQ * MDQ] =                                           \
      (Real_type(*)[MDQ * MDQ * MDQ])(sBG + MQ1 * MD1);                        \
  Real_type(*sm1)[MDQ * MDQ * MDQ] = sm0 + 3;                                  \
  Real_type(*B)[MD1] = (Real_type(*)[MD1])sBG;                                 \
  Real_type(*G)[MD1] = (Real_type(*)[MD1])sBG;                                 \
  Real_type(*Bt)[MQ1] = (Real_type(*)[MQ1])sBG;                                \
  Real_type(*Gt)[MQ1] = (Real_type(*)[MQ1])sBG;                                \
  Real_type(*s_X)[MD1][MD1] = (Real_type(*)[MD1][MD1])(sm0 + 2);               \
  Real_type(*DDQ0)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])(sm0 + 0);              \
  Real_type(*DDQ1)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])(sm0 + 1);              \
  Real_type(*DQQ0)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm1 + 0);              \
  Real_type(*DQQ1)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm1 + 1);              \
  Real_type(*DQQ2)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm1 + 2);              \
  Real_type(*QQQ0)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm0 + 0);              \
  Real_type(*QQQ1)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm0 + 1);              \
  Real_type(*QQQ2)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])(sm0 + 2);              \
  Real_type(*QQD0)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])(sm1 + 0);              \
  Real_type(*QQD1)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])(sm1 + 1);              \
  Real_type(*QQD2)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])(sm1 + 2);              \
  Real_type(*QDD0)[MD1][MD1] = (Real_type(*)[MD1][MD1])(sm0 + 0);              \
  Real_type(*QDD1)[MD1][MD1] = (Real_type(*)[MD1][MD1])(sm0 + 1);              \
  Real_type(*QDD2)[MD1][MD1] = (Real_type(*)[MD1][MD1])(sm0 + 2);

#define DIFFUSION3DPA_NOSHARED_1 s_X[dz][dy][dx] = DPANS_X(dx, dy, dz, e);

#define DIFFUSION3DPA_NOSHARED_2                                               \
  B[qx][dy] = DPANS_b(qx, dy);                                                 \
  G[qx][dy] = DPANS_g(qx, dy);

#define DIFFUSION3DPA_NOSHARED_3                                               \
  Real_type u = 0.0, v = 0.0;                                                  \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dx = 0; dx < diff_ns::D1D; ++dx) {                           \
    const Real_type coords = s_X[dz][dy][dx];                                  \
    u += coords * B[qx][dx];                                                   \
    v += coords * G[qx][dx];                                                   \
  }                                                                            \
  DDQ0[dz][dy][qx] = u;                                                        \
  DDQ1[dz][dy][qx] = v;

#define DIFFUSION3DPA_NOSHARED_4                                               \
  Real_type u = 0.0, v = 0.0, w = 0.0;                                         \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dy = 0; dy < diff_ns::D1D; ++dy) {                           \
    u += DDQ1[dz][dy][qx] * B[qy][dy];                                         \
    v += DDQ0[dz][dy][qx] * G[qy][dy];                                         \
    w += DDQ0[dz][dy][qx] * B[qy][dy];                                         \
  }                                                                            \
  DQQ0[dz][qy][qx] = u;                                                        \
  DQQ1[dz][qy][qx] = v;                                                        \
  DQQ2[dz][qy][qx] = w;

#define DIFFUSION3DPA_NOSHARED_5                                               \
  Real_type u = 0.0, v = 0.0, w = 0.0;                                         \
  RAJAPERF_UNROLL(MD1)                                                         \
  for (Index_type dz = 0; dz < diff_ns::D1D; ++dz) {                           \
    u += DQQ0[dz][qy][qx] * B[qz][dz];                                         \
    v += DQQ1[dz][qy][qx] * B[qz][dz];                                         \
    w += DQQ2[dz][qy][qx] * G[qz][dz];                                         \
  }                                                                            \
  const Real_type O11 = DPANS_d(qx, qy, qz, 0, e);                             \
  const Real_type O12 = DPANS_d(qx, qy, qz, 1, e);                             \
  const Real_type O13 = DPANS_d(qx, qy, qz, 2, e);                             \
  const Real_type O21 = symmetric ? O12 : DPANS_d(qx, qy, qz, 3, e);           \
  const Real_type O22 =                                                        \
      symmetric ? DPANS_d(qx, qy, qz, 3, e) : DPANS_d(qx, qy, qz, 4, e);       \
  const Real_type O23 =                                                        \
      symmetric ? DPANS_d(qx, qy, qz, 4, e) : DPANS_d(qx, qy, qz, 5, e);       \
  const Real_type O31 = symmetric ? O13 : DPANS_d(qx, qy, qz, 6, e);           \
  const Real_type O32 = symmetric ? O23 : DPANS_d(qx, qy, qz, 7, e);           \
  const Real_type O33 =                                                        \
      symmetric ? DPANS_d(qx, qy, qz, 5, e) : DPANS_d(qx, qy, qz, 8, e);       \
  const Real_type gX = u;                                                      \
  const Real_type gY = v;                                                      \
  const Real_type gZ = w;                                                      \
  QQQ0[qz][qy][qx] = (O11 * gX) + (O12 * gY) + (O13 * gZ);                     \
  QQQ1[qz][qy][qx] = (O21 * gX) + (O22 * gY) + (O23 * gZ);                     \
  QQQ2[qz][qy][qx] = (O31 * gX) + (O32 * gY) + (O33 * gZ);

#define DIFFUSION3DPA_NOSHARED_6                                               \
  Bt[dy][qx] = DPANS_b(qx, dy);                                                \
  Gt[dy][qx] = DPANS_g(qx, dy);

#define DIFFUSION3DPA_NOSHARED_7                                               \
  Real_type u = 0.0, v = 0.0, w = 0.0;                                         \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qx = 0; qx < diff_ns::Q1D; ++qx) {                           \
    u += QQQ0[qz][qy][qx] * Gt[dx][qx];                                        \
    v += QQQ1[qz][qy][qx] * Bt[dx][qx];                                        \
    w += QQQ2[qz][qy][qx] * Bt[dx][qx];                                        \
  }                                                                            \
  QQD0[qz][qy][dx] = u;                                                        \
  QQD1[qz][qy][dx] = v;                                                        \
  QQD2[qz][qy][dx] = w;

#define DIFFUSION3DPA_NOSHARED_8                                               \
  Real_type u = 0.0, v = 0.0, w = 0.0;                                         \
  RAJAPERF_UNROLL(diff_ns::Q1D)                                                \
  for (Index_type qy = 0; qy < diff_ns::Q1D; ++qy) {                           \
    u += QQD0[qz][qy][dx] * Bt[dy][qy];                                        \
    v += QQD1[qz][qy][dx] * Gt[dy][qy];                                        \
    w += QQD2[qz][qy][dx] * Bt[dy][qy];                                        \
  }                                                                            \
  QDD0[qz][dy][dx] = u;                                                        \
  QDD1[qz][dy][dx] = v;                                                        \
  QDD2[qz][dy][dx] = w;

#define DIFFUSION3DPA_NOSHARED_9                                               \
  Real_type u = 0.0, v = 0.0, w = 0.0;                                         \
  RAJAPERF_UNROLL(MQ1)                                                         \
  for (Index_type qz = 0; qz < diff_ns::Q1D; ++qz) {                           \
    u += QDD0[qz][dy][dx] * Bt[dz][qz];                                        \
    v += QDD1[qz][dy][dx] * Bt[dz][qz];                                        \
    w += QDD2[qz][dy][dx] * Gt[dz][qz];                                        \
  }                                                                            \
  DPANS_Y(dx, dy, dz, e) += (u + v + w);

namespace rajaperf {
class RunParams;

namespace apps {

class DIFFUSION3DPA_NOSHARED : public KernelBase {
public:
  DIFFUSION3DPA_NOSHARED(const RunParams &params);

  ~DIFFUSION3DPA_NOSHARED();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSeqVariantTunings() {}
  void defineOpenMPVariantTunings() {}
  void defineOpenMPTargetVariantTunings() {}
  void defineKokkosVariantTunings() {}
  void defineSyclVariantTunings() {}

  template <size_t block_size> void runCudaVariantImpl(VariantID vid);
  template <size_t block_size> void runHipVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size =
      diff_ns::Q1D * diff_ns::Q1D * diff_ns::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_G;
  Real_ptr m_Gt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;
  Real_ptr m_Workspace;

  Index_type m_NE;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
