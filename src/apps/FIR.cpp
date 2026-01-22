//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIR.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace apps
{


FIR::FIR(const RunParams& params)
  : KernelBase(rajaperf::Apps_FIR, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(160);

  m_coefflen = FIR_COEFFLEN;

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

void FIR::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( m_coefflen*sizeof(Real_type) + // coeff
                      1*sizeof(Real_type) * (getActualProblemSize() + m_coefflen-1) ); // in
  setBytesWrittenPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // out
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep((2 * m_coefflen) * getActualProblemSize());
}

FIR::~FIR()
{
}

void FIR::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandValue(m_in, getActualProblemSize() + m_coefflen-1, vid);
  allocAndInitDataConst(m_out, getActualProblemSize(), 0.0, vid);
}

void FIR::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_out, getActualProblemSize(), vid);
}

void FIR::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_in, vid);
  deallocData(m_out, vid);
}

} // end namespace apps
} // end namespace rajaperf
