//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// REDUCE_STRUCT kernel reference implementation:
///
/// Real_type xsum = m_sum_init; Real_type ysum = m_sum_init;
/// Real_type xmin = m_min_init; Real_type ymin = m_min_init;
/// Real_type xmax = m_max_init; Real_type ymax = m_max_init;

///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   xsum += x[i] ; ysum += y[i] ;
///   xmin = RAJA_MIN(xmin, x[i]) ; xmax = RAJA_MAX(xmax, x[i]) ;
///   ymin = RAJA_MIN(ymin, y[i]) ; ymax = RAJA_MAX(ymax, y[i]) ;
/// }
///
/// points.xcenter = xsum;
/// points.xcenter /= points.N
/// points.xmin = xmin;
/// points.xmax = xmax;
/// points.ycenter = ysum;
/// points.ycenter /= points.N
/// points.ymin = ymin;
/// points.ymax = ymax;

///
/// RAJA_MIN/MAX are macros that do what you would expect.
///

#ifndef RAJAPerf_Basic_REDUCE_STRUCT_HPP
#define RAJAPerf_Basic_REDUCE_STRUCT_HPP


#define REDUCE_STRUCT_DATA_SETUP \
  PointsType points; \
  points.N = getActualProblemSize(); \
  Real_ptr x = m_x; \
  Real_ptr y = m_y;

#define REDUCE_STRUCT_BODY  \
  xsum += x[i] ; \
  xmin = RAJA_MIN(xmin, x[i]) ; \
  xmax = RAJA_MAX(xmax, x[i]) ; \
  ysum += y[i] ; \
  ymin = RAJA_MIN(ymin, y[i]) ; \
  ymax = RAJA_MAX(ymax, y[i]) ;

#define REDUCE_STRUCT_BODY_RAJA  \
  xsum += x[i] ; \
  xmin.min(x[i]) ; \
  xmax.max(x[i]) ; \
  ysum += y[i] ; \
  ymin.min(y[i]) ; \
  ymax.max(y[i]) ;


#include "common/KernelBase.hpp"

namespace rajaperf
{
class RunParams;

namespace basic
{

class REDUCE_STRUCT : public KernelBase
{
public:

  REDUCE_STRUCT(const RunParams& params);

  ~REDUCE_STRUCT();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineOpenMPTargetVariantTunings();
  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();

  void runOpenMPTargetVariant(VariantID vid);

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

  struct PointsType {
    Index_type N;

    Real_ptr GetCenter(){return &center[0];};
    Real_type GetXMax(){return xmax;};
    Real_type GetXMin(){return xmin;};
    Real_type GetYMax(){return ymax;};
    Real_type GetYMin(){return ymin;};
    void SetCenter(Real_type xval, Real_type yval){this->center[0]=xval, this->center[1]=yval;};
    void SetXMin(Real_type val){this->xmin=val;};
    void SetXMax(Real_type val){this->xmax=val;};
    void SetYMin(Real_type val){this->ymin=val;};
    void SetYMax(Real_type val){this->ymax=val;};              
        
    //results
    private:
    Real_type center[2] = {0.0,0.0};
    Real_type xmin, xmax;
    Real_type ymin, ymax;
  }; 

private:
  static const size_t default_gpu_block_size = 256;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size>;
  Real_ptr m_x; Real_ptr m_y;
  Real_type	m_init_sum; 
  Real_type	m_init_min; 
  Real_type	m_init_max; 
  PointsType m_points;
  Real_type X_MIN = 0.0, X_MAX = 100.0; 
  Real_type Y_MIN = 0.0, Y_MAX = 50.0; 
  Real_type Lx = (X_MAX) - (X_MIN); 
  Real_type Ly = (Y_MAX) - (Y_MIN);
 
};

} // end namespace basic
} // end namespace rajaperf

#endif // closing endif for header file include guard
