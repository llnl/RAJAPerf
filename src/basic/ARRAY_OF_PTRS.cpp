//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "ARRAY_OF_PTRS.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


ARRAY_OF_PTRS::ARRAY_OF_PTRS(const RunParams& params)
  : KernelBase(rajaperf::Basic_ARRAY_OF_PTRS, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  m_array_size = params.getArrayOfPtrsArraySize();

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void ARRAY_OF_PTRS::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( m_array_size*sizeof(Real_type) * getActualProblemSize() ); // x
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // y
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(m_array_size * getActualProblemSize());
}

ARRAY_OF_PTRS::~ARRAY_OF_PTRS()
{
}

void ARRAY_OF_PTRS::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_y, getActualProblemSize(), 0.0, vid);
  allocAndInitData(m_x, m_array_size*getActualProblemSize(), vid);
}

void ARRAY_OF_PTRS::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_y, getActualProblemSize(), vid);
}

void ARRAY_OF_PTRS::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_x, vid);
  deallocData(m_y, vid);
}

} // end namespace basic
} // end namespace rajaperf
