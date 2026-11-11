//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DEA_NOSHARED.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


MASS3DEA_NOSHARED::MASS3DEA_NOSHARED(const RunParams& params)
  : KernelBase(rajaperf::Apps_MASS3DEA_NOSHARED, params)
{
  Index_type NE_default = 8000;
  setDefaultProblemSize(NE_default*mea::D1D*mea::D1D*mea::D1D);
  setDefaultReps(1);

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

void MASS3DEA_NOSHARED::setSize(Index_type target_size, Index_type target_reps)
{
  const Index_type ea_mat_entries = mea::D1D * mea::D1D * mea::D1D * mea::D1D * mea::D1D * mea::D1D;
  const Index_type qpt_entries = mea::Q1D * mea::Q1D * mea::Q1D;
  const Index_type flops_per_qpt = 7;

  m_NE = std::max((target_size + (ea_mat_entries)/2) / (ea_mat_entries), Index_type(1));

  setActualProblemSize( m_NE*ea_mat_entries );
  setRunReps( target_reps );

  setItsPerRep( m_NE*mea::D1D*mea::D1D*mea::D1D );
  setKernelsPerRep(1);

  //
  // Every byte and FLOP count below is deliberately identical to MASS3DEA's,
  // and the global workspace is excluded from all of them, allocated bytes
  // included. The two kernels run the same algorithm over the same inputs and
  // differ only in where the scratch lives; the workspace traffic is the
  // overhead under study. Identical accounting means every sizing flag
  // (--size, --memory-allocated, --memory-touched, --memory-moved) gives both
  // kernels the same problem size, so any pair of runs is directly comparable.
  // The workspace's real footprint is mea_ns::SCRATCH * m_NE *
  // sizeof(Real_type) bytes on top of what is reported.
  //
  setBytesAllocatedPerRep( 1*sizeof(Real_type) * mea::Q1D*mea::D1D + // B
                           1*sizeof(Real_type) * mea::Q1D*mea::Q1D*mea::Q1D*m_NE + // D
                           1*sizeof(Real_type) * ea_mat_entries*m_NE ); // M_e
  setBytesReadPerRep( 1*sizeof(Real_type) * mea::Q1D*mea::D1D + // B
                      1*sizeof(Real_type) * mea::Q1D*mea::Q1D*mea::Q1D*m_NE ); // D
  setBytesWrittenPerRep( 1*sizeof(Real_type) * ea_mat_entries*m_NE ); // M_e
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * flops_per_qpt * qpt_entries * ea_mat_entries);
}

MASS3DEA_NOSHARED::~MASS3DEA_NOSHARED()
{
}

void MASS3DEA_NOSHARED::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type ea_mat_entries = mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D;

  allocAndInitDataConst(m_B, mea::Q1D*mea::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, mea::Q1D*mea::Q1D*mea::Q1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_M, ea_mat_entries*m_NE, Real_type(0.0), vid);
  allocData(m_Workspace, mea_ns::SCRATCH * m_NE, vid);
}

void MASS3DEA_NOSHARED::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Index_type ea_mat_entries = mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D;

  addToChecksum(m_M, ea_mat_entries*m_NE, vid);
}

void MASS3DEA_NOSHARED::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_D, vid);
  deallocData(m_M, vid);
  deallocData(m_Workspace, vid);
}

} // end namespace apps
} // end namespace rajaperf
