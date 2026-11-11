//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DIFFUSION3DPA_NOSHARED.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


DIFFUSION3DPA_NOSHARED::DIFFUSION3DPA_NOSHARED(const RunParams& params)
  : KernelBase(rajaperf::Apps_DIFFUSION3DPA_NOSHARED, params)
{
  Index_type NE_default = 15625;
  setDefaultProblemSize(NE_default*diff_ns::D1D*diff_ns::D1D*diff_ns::D1D);
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(3);

  setUsesFeature(Launch);

  addVariantTunings();
}

void DIFFUSION3DPA_NOSHARED::setSize(Index_type target_size, Index_type target_reps)
{
  m_NE = std::max((target_size + (diff_ns::D1D*diff_ns::D1D*diff_ns::D1D)/2) / (diff_ns::D1D*diff_ns::D1D*diff_ns::D1D), Index_type(1));

  setActualProblemSize( m_NE*diff_ns::D1D*diff_ns::D1D*diff_ns::D1D );
  setRunReps( target_reps );

  setItsPerRep( m_NE*diff_ns::D1D*diff_ns::D1D*diff_ns::D1D );
  setKernelsPerRep(1);

  //
  // Every byte and FLOP count below is deliberately identical to
  // DIFFUSION3DPA's, and the global workspace is excluded from all of them,
  // allocated bytes included. The two kernels run the same algorithm over the
  // same inputs and differ only in where the per-element scratch lives; the
  // workspace traffic is the overhead under study. Identical accounting means
  // every sizing flag (--size, --memory-allocated, --memory-touched,
  // --memory-moved) gives both kernels the same problem size, so any pair of
  // runs is directly comparable. The workspace's real footprint is
  // SCRATCH * m_NE * sizeof(Real_type) bytes on top of what is reported.
  //
  setBytesAllocatedPerRep( 2*sizeof(Real_type) * diff_ns::Q1D*diff_ns::D1D + // b, g
           diff_ns::DPA_SYM*sizeof(Real_type) * diff_ns::Q1D*diff_ns::Q1D*diff_ns::Q1D*m_NE + // d
                           2*sizeof(Real_type) * diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE ); // x, y
  setBytesReadPerRep( 2*sizeof(Real_type) * diff_ns::Q1D*diff_ns::D1D + // b, g
                      1*sizeof(Real_type) * diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE + // x
      diff_ns::DPA_SYM*sizeof(Real_type) * diff_ns::Q1D*diff_ns::Q1D*diff_ns::Q1D*m_NE ); // d
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE ); // y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (4 * diff_ns::D1D * diff_ns::D1D * diff_ns::D1D * diff_ns::Q1D + //DIFFUSION3DPA_NOSHARED_3
                         6 * diff_ns::D1D * diff_ns::D1D * diff_ns::Q1D * diff_ns::Q1D + //DIFFUSION3DPA_NOSHARED_4
                         (6 * diff_ns::D1D  + 15) * diff_ns::Q1D * diff_ns::Q1D * diff_ns::Q1D + //DIFFUSION3DPA_NOSHARED_5
                         (6 * diff_ns::Q1D) * diff_ns::D1D * diff_ns::Q1D * diff_ns::Q1D + //DIFFUSION3DPA_NOSHARED_7
                         (6 * diff_ns::Q1D) * diff_ns::D1D * diff_ns::D1D * diff_ns::Q1D + //DIFFUSION3DPA_NOSHARED_8
                         (6 * diff_ns::Q1D + 3) * diff_ns::D1D * diff_ns::D1D * diff_ns::D1D)); //DIFFUSION3DPA_NOSHARED_9
}

DIFFUSION3DPA_NOSHARED::~DIFFUSION3DPA_NOSHARED()
{
}

void DIFFUSION3DPA_NOSHARED::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B, diff_ns::Q1D*diff_ns::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_G, diff_ns::Q1D*diff_ns::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, diff_ns::Q1D*diff_ns::Q1D*diff_ns::Q1D*diff_ns::DPA_SYM*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X, diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE, Real_type(0.0), vid);
  allocData(m_Workspace, diff_ns::SCRATCH*m_NE, vid);
}

void DIFFUSION3DPA_NOSHARED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Y, diff_ns::D1D*diff_ns::D1D*diff_ns::D1D*m_NE, vid);
}

void DIFFUSION3DPA_NOSHARED::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_G, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
  deallocData(m_Workspace, vid);
}

} // end namespace apps
} // end namespace rajaperf
