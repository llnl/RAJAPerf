//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DEA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


MASS3DEA::MASS3DEA(const RunParams& params)
  : KernelBase(rajaperf::Apps_MASS3DEA, params)
{
  m_NE_default = 8000;

  setDefaultProblemSize(m_NE_default*mea::D1D*mea::D1D*mea::D1D);
  setDefaultReps(1);

  const Index_type ea_mat_entries = mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D;

  m_NE = std::max((getTargetProblemSize() + (ea_mat_entries)/2) / (ea_mat_entries), Index_type(1));

  setActualProblemSize( m_NE*ea_mat_entries );

  setItsPerRep( m_NE*mea::D1D*mea::D1D*mea::D1D );
  setKernelsPerRep(1);

  setBytesReadPerRep( 1*sizeof(Real_type) * mea::Q1D*mea::D1D + // B
                      1*sizeof(Real_type) * mea::Q1D*mea::Q1D*mea::Q1D*m_NE ); // D
  setBytesWrittenPerRep( 1*sizeof(Real_type) * ea_mat_entries*m_NE ); // M_e
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * 7 * ea_mat_entries);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setUsesFeature(Launch);

  addVariantTunings();
}

MASS3DEA::~MASS3DEA()
{
}

void MASS3DEA::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B, Index_type(mea::Q1D*mea::D1D), Real_type(1.0), vid);
  allocAndInitDataConst(m_D, Index_type(mea::Q1D*mea::Q1D*mea::Q1D*m_NE), Real_type(1.0), vid);
  allocAndInitDataConst(m_M, Index_type(mea::D1D*mea::D1D*mea::D1D*
                                 mea::D1D*mea::D1D*mea::D1D*m_NE), Real_type(0.0), vid);
}

void MASS3DEA::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_M, mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D*mea::D1D*m_NE, vid);
}

void MASS3DEA::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_D, vid);
  deallocData(m_M, vid);
}

} // end namespace apps
} // end namespace rajaperf
