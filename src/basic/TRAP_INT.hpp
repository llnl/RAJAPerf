//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// TRAP_INT kernel reference implementation:
///
/// Real_type trap_int_func(Real_type x,
///                         Real_type y,
///                         Real_type xp,
///                         Real_type yp)
/// {
///    Real_type denom = (x - xp)*(x - xp) + (y - yp)*(y - yp);
///    denom = 1.0/sqrt(denom);
///    return denom;
/// }
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///    Real_type x = x0 + i*h;
///    sumx += trap_int_func(x, y, xp, yp);
/// }
///

#ifndef RAJAPerf_Basic_TRAP_INT_HPP
#define RAJAPerf_Basic_TRAP_INT_HPP


#define TRAP_INT_DATA_SETUP \
  Real_type x0 = m_x0; \
  Real_type xp = m_xp; \
  Real_type y = m_y; \
  Real_type yp = m_yp; \
  Real_type h = m_h;

#define TRAP_INT_BODY \
  Real_type x = x0 + i*h; \
  sumx += trap_int_func(x, y, xp, yp);


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class TRAP_INT : public KernelBase
{
public:

  TRAP_INT(const RunParams& params);

  ~TRAP_INT();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineOpenMPTargetVariantTunings();
  void defineKokkosVariantTunings();
  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runOpenMPTargetVariant(VariantID vid);
  void runKokkosVariant(VariantID vid);

  template < size_t tune_idx >
  void runSeqVariant(VariantID vid);

  template < size_t tune_idx >
  void runOpenMPVariant(VariantID vid);

  template < size_t block_size, typename MappingHelper >
  void runCudaVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runCudaVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runCudaVariantRAJANewReduce(VariantID vid);

  template < size_t block_size, typename MappingHelper >
  void runHipVariantBase(VariantID vid);
  template < size_t block_size, typename AlgorithmHelper, typename MappingHelper >
  void runHipVariantRAJA(VariantID vid);
  template < size_t block_size, typename MappingHelper >
  void runHipVariantRAJANewReduce(VariantID vid);

  template < size_t work_group_size >
  void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;

  Real_type m_x0;
  Real_type m_xp;
  Real_type m_y;
  Real_type m_yp;
  Real_type m_h;
  Real_type m_sumx_init;

  Real_type m_sumx;
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
