//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// POLYBENCH_FDTD_2D kernel reference implementation:
///
/// // removed loop [0, TSTEPS)
///
/// for (j = 0; j < ny; j++) {
///   ey[0][j] = fict[t];
/// }
/// for (i = 1; i < nx; i++) {
///   for (j = 0; j < ny; j++) {
///     ey[i][j] = ey[i][j] - 0.5*(hz[i][j]-hz[i-1][j]);
///   }
/// }
/// for (i = 0; i < nx; i++) {
///   for (j = 1; j < ny; j++) {
///     ex[i][j] = ex[i][j] - 0.5*(hz[i][j]-hz[i][j-1]);
///   }
/// }
/// for (i = 0; i < nx - 1; i++) {
///   for (j = 0; j < ny - 1; j++) {
///     hz[i][j] = hz[i][j] - 0.7*(ex[i][j+1] - ex[i][j] +
///                                ey[i+1][j] - ey[i][j]);
///   }
/// }
/// // removed end loop


#ifndef RAJAPerf_POLYBENCH_FDTD_2D_HPP
#define RAJAPerf_POLYBENCH_FDTD_2D_HPP


#define POLYBENCH_FDTD_2D_DATA_SETUP \
  Index_type t = 0; \
  const Index_type nx = m_nx; \
  const Index_type ny = m_ny; \
\
  Real_ptr fict = m_fict; \
  Real_ptr ex = m_ex; \
  Real_ptr ey = m_ey; \
  Real_ptr hz = m_hz;


#define POLYBENCH_FDTD_2D_BODY1 \
  ey[j + 0*ny] = fict[t];

#define POLYBENCH_FDTD_2D_BODY2 \
  ey[j + i*ny] = ey[j + i*ny] - 0.5*(hz[j + i*ny] - hz[j + (i-1)*ny]);

#define POLYBENCH_FDTD_2D_BODY3 \
  ex[j + i*ny] = ex[j + i*ny] - 0.5*(hz[j + i*ny] - hz[j-1 + i*ny]);

#define POLYBENCH_FDTD_2D_BODY4 \
  hz[j + i*ny] = hz[j + i*ny] - 0.7*(ex[j+1 + i*ny] - ex[j + i*ny] + \
                                     ey[j + (i+1)*ny] - ey[j + i*ny]);


#define POLYBENCH_FDTD_2D_BODY1_RAJA \
  eyview(0, j) = fict[t];

#define POLYBENCH_FDTD_2D_BODY2_RAJA \
  eyview(i, j) = eyview(i, j) - 0.5*(hzview(i, j) - hzview(i-1, j));

#define POLYBENCH_FDTD_2D_BODY3_RAJA \
  exview(i, j) = exview(i, j) - 0.5*(hzview(i, j) - hzview(i, j-1));

#define POLYBENCH_FDTD_2D_BODY4_RAJA \
  hzview(i, j) = hzview(i, j) - 0.7*(exview(i, j+1) - exview(i, j) + \
                                     eyview(i+1, j) - eyview(i, j));


#define POLYBENCH_FDTD_2D_VIEWS_RAJA \
using VIEW_TYPE = RAJA::View<Real_type, \
                             RAJA::Layout<2, Index_type, 1>>; \
\
  VIEW_TYPE exview(ex, RAJA::Layout<2>(nx, ny)); \
  VIEW_TYPE eyview(ey, RAJA::Layout<2>(nx, ny)); \
  VIEW_TYPE hzview(hz, RAJA::Layout<2>(nx, ny));


#include "common/KernelBase.hpp"

namespace rajaperf
{

class RunParams;

namespace polybench
{

class POLYBENCH_FDTD_2D : public KernelBase
{
public:

  POLYBENCH_FDTD_2D(const RunParams& params);

  ~POLYBENCH_FDTD_2D();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineOpenMPTargetVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);
  void runOpenMPTargetVariant(VariantID vid);
 
  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);
  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size,
                                                         integer::MultipleOf<32>>;

  Index_type m_nx;
  Index_type m_ny;
  Index_type m_tsteps;

  Real_ptr m_fict;
  Real_ptr m_ex;
  Real_ptr m_ey;
  Real_ptr m_hz;
};

} // end namespace polybench
} // end namespace rajaperf

#endif // closing endif for header file include guard
