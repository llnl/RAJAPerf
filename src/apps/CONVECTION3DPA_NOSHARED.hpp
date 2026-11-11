//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Element-wise action of a 3D finite element volume convection operator
/// via partial assembly and sum factorization, with the per-element scratch
/// arrays held in global memory rather than in team shared memory.
///
/// This is the same algorithm as CONVECTION3DPA; the two differ only in where
/// the scratch arrays sm0..sm5 live, so that comparing them isolates the cost
/// of the scratch storage location.
///
/// Based on MFEM's/CEED algorithms.
/// Reference implementation - MFEM-v4.9
/// https://github.com/mfem/mfem/blob/v4.9/fem/integ/bilininteg_convection_kernels.hpp
///
///
/// for(Index_type e = 0; e < NE; ++e) {
///
///   constexpr Index_type max_D1D = conv_ns::D1D;
///   constexpr Index_type max_Q1D = conv_ns::Q1D;
///   constexpr Index_type max_DQ = (max_Q1D > max_D1D) ? max_Q1D : max_D1D;
///
///   // The six scratch arrays are carved out of a per-element slice of a
///   // global Workspace array. This is the only difference from
///   // CONVECTION3DPA, which declares them MFEM_SHARED / RAJA_TEAM_SHARED.
///   Real_ptr sm0 = Workspace + conv_ns::SCRATCH * e;
///   Real_ptr sm1 = sm0 + max_DQ*max_DQ*max_DQ;
///   Real_ptr sm2 = sm1 + max_DQ*max_DQ*max_DQ;
///   Real_ptr sm3 = sm2 + max_DQ*max_DQ*max_DQ;
///   Real_ptr sm4 = sm3 + max_DQ*max_DQ*max_DQ;
///   Real_ptr sm5 = sm4 + max_DQ*max_DQ*max_DQ;
///
///   Real_type (*u)[max_D1D][max_D1D] = (Real_type (*)[max_D1D][max_D1D]) sm0;
///   for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv_ns::D1D; ++dy)
///     {
///       for(Index_type dx = 0; dx < conv_ns::D1D; ++dx)
///       {
///         u[dz][dy][dx] = CPANS_X(dx,dy,dz,e);
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*Bu)[max_D1D][max_Q1D] = (Real_type (*)[max_D1D][max_Q1D])sm1;
///   Real_type (*Gu)[max_D1D][max_Q1D] = (Real_type (*)[max_D1D][max_Q1D])sm2;
///   for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv_ns::D1D; ++dy)
///     {
///       for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///       {
///         Real_type Bu_ = 0.0;
///         Real_type Gu_ = 0.0;
///         for(Index_type dx = 0; dx < conv_ns::D1D; ++dx)
///         {
///           const Real_type bx = CPANS_B(qx,dx);
///           const Real_type gx = CPANS_G(qx,dx);
///           const Real_type x = u[dz][dy][dx];
///           Bu_ += bx * x;
///           Gu_ += gx * x;
///         }
///         Bu[dz][dy][qx] = Bu_;
///         Gu[dz][dy][qx] = Gu_;
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*BBu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm3;
///   Real_type (*GBu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm4;
///   Real_type (*BGu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm5;
///   for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///   {
///     for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///     {
///       for(Index_type qy = 0; qy < conv_ns::Q1D; ++qy)
///       {
///         Real_type BBu_ = 0.0;
///         Real_type GBu_ = 0.0;
///         Real_type BGu_ = 0.0;
///         for(Index_type dy = 0; dy < conv_ns::D1D; ++dy)
///         {
///           const Real_type bx = CPANS_B(qy,dy);
///           const Real_type gx = CPANS_G(qy,dy);
///           BBu_ += bx * Bu[dz][dy][qx];
///           GBu_ += gx * Bu[dz][dy][qx];
///           BGu_ += bx * Gu[dz][dy][qx];
///         }
///         BBu[dz][qy][qx] = BBu_;
///         GBu[dz][qy][qx] = GBu_;
///         BGu[dz][qy][qx] = BGu_;
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*GBBu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm0;
///   Real_type (*BGBu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm1; 
///   Real_type (*BBGu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm2; 
///   
///   for(Index_type qx = 0; qx <conv_ns::Q1D; ++qx)
///   {
///     for(Index_type qy = 0; qy < conv_ns::Q1D; ++qy)
///     {
///       for(Index_type qz = 0; qz < conv_ns::Q1D; ++qz)
///       {
///         Real_type GBBu_ = 0.0;
///         Real_type BGBu_ = 0.0;
///         Real_type BBGu_ = 0.0;
///         for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///         {
///           const Real_type bx = CPANS_B(qz,dz);
///           const Real_type gx = CPANS_G(qz,dz);
///           GBBu_ += gx * BBu[dz][qy][qx];
///           BGBu_ += bx * GBu[dz][qy][qx];
///           BBGu_ += bx * BGu[dz][qy][qx];
///         }
///         GBBu[qz][qy][qx] = GBBu_;
///         BGBu[qz][qy][qx] = BGBu_;
///         BBGu[qz][qy][qx] = BBGu_;
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*DGu)[max_Q1D][max_Q1D] = (Real_type (*)[max_Q1D][max_Q1D])sm3;
///   for(Index_type qz = 0; qz < conv_ns::Q1D; ++qz)
///   {
///     for(Index_type qy = 0; qy < conv_ns::Q1D; ++qy)
///     {
///       for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///       {
///         const Real_type O1 = CPANS_op(qx,qy,qz,0,e);
///         const Real_type O2 = CPANS_op(qx,qy,qz,1,e);
///         const Real_type O3 = CPANS_op(qx,qy,qz,2,e);
///
///         const Real_type gradX = BBGu[qz][qy][qx];
///         const Real_type gradY = BGBu[qz][qy][qx];
///         const Real_type gradZ = GBBu[qz][qy][qx];
///
///         DGu[qz][qy][qx] = (O1 * gradX) + (O2 * gradY) + (O3 * gradZ);
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*BDGu)[max_Q1D][max_Q1D] = (Real_type
///   (*)[max_Q1D][max_Q1D])sm4; for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///   {
///     for(Index_type qy = 0; qy < conv_ns::Q1D; ++qy)
///     {
///       for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///       {
///          Real_type BDGu_ = 0.0;
///          for(Index_type qz = 0; qz < conv_ns::Q1D; ++qz)
///          {
///             const Real_type w = CPANS_Bt(dz,qz);
///             BDGu_ += w * DGu[qz][qy][qx];
///          }
///          BDGu[dz][qy][qx] = BDGu_;
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*BBDGu)[max_D1D][max_Q1D] = (Real_type
///   (*)[max_D1D][max_Q1D])sm5; for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///   {
///     for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///      {
///        for(Index_type dy = 0; dy < conv_ns::D1D; ++dy)
///         {
///            Real_type BBDGu_ = 0.0;
///            for(Index_type qy = 0; qy < conv_ns::Q1D; ++qy)
///            {
///              const Real_type w = CPANS_Bt(dy,qy);
///              BBDGu_ += w * BDGu[dz][qy][qx];
///           }
///           BBDGu[dz][dy][qx] = BBDGu_;
///        }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   for(Index_type dz = 0; dz < conv_ns::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv_ns::D1D; ++dy)
///     {
///       for(Index_type dx = 0; dx < conv_ns::D1D; ++dx)
///       {
///         Real_type BBBDGu = 0.0;
///         for(Index_type qx = 0; qx < conv_ns::Q1D; ++qx)
///         {
///           const Real_type w = CPANS_Bt(dx,qx);
///           BBBDGu += w * BBDGu[dz][dy][qx];
///         }
///         CPANS_Y(dx,dy,dz,e) += BBBDGu;
///       }
///     }
///   }
/// } // element loop
///

#ifndef RAJAPerf_Apps_CONVECTION3DPA_NOSHARED_HPP
#define RAJAPerf_Apps_CONVECTION3DPA_NOSHARED_HPP

#define CONVECTION3DPA_NOSHARED_DATA_SETUP                                     \
  Real_ptr Basis = m_B;                                                        \
  Real_ptr tBasis = m_Bt;                                                      \
  Real_ptr dBasis = m_G;                                                       \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Real_ptr Workspace = m_Workspace;                                            \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace conv_ns {
constexpr RAJA::Index_type D1D = 3;
constexpr RAJA::Index_type Q1D = 4;
constexpr RAJA::Index_type VDIM = 3;
// Per-element global scratch footprint: the same 6 * max_DQ^3 values that
// CONVECTION3DPA holds in team shared memory.
constexpr RAJA::Index_type max_DQ = (Q1D > D1D) ? Q1D : D1D;
constexpr RAJA::Index_type SCRATCH = 6 * max_DQ * max_DQ * max_DQ;
} // namespace conv_ns

#define CPANS_B(x, y) Basis[x + conv_ns::Q1D * y]
#define CPANS_Bt(x, y) tBasis[x + conv_ns::D1D * y]
#define CPANS_G(x, y) dBasis[x + conv_ns::Q1D * y]
#define CPANS_X(dx, dy, dz, e)                                                 \
  X[dx + conv_ns::D1D * dy + conv_ns::D1D * conv_ns::D1D * dz +                \
    conv_ns::D1D * conv_ns::D1D * conv_ns::D1D * e]
#define CPANS_Y(dx, dy, dz, e)                                                 \
  Y[dx + conv_ns::D1D * dy + conv_ns::D1D * conv_ns::D1D * dz +                \
    conv_ns::D1D * conv_ns::D1D * conv_ns::D1D * e]
#define CPANS_op(qx, qy, qz, d, e)                                             \
  D[qx + conv_ns::Q1D * qy + conv_ns::Q1D * conv_ns::Q1D * qz +                \
    conv_ns::Q1D * conv_ns::Q1D * conv_ns::Q1D * d +                           \
    conv_ns::VDIM * conv_ns::Q1D * conv_ns::Q1D * conv_ns::Q1D * e]

#define CONVECTION3DPA_NOSHARED_0                                              \
  constexpr Index_type max_D1D = conv_ns::D1D;                                 \
  constexpr Index_type max_Q1D = conv_ns::Q1D;                                 \
  constexpr Index_type max_DQ = conv_ns::max_DQ;                               \
  Real_ptr sm0 = Workspace + conv_ns::SCRATCH * e;                             \
  Real_ptr sm1 = sm0 + max_DQ * max_DQ * max_DQ;                               \
  Real_ptr sm2 = sm1 + max_DQ * max_DQ * max_DQ;                               \
  Real_ptr sm3 = sm2 + max_DQ * max_DQ * max_DQ;                               \
  Real_ptr sm4 = sm3 + max_DQ * max_DQ * max_DQ;                               \
  Real_ptr sm5 = sm4 + max_DQ * max_DQ * max_DQ;                               \
  Real_type(*u)[max_D1D][max_D1D] = (Real_type(*)[max_D1D][max_D1D])sm0;       \
  Real_type(*Bu)[max_D1D][max_Q1D] = (Real_type(*)[max_D1D][max_Q1D])sm1;      \
  Real_type(*Gu)[max_D1D][max_Q1D] = (Real_type(*)[max_D1D][max_Q1D])sm2;      \
  Real_type(*BBu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm3;     \
  Real_type(*GBu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm4;     \
  Real_type(*BGu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm5;     \
  Real_type(*GBBu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm0;    \
  Real_type(*BGBu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm1;    \
  Real_type(*BBGu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm2;    \
  Real_type(*DGu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm3;     \
  Real_type(*BDGu)[max_Q1D][max_Q1D] = (Real_type(*)[max_Q1D][max_Q1D])sm4;    \
  Real_type(*BBDGu)[max_D1D][max_Q1D] = (Real_type(*)[max_D1D][max_Q1D])sm5;

#define CONVECTION3DPA_NOSHARED_1 u[dz][dy][dx] = CPANS_X(dx, dy, dz, e);

#define CONVECTION3DPA_NOSHARED_2                                              \
  Real_type Bu_ = 0.0;                                                         \
  Real_type Gu_ = 0.0;                                                         \
  for (Index_type dx = 0; dx < conv_ns::D1D; ++dx) {                           \
    const Real_type bx = CPANS_B(qx, dx);                                      \
    const Real_type gx = CPANS_G(qx, dx);                                      \
    const Real_type x = u[dz][dy][dx];                                         \
    Bu_ += bx * x;                                                             \
    Gu_ += gx * x;                                                             \
  }                                                                            \
  Bu[dz][dy][qx] = Bu_;                                                        \
  Gu[dz][dy][qx] = Gu_;

#define CONVECTION3DPA_NOSHARED_3                                              \
  Real_type BBu_ = 0.0;                                                        \
  Real_type GBu_ = 0.0;                                                        \
  Real_type BGu_ = 0.0;                                                        \
  for (Index_type dy = 0; dy < conv_ns::D1D; ++dy) {                           \
    const Real_type bx = CPANS_B(qy, dy);                                      \
    const Real_type gx = CPANS_G(qy, dy);                                      \
    BBu_ += bx * Bu[dz][dy][qx];                                               \
    GBu_ += gx * Bu[dz][dy][qx];                                               \
    BGu_ += bx * Gu[dz][dy][qx];                                               \
  }                                                                            \
  BBu[dz][qy][qx] = BBu_;                                                      \
  GBu[dz][qy][qx] = GBu_;                                                      \
  BGu[dz][qy][qx] = BGu_;

#define CONVECTION3DPA_NOSHARED_4                                              \
  Real_type GBBu_ = 0.0;                                                       \
  Real_type BGBu_ = 0.0;                                                       \
  Real_type BBGu_ = 0.0;                                                       \
  for (Index_type dz = 0; dz < conv_ns::D1D; ++dz) {                           \
    const Real_type bx = CPANS_B(qz, dz);                                      \
    const Real_type gx = CPANS_G(qz, dz);                                      \
    GBBu_ += gx * BBu[dz][qy][qx];                                             \
    BGBu_ += bx * GBu[dz][qy][qx];                                             \
    BBGu_ += bx * BGu[dz][qy][qx];                                             \
  }                                                                            \
  GBBu[qz][qy][qx] = GBBu_;                                                    \
  BGBu[qz][qy][qx] = BGBu_;                                                    \
  BBGu[qz][qy][qx] = BBGu_;

#define CONVECTION3DPA_NOSHARED_5                                              \
  const Real_type O1 = CPANS_op(qx, qy, qz, 0, e);                             \
  const Real_type O2 = CPANS_op(qx, qy, qz, 1, e);                             \
  const Real_type O3 = CPANS_op(qx, qy, qz, 2, e);                             \
  const Real_type gradX = BBGu[qz][qy][qx];                                    \
  const Real_type gradY = BGBu[qz][qy][qx];                                    \
  const Real_type gradZ = GBBu[qz][qy][qx];                                    \
  DGu[qz][qy][qx] = (O1 * gradX) + (O2 * gradY) + (O3 * gradZ);

#define CONVECTION3DPA_NOSHARED_6                                              \
  Real_type BDGu_ = 0.0;                                                       \
  for (Index_type qz = 0; qz < conv_ns::Q1D; ++qz) {                           \
    const Real_type w = CPANS_Bt(dz, qz);                                      \
    BDGu_ += w * DGu[qz][qy][qx];                                              \
  }                                                                            \
  BDGu[dz][qy][qx] = BDGu_;

#define CONVECTION3DPA_NOSHARED_7                                              \
  Real_type BBDGu_ = 0.0;                                                      \
  for (Index_type qy = 0; qy < conv_ns::Q1D; ++qy) {                           \
    const Real_type w = CPANS_Bt(dy, qy);                                      \
    BBDGu_ += w * BDGu[dz][qy][qx];                                            \
  }                                                                            \
  BBDGu[dz][dy][qx] = BBDGu_;

#define CONVECTION3DPA_NOSHARED_8                                              \
  Real_type BBBDGu = 0.0;                                                      \
  for (Index_type qx = 0; qx < conv_ns::Q1D; ++qx) {                           \
    const Real_type w = CPANS_Bt(dx, qx);                                      \
    BBBDGu += w * BBDGu[dz][dy][qx];                                           \
  }                                                                            \
  CPANS_Y(dx, dy, dz, e) += BBBDGu;

namespace rajaperf {
class RunParams;

namespace apps {

class CONVECTION3DPA_NOSHARED : public KernelBase {
public:
  CONVECTION3DPA_NOSHARED(const RunParams &params);

  ~CONVECTION3DPA_NOSHARED();

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
      conv_ns::Q1D * conv_ns::Q1D * conv_ns::Q1D;
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
