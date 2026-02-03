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
/// via partial assembly and sum factorization
///
/// Based on MFEM's/CEED algorithms.
/// Reference implementation - MFEM-v4.9
/// https://github.com/mfem/mfem/blob/v4.9/fem/integ/bilininteg_convection_kernels.hpp
///
///
/// for(Index_type e = 0; e < NE; ++e) {
///
///   constexpr Index_type max_D1D = conv::D1D;
///   constexpr Index_type max_Q1D = conv::Q1D;
///   constexpr Index_type max_DQ = (max_Q1D > max_D1D) ? max_Q1D : max_D1D;
///   MFEM_SHARED Real_type sm0[max_DQ*max_DQ*max_DQ];
///   MFEM_SHARED Real_type sm1[max_DQ*max_DQ*max_DQ];
///   MFEM_SHARED Real_type sm2[max_DQ*max_DQ*max_DQ];
///   MFEM_SHARED Real_type sm3[max_DQ*max_DQ*max_DQ];
///   MFEM_SHARED Real_type sm4[max_DQ*max_DQ*max_DQ];
///   MFEM_SHARED Real_type sm5[max_DQ*max_DQ*max_DQ];
///
///   Real_type (*u)[max_D1D][max_D1D] = (Real_type (*)[max_D1D][max_D1D]) sm0;
///   for(Index_type dz = 0; dz < conv::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv::D1D; ++dy)
///     {
///       for(Index_type dx = 0; dx < conv::D1D; ++dx)
///       {
///         u[dz][dy][dx] = CPA_X(dx,dy,dz,e);
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*Bu)[max_D1D][max_Q1D] = (Real_type (*)[max_D1D][max_Q1D])sm1;
///   Real_type (*Gu)[max_D1D][max_Q1D] = (Real_type (*)[max_D1D][max_Q1D])sm2;
///   for(Index_type dz = 0; dz < conv::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv::D1D; ++dy)
///     {
///       for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///       {
///         Real_type Bu_ = 0.0;
///         Real_type Gu_ = 0.0;
///         for(Index_type dx = 0; dx < conv::D1D; ++dx)
///         {
///           const Real_type bx = CPA_B(qx,dx);
///           const Real_type gx = CPA_G(qx,dx);
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
///   for(Index_type dz = 0; dz < conv::D1D; ++dz)
///   {
///     for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///     {
///       for(Index_type qy = 0; qy < conv::Q1D; ++qy)
///       {
///         Real_type BBu_ = 0.0;
///         Real_type GBu_ = 0.0;
///         Real_type BGu_ = 0.0;
///         for(Index_type dy = 0; dy < conv::D1D; ++dy)
///         {
///           const Real_type bx = CPA_B(qy,dy);
///           const Real_type gx = CPA_G(qy,dy);
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
///   for(Index_type qx = 0; qx <conv::Q1D; ++qx)
///   {
///     for(Index_type qy = 0; qy < conv::Q1D; ++qy)
///     {
///       for(Index_type qz = 0; qz < conv::Q1D; ++qz)
///       {
///         Real_type GBBu_ = 0.0;
///         Real_type BGBu_ = 0.0;
///         Real_type BBGu_ = 0.0;
///         for(Index_type dz = 0; dz < conv::D1D; ++dz)
///         {
///           const Real_type bx = CPA_B(qz,dz);
///           const Real_type gx = CPA_G(qz,dz);
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
///   for(Index_type qz = 0; qz < conv::Q1D; ++qz)
///   {
///     for(Index_type qy = 0; qy < conv::Q1D; ++qy)
///     {
///       for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///       {
///         const Real_type O1 = CPA_op(qx,qy,qz,0,e);
///         const Real_type O2 = CPA_op(qx,qy,qz,1,e);
///         const Real_type O3 = CPA_op(qx,qy,qz,2,e);
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
///   (*)[max_Q1D][max_Q1D])sm4; for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///   {
///     for(Index_type qy = 0; qy < conv::Q1D; ++qy)
///     {
///       for(Index_type dz = 0; dz < conv::D1D; ++dz)
///       {
///          Real_type BDGu_ = 0.0;
///          for(Index_type qz = 0; qz < conv::Q1D; ++qz)
///          {
///             const Real_type w = CPA_Bt(dz,qz);
///             BDGu_ += w * DGu[qz][qy][qx];
///          }
///          BDGu[dz][qy][qx] = BDGu_;
///       }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   Real_type (*BBDGu)[max_D1D][max_Q1D] = (Real_type
///   (*)[max_D1D][max_Q1D])sm5; for(Index_type dz = 0; dz < conv::D1D; ++dz)
///   {
///     for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///      {
///        for(Index_type dy = 0; dy < conv::D1D; ++dy)
///         {
///            Real_type BBDGu_ = 0.0;
///            for(Index_type qy = 0; qy < conv::Q1D; ++qy)
///            {
///              const Real_type w = CPA_Bt(dy,qy);
///              BBDGu_ += w * BDGu[dz][qy][qx];
///           }
///           BBDGu[dz][dy][qx] = BBDGu_;
///        }
///     }
///   }
///   MFEM_SYNC_THREAD;
///   for(Index_type dz = 0; dz < conv::D1D; ++dz)
///   {
///     for(Index_type dy = 0; dy < conv::D1D; ++dy)
///     {
///       for(Index_type dx = 0; dx < conv::D1D; ++dx)
///       {
///         Real_type BBBDGu = 0.0;
///         for(Index_type qx = 0; qx < conv::Q1D; ++qx)
///         {
///           const Real_type w = CPA_Bt(dx,qx);
///           BBBDGu += w * BBDGu[dz][dy][qx];
///         }
///         CPA_Y(dx,dy,dz,e) += BBBDGu;
///       }
///     }
///   }
/// } // element loop
///

