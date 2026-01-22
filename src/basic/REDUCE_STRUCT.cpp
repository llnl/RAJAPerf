//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "REDUCE_STRUCT.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <limits>

namespace rajaperf
{
namespace basic
{


REDUCE_STRUCT::REDUCE_STRUCT(const RunParams& params)
  : KernelBase(rajaperf::Basic_REDUCE_STRUCT, params)
{
  setDefaultProblemSize(1000000);
//setDefaultReps(5000);
// Set reps to low value until we resolve RAJA omp-target
// reduction performance issues
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Inconsistent);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Reduction);

  addVariantTunings();
}

void REDUCE_STRUCT::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 2*sizeof(Real_type) * getActualProblemSize() ); // x, y
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * getActualProblemSize() + 2);
}

REDUCE_STRUCT::~REDUCE_STRUCT()
{
}

void REDUCE_STRUCT::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  m_init_sum = 0.0;
  m_init_min = std::numeric_limits<Real_type>::max();
  m_init_max = std::numeric_limits<Real_type>::lowest();

  auto reset_x = allocAndInitDataForInit(m_x, getActualProblemSize(), vid);
  auto reset_y = allocAndInitDataForInit(m_y, getActualProblemSize(), vid);

  Real_type dx = Lx/(Real_type)(getActualProblemSize());
  Real_type dy = Ly/(Real_type)(getActualProblemSize());
  for (int i=0;i<getActualProblemSize();i++){ \
    m_x[i] = i*dx;
    m_y[i] = i*dy;
  }
}

void REDUCE_STRUCT::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_points.GetCenter()[0]);
  addToChecksum(m_points.GetXMin());
  addToChecksum(m_points.GetXMax());
  addToChecksum(m_points.GetCenter()[1]);
  addToChecksum(m_points.GetYMin());
  addToChecksum(m_points.GetYMax());

  return;
}

void REDUCE_STRUCT::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
}

} // end namespace basic
} // end namespace rajaperf
