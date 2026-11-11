//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_MASS3DEA_NOSHARED_HPP
#define RAJAPerf_Apps_MASS3DEA_NOSHARED_HPP

#include "MASS3DEA.hpp"

// This kernel uses the MASS3DEA algorithm, thread decomposition, arithmetic,
// and synchronization. It relocates only the scratch storage from team shared
// memory to a global workspace, isolating the cost of the storage location.

namespace mea_ns {
constexpr RAJA::Index_type SCRATCH =
    mea::Q1D * mea::D1D + mea::Q1D * mea::Q1D * mea::Q1D;
} // namespace mea_ns

#define MASS3DEA_NOSHARED_0                                                   \
  Real_ptr ea_slice = Workspace + mea_ns::SCRATCH * e;                        \
  Real_type(*s_B)[mea::D1D] = (Real_type(*)[mea::D1D])ea_slice;

#define MASS3DEA_NOSHARED_2                                                   \
  Real_type(*s_D)[mea::Q1D][mea::Q1D] =                                      \
      (Real_type(*)[mea::Q1D][mea::Q1D])(ea_slice + mea::Q1D * mea::D1D);

#define MASS3DEA_NOSHARED_DATA_SETUP                                          \
  Real_ptr B = m_B;                                                           \
  Real_ptr D = m_D;                                                           \
  Real_ptr M = m_M;                                                           \
  Real_ptr Workspace = m_Workspace;                                           \
  Index_type NE = m_NE;

namespace rajaperf {
class RunParams;
namespace apps {

class MASS3DEA_NOSHARED : public KernelBase {
public:
  MASS3DEA_NOSHARED(const RunParams& params);
  ~MASS3DEA_NOSHARED();

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

  template <size_t block_size, size_t tune_idx>
  void runCudaVariantImpl(VariantID vid);
  template <size_t block_size, size_t tune_idx>
  void runHipVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size =
      mea::D1D * mea::D1D * mea::D1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_D;
  Real_ptr m_M;
  Real_ptr m_Workspace;
  Index_type m_NE;
};

} // namespace apps
} // namespace rajaperf

#endif