#ifndef RAJAPerf_Apps_CONVECTION3DPA_HPP
#define RAJAPerf_Apps_CONVECTION3DPA_HPP

#define CONVECTION3DPA_DATA_SETUP                                              \
  Real_ptr Basis = m_B;                                                        \
  Real_ptr tBasis = m_Bt;                                                      \
  Real_ptr dBasis = m_G;                                                       \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace conv {
constexpr RAJA::Index_type D1D = 3;
constexpr RAJA::Index_type Q1D = 4;
constexpr RAJA::Index_type VDIM = 3;
} // namespace conv

#define CPA_B(x, y) Basis[x + conv::Q1D * y]
#define CPA_Bt(x, y) tBasis[x + conv::D1D * y]
#define CPA_G(x, y) dBasis[x + conv::Q1D * y]
#define CPA_X(dx, dy, dz, e)                                                   \
  X[dx + conv::D1D * dy + conv::D1D * conv::D1D * dz +                         \
    conv::D1D * conv::D1D * conv::D1D * e]
#define CPA_Y(dx, dy, dz, e)                                                   \
  Y[dx + conv::D1D * dy + conv::D1D * conv::D1D * dz +                         \
    conv::D1D * conv::D1D * conv::D1D * e]
#define CPA_op(qx, qy, qz, d, e)                                               \
  D[qx + conv::Q1D * qy + conv::Q1D * conv::Q1D * qz +                         \
    conv::Q1D * conv::Q1D * conv::Q1D * d +                                    \
    conv::VDIM * conv::Q1D * conv::Q1D * conv::Q1D * e]

#define CONVECTION3DPA_0_GPU                                                   \
  constexpr Index_type max_D1D = conv::D1D;                                    \
  constexpr Index_type max_Q1D = conv::Q1D;                                    \
  constexpr Index_type max_DQ = (max_Q1D > max_D1D) ? max_Q1D : max_D1D;       \
  RAJA_TEAM_SHARED Real_type sm0[max_DQ * max_DQ * max_DQ];                    \
  RAJA_TEAM_SHARED Real_type sm1[max_DQ * max_DQ * max_DQ];                    \
  RAJA_TEAM_SHARED Real_type sm2[max_DQ * max_DQ * max_DQ];                    \
  RAJA_TEAM_SHARED Real_type sm3[max_DQ * max_DQ * max_DQ];                    \
  RAJA_TEAM_SHARED Real_type sm4[max_DQ * max_DQ * max_DQ];                    \
  RAJA_TEAM_SHARED Real_type sm5[max_DQ * max_DQ * max_DQ];                    \
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

#define CONVECTION3DPA_0_CPU                                                   \
  constexpr Index_type max_D1D = conv::D1D;                                    \
  constexpr Index_type max_Q1D = conv::Q1D;                                    \
  constexpr Index_type max_DQ = (max_Q1D > max_D1D) ? max_Q1D : max_D1D;       \
  Real_type sm0[max_DQ * max_DQ * max_DQ];                                     \
  Real_type sm1[max_DQ * max_DQ * max_DQ];                                     \
  Real_type sm2[max_DQ * max_DQ * max_DQ];                                     \
  Real_type sm3[max_DQ * max_DQ * max_DQ];                                     \
  Real_type sm4[max_DQ * max_DQ * max_DQ];                                     \
  Real_type sm5[max_DQ * max_DQ * max_DQ];                                     \
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

#define CONVECTION3DPA_1 u[dz][dy][dx] = CPA_X(dx, dy, dz, e);

