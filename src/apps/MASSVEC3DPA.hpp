//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Action of 3D mass matrix via partial assembly
///
/// for (int e = 0; e < NE; ++e) {
///
/// } // element loop
///

#ifndef RAJAPerf_Apps_MASSVEC3DPA_HPP
#define RAJAPerf_Apps_MASSVEC3DPA_HPP

#define MASSVEC3DPA_DATA_SETUP \
Real_ptr B = m_B; \
Real_ptr Bt = m_Bt; \
Real_ptr D = m_D; \
Real_ptr X = m_X; \
Real_ptr Y = m_Y; \
Index_type NE = m_NE;

#include "common/KernelBase.hpp"
#include "FEM_MACROS.hpp"

#include "RAJA/RAJA.hpp"

//Number of Dofs/Qpts in 1D
#define MVPA_D1D 2
#define MVPA_Q1D 3
#define MVDIM 3
#define mvpaB_(x, y) B[x + MVPA_Q1D * y]
#define mvpaBt_(x, y) Bt[x + MVPA_D1D * y]
#define mvpaX_(dx, dy, dz, c, e)                                           \
  X[dx + MVPA_D1D * dy + MVPA_D1D * MVPA_D1D * dz + MVPA_D1D * MVPA_D1D * MVPA_D1D * c +  MVPA_D1D * MVPA_D1D * MVPA_D1D * MVDIM * e]
#define mvpaY_(dx, dy, dz, c, e)                                             \
  Y[dx + MVPA_D1D * dy + MVPA_D1D * MVPA_D1D * dz + MVPA_D1D * MVPA_D1D * MVPA_D1D * c + MVPA_D1D * MVPA_D1D * MVPA_D1D * MVDIM * e]
#define mvpaD_(qx, qy, qz, e)                                             \
  D[qx + MVPA_Q1D * qy + MVPA_Q1D * MVPA_Q1D * qz + MVPA_Q1D * MVPA_Q1D * MVPA_Q1D * e ]

#define MASSVEC3DPA_0_CPU           \
        constexpr int MQ1 = MVPA_Q1D; \
        constexpr int MD1 = MVPA_D1D; \
        constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1; \
        double sDQ[MQ1 * MD1]; \
        double(*Bsmem)[MD1] = (double(*)[MD1])sDQ; \
        double(*Btsmem)[MQ1] = (double(*)[MQ1])sDQ; \
        double sm0[MDQ * MDQ * MDQ]; \
        double sm1[MDQ * MDQ * MDQ]; \
        double(*Xsmem)[MD1][MD1] = (double(*)[MD1][MD1])sm0; \
        double(*DDQ)[MD1][MQ1] = (double(*)[MD1][MQ1])sm1; \
        double(*DQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm0; \
        double(*QQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm1; \
        double(*QQD)[MQ1][MD1] = (double(*)[MQ1][MD1])sm0; \
        double(*QDD)[MD1][MD1] = (double(*)[MD1][MD1])sm1;

#define MASSVEC3DPA_0_GPU \
        constexpr int MQ1 = MVPA_Q1D; \
        constexpr int MD1 = MVPA_D1D; \
        constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1; \
        RAJA_TEAM_SHARED  double sDQ[MQ1 * MD1];     \
        double(*Bsmem)[MD1] = (double(*)[MD1])sDQ; \
        double(*Btsmem)[MQ1] = (double(*)[MQ1])sDQ; \
        RAJA_TEAM_SHARED double sm0[MDQ * MDQ * MDQ];       \
        RAJA_TEAM_SHARED double sm1[MDQ * MDQ * MDQ];      \
        double(*Xsmem)[MD1][MD1] = (double(*)[MD1][MD1])sm0; \
        double(*DDQ)[MD1][MQ1] = (double(*)[MD1][MQ1])sm1; \
        double(*DQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm0; \
        double(*QQQ)[MQ1][MQ1] = (double(*)[MQ1][MQ1])sm1; \
        double(*QQD)[MQ1][MD1] = (double(*)[MQ1][MD1])sm0; \
        double(*QDD)[MD1][MD1] = (double(*)[MD1][MD1])sm1;

#define MASSVEC3DPA_1 \


#define MASSVEC3DPA_2 \

// 2 * MVPA_D1D * MVPA_D1D * MVPA_D1D * MVPA_Q1D
#define MASSVEC3DPA_3 \


//2 * MVPA_D1D * MVPA_D1D * MVPA_Q1D * MVPA_Q1D
#define MASSVEC3DPA_4 \

//2 * MVPA_D1D * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D + MVPA_Q1D * MVPA_Q1D * MVPA_Q1D
#define MASSVEC3DPA_5 \

#define MASSVEC3DPA_6 \


//2 * MVPA_Q1D * MVPA_Q1D * MVPA_Q1D * MVPA_D1D
#define MASSVEC3DPA_7 \


// 2 * MVPA_Q1D * MVPA_Q1D * MVPA_D1D * MVPA_D1D
#define MASSVEC3DPA_8 \

//2 * MVPA_Q1D * MVPA_D1D * MVPA_D1D * MVPA_D1D + MVPA_D1D * MVPA_D1D * MVPA_D1D
#define MASSVEC3DPA_9 \


namespace rajaperf
{
class RunParams;

namespace apps
{

class MASSVEC3DPA : public KernelBase
{
public:

  MASSVEC3DPA(const RunParams& params);

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

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = MVPA_Q1D * MVPA_Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_NE;
  Index_type m_NE_default;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
