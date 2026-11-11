//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_MASS3DPA_NOSHARED_HPP
#define RAJAPerf_Apps_MASS3DPA_NOSHARED_HPP

#include "MASS3DPA.hpp"

// This kernel uses the MASS3DPA algorithm, thread decomposition, arithmetic,
// and synchronization. It relocates only the scratch storage from team shared
// memory to a global workspace, isolating the cost of the storage location.

namespace mpa_ns {
constexpr RAJA::Index_type MDQ = (mpa::Q1D > mpa::D1D) ? mpa::Q1D : mpa::D1D;
constexpr RAJA::Index_type SCRATCH_ELEM = 2 * MDQ * MDQ * MDQ;
constexpr RAJA::Index_type SCRATCH_BLOCK = mpa::Q1D * mpa::D1D;
} // namespace mpa_ns

#define MASS3DPA_NOSHARED_SMEM_DECL(TBATCH, elem_block)                        \
  constexpr Index_type MDQ = (MQ1 > MD1) ? MQ1 : MD1;                         \
  Real_ptr sDQ = Workspace + mpa_ns::SCRATCH_ELEM * NE +                      \
                 mpa_ns::SCRATCH_BLOCK * (elem_block);                        \
  Real_type(*Bsmem)[MD1] = (Real_type(*)[MD1])sDQ;                            \
  Real_type(*Btsmem)[MQ1] = (Real_type(*)[MQ1])sDQ;                           \
  Real_ptr sm0_base = Workspace;                                              \
  Real_ptr sm1_base = Workspace + MDQ * MDQ * MDQ;

#define MASS3DPA_NOSHARED_SMEM_SLICE(e)                                       \
  Real_ptr sm0 = sm0_base + mpa_ns::SCRATCH_ELEM * (e);                       \
  Real_ptr sm1 = sm1_base + mpa_ns::SCRATCH_ELEM * (e);                       \
  [[maybe_unused]] Real_type(*Xsmem)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm0; \
  [[maybe_unused]] Real_type(*DDQ)[MD1][MQ1] = (Real_type(*)[MD1][MQ1])sm1;   \
  [[maybe_unused]] Real_type(*DQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm0;   \
  [[maybe_unused]] Real_type(*QQQ)[MQ1][MQ1] = (Real_type(*)[MQ1][MQ1])sm1;   \
  [[maybe_unused]] Real_type(*QQD)[MQ1][MD1] = (Real_type(*)[MQ1][MD1])sm0;   \
  [[maybe_unused]] Real_type(*QDD)[MD1][MD1] = (Real_type(*)[MD1][MD1])sm1;

#define MASS3DPA_NOSHARED_DATA_SETUP                                          \
  Real_ptr B = m_B;                                                           \
  Real_ptr Bt = m_Bt;                                                         \
  Real_ptr D = m_D;                                                           \
  Real_ptr X = m_X;                                                           \
  Real_ptr Y = m_Y;                                                           \
  Real_ptr Workspace = m_Workspace;                                           \
  Index_type NE = m_NE;

namespace rajaperf {
class RunParams;
namespace apps {

class MASS3DPA_NOSHARED : public KernelBase {
public:
  MASS3DPA_NOSHARED(const RunParams& params);
  ~MASS3DPA_NOSHARED();

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
      mpa::Q1D * mpa::Q1D * mpa::TBATCH;
  using gpu_block_sizes_type =
      integer::make_gpu_block_size_list_type<default_gpu_block_size,
                                             MASS3DPAValidGPUBlockSize>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;
  Real_ptr m_Workspace;
  Index_type m_NE;
};

} // namespace apps
} // namespace rajaperf

#endif