#define CONVECTION3DPA_2                                                       \
  Real_type Bu_ = 0.0;                                                         \
  Real_type Gu_ = 0.0;                                                         \
  for (Index_type dx = 0; dx < conv::D1D; ++dx) {                              \
    const Real_type bx = CPA_B(qx, dx);                                        \
    const Real_type gx = CPA_G(qx, dx);                                        \
    const Real_type x = u[dz][dy][dx];                                         \
    Bu_ += bx * x;                                                             \
    Gu_ += gx * x;                                                             \
  }                                                                            \
  Bu[dz][dy][qx] = Bu_;                                                        \
  Gu[dz][dy][qx] = Gu_;

#define CONVECTION3DPA_3                                                       \
  Real_type BBu_ = 0.0;                                                        \
  Real_type GBu_ = 0.0;                                                        \
  Real_type BGu_ = 0.0;                                                        \
  for (Index_type dy = 0; dy < conv::D1D; ++dy) {                              \
    const Real_type bx = CPA_B(qy, dy);                                        \
    const Real_type gx = CPA_G(qy, dy);                                        \
    BBu_ += bx * Bu[dz][dy][qx];                                               \
    GBu_ += gx * Bu[dz][dy][qx];                                               \
    BGu_ += bx * Gu[dz][dy][qx];                                               \
  }                                                                            \
  BBu[dz][qy][qx] = BBu_;                                                      \
  GBu[dz][qy][qx] = GBu_;                                                      \
  BGu[dz][qy][qx] = BGu_;

#define CONVECTION3DPA_4                                                       \
  Real_type GBBu_ = 0.0;                                                       \
  Real_type BGBu_ = 0.0;                                                       \
  Real_type BBGu_ = 0.0;                                                       \
  for (Index_type dz = 0; dz < conv::D1D; ++dz) {                              \
    const Real_type bx = CPA_B(qz, dz);                                        \
    const Real_type gx = CPA_G(qz, dz);                                        \
    GBBu_ += gx * BBu[dz][qy][qx];                                             \
    BGBu_ += bx * GBu[dz][qy][qx];                                             \
    BBGu_ += bx * BGu[dz][qy][qx];                                             \
  }                                                                            \
  GBBu[qz][qy][qx] = GBBu_;                                                    \
  BGBu[qz][qy][qx] = BGBu_;                                                    \
  BBGu[qz][qy][qx] = BBGu_;

#define CONVECTION3DPA_5                                                       \
  const Real_type O1 = CPA_op(qx, qy, qz, 0, e);                               \
  const Real_type O2 = CPA_op(qx, qy, qz, 1, e);                               \
  const Real_type O3 = CPA_op(qx, qy, qz, 2, e);                               \
  const Real_type gradX = BBGu[qz][qy][qx];                                    \
  const Real_type gradY = BGBu[qz][qy][qx];                                    \
  const Real_type gradZ = GBBu[qz][qy][qx];                                    \
  DGu[qz][qy][qx] = (O1 * gradX) + (O2 * gradY) + (O3 * gradZ);

#define CONVECTION3DPA_6                                                       \
  Real_type BDGu_ = 0.0;                                                       \
  for (Index_type qz = 0; qz < conv::Q1D; ++qz) {                              \
    const Real_type w = CPA_Bt(dz, qz);                                        \
    BDGu_ += w * DGu[qz][qy][qx];                                              \
  }                                                                            \
  BDGu[dz][qy][qx] = BDGu_;

#define CONVECTION3DPA_7                                                       \
  Real_type BBDGu_ = 0.0;                                                      \
  for (Index_type qy = 0; qy < conv::Q1D; ++qy) {                              \
    const Real_type w = CPA_Bt(dy, qy);                                        \
    BBDGu_ += w * BDGu[dz][qy][qx];                                            \
  }                                                                            \
  BBDGu[dz][dy][qx] = BBDGu_;

#define CONVECTION3DPA_8                                                       \
  Real_type BBBDGu = 0.0;                                                      \
  for (Index_type qx = 0; qx < conv::Q1D; ++qx) {                              \
    const Real_type w = CPA_Bt(dx, qx);                                        \
    BBBDGu += w * BBDGu[dz][dy][qx];                                           \
  }                                                                            \
  CPA_Y(dx, dy, dz, e) += BBBDGu;

namespace rajaperf {
class RunParams;

namespace apps {

class CONVECTION3DPA : public KernelBase {
public:
  CONVECTION3DPA(const RunParams &params);

  ~CONVECTION3DPA();

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
      conv::Q1D * conv::Q1D * conv::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_G;
  Real_ptr m_Gt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_NE;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
