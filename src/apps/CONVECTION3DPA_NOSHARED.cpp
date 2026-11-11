//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "CONVECTION3DPA_NOSHARED.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


CONVECTION3DPA_NOSHARED::CONVECTION3DPA_NOSHARED(const RunParams& params)
  : KernelBase(rajaperf::Apps_CONVECTION3DPA_NOSHARED, params)
{
  Index_type NE_default = 15625;
  setDefaultProblemSize(NE_default*conv_ns::D1D*conv_ns::D1D*conv_ns::D1D);
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

void CONVECTION3DPA_NOSHARED::setSize(Index_type target_size, Index_type target_reps)
{
  //Define problem size in terms of DOFS
  m_NE = std::max((target_size + (conv_ns::D1D*conv_ns::D1D*conv_ns::D1D)/2) / (conv_ns::D1D*conv_ns::D1D*conv_ns::D1D), Index_type(1));

  setActualProblemSize( m_NE*conv_ns::D1D*conv_ns::D1D*conv_ns::D1D );
  setRunReps( target_reps );

  setItsPerRep( m_NE*conv_ns::D1D*conv_ns::D1D*conv_ns::D1D );
  setKernelsPerRep(1);

  //
  // Every byte and FLOP count below is deliberately identical to
  // CONVECTION3DPA's, and the global workspace is excluded from all of them,
  // allocated bytes included. The two kernels run the same algorithm over the
  // same inputs and differ only in where the per-element scratch lives; the
  // workspace traffic is the overhead under study. Identical accounting means
  // every sizing flag (--size, --memory-allocated, --memory-touched,
  // --memory-moved) gives both kernels the same problem size, so any pair of
  // runs is directly comparable. The workspace's real footprint is
  // SCRATCH * m_NE * sizeof(Real_type) bytes on top of what is reported.
  //
  setBytesAllocatedPerRep( 3*sizeof(Real_type) * (conv_ns::Q1D*conv_ns::D1D) + // b, bt, g
                  conv_ns::VDIM*sizeof(Real_type) * (conv_ns::Q1D*conv_ns::Q1D*conv_ns::Q1D*m_NE) + // d
                           2*sizeof(Real_type) * (conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE) ); // x, y
  setBytesReadPerRep( 3*sizeof(Real_type) * conv_ns::Q1D*conv_ns::D1D + // b, bt, g
                      1*sizeof(Real_type) * conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE + // x
               conv_ns::VDIM*sizeof(Real_type) * conv_ns::Q1D*conv_ns::Q1D*conv_ns::Q1D*m_NE ); // d
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE ); // y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (
                         4 * conv_ns::D1D * conv_ns::Q1D * conv_ns::D1D * conv_ns::D1D + //2
                         6 * conv_ns::D1D * conv_ns::Q1D * conv_ns::Q1D * conv_ns::D1D + //3
                         6 * conv_ns::D1D * conv_ns::Q1D * conv_ns::Q1D * conv_ns::Q1D + //4
                         5 * conv_ns::Q1D * conv_ns::Q1D * conv_ns::Q1D +  // 5
                         2 * conv_ns::Q1D * conv_ns::D1D * conv_ns::Q1D * conv_ns::Q1D + // 6
                         2 * conv_ns::Q1D * conv_ns::D1D * conv_ns::Q1D * conv_ns::D1D + // 7
                         (1 + 2*conv_ns::Q1D) * conv_ns::D1D * conv_ns::D1D * conv_ns::D1D // 8
                         ));
}

CONVECTION3DPA_NOSHARED::~CONVECTION3DPA_NOSHARED()
{
}

void CONVECTION3DPA_NOSHARED::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B,  conv_ns::Q1D*conv_ns::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_Bt, conv_ns::Q1D*conv_ns::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_G, conv_ns::Q1D*conv_ns::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, conv_ns::Q1D*conv_ns::Q1D*conv_ns::Q1D*conv_ns::VDIM*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X, conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE, Real_type(0.0), vid);
  allocData(m_Workspace, conv_ns::SCRATCH*m_NE, vid);
}

void CONVECTION3DPA_NOSHARED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Y, conv_ns::D1D*conv_ns::D1D*conv_ns::D1D*m_NE, vid);
}

void CONVECTION3DPA_NOSHARED::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_G, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
  deallocData(m_Workspace, vid);
}

} // end namespace apps
} // end namespace rajaperf
