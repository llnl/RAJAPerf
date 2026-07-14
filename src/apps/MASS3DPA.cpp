//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"
#include "common/RunParams.hpp"

#include <algorithm>
#include <string>

namespace rajaperf
{
namespace apps
{

MASS3DPA::Geometry
MASS3DPA::getGeometryForTuning(VariantID vid, size_t tune_idx) const
{
  if (hasVariantTuningDefined(vid, tune_idx)) {
    const std::string& tuning_name = getVariantTuningName(vid, tune_idx);

#define MASS3DPA_GEOMETRY_LOOKUP(name, geometry_tuning_name, d1d, q1d)         \
    if (tuning_name == geometry_tuning_name) {                                 \
      return {d1d, q1d};                                                       \
    }
    MASS3DPA_GEOMETRIES(MASS3DPA_GEOMETRY_LOOKUP)
#undef MASS3DPA_GEOMETRY_LOOKUP
  }

  return {mpa::D1D, mpa::Q1D};
}

void MASS3DPA::configureGeometryForTuning(VariantID vid, size_t tune_idx)
{
  Geometry geometry = getGeometryForTuning(vid, tune_idx);
  m_D1D = geometry.d1d;
  m_Q1D = geometry.q1d;
  setSize(m_target_size, m_target_reps);
}


MASS3DPA::MASS3DPA(const RunParams& params)
  : KernelBase(rajaperf::Apps_MASS3DPA, params)
{
  m_D1D = mpa::D1D;
  m_Q1D = mpa::Q1D;

  Index_type NE_default = 8000;
  setDefaultProblemSize(NE_default*m_D1D*m_D1D*m_D1D);
  setDefaultReps(50);

  m_target_size = params.getTargetSize(getDefaultProblemSize());
  m_target_reps = params.getReps(getDefaultReps());

  setSize(m_target_size, m_target_reps);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(2);
  setProblemDimensionality(3);

  setUsesFeature(Launch);

  addVariantTunings();
}

void MASS3DPA::setSize(Index_type target_size, Index_type target_reps)
{
  m_NE = std::max((target_size + (m_D1D*m_D1D*m_D1D)/2) /
                  (m_D1D*m_D1D*m_D1D), Index_type(1));

  setActualProblemSize( m_NE*m_D1D*m_D1D*m_D1D );
  setRunReps( target_reps );

  setItsPerRep( m_NE*m_D1D*m_D1D );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * m_Q1D*m_D1D + // B, Bt
                           1*sizeof(Real_type) * m_Q1D*m_Q1D*m_Q1D*m_NE + // D
                           2*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*m_NE ); // X, Y
  setBytesReadPerRep( 2*sizeof(Real_type) * m_Q1D*m_D1D + // B, Bt
                      1*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*m_NE + // X
                      1*sizeof(Real_type) * m_Q1D*m_Q1D*m_Q1D*m_NE ); // D
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*m_NE ); // Y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (2 * m_D1D * m_D1D * m_D1D * m_Q1D +
                         2 * m_D1D * m_D1D * m_Q1D * m_Q1D +
                         2 * m_D1D * m_Q1D * m_Q1D * m_Q1D + m_Q1D * m_Q1D * m_Q1D +
                         2 * m_Q1D * m_Q1D * m_Q1D * m_D1D +
                         2 * m_Q1D * m_Q1D * m_D1D * m_D1D +
                         2 * m_Q1D * m_D1D * m_D1D * m_D1D + m_D1D * m_D1D * m_D1D));
}

MASS3DPA::~MASS3DPA()
{
}

void MASS3DPA::setUp(VariantID vid, size_t tune_idx)
{
  configureGeometryForTuning(vid, tune_idx);

  allocAndInitDataConst(m_B,  m_Q1D*m_D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_Bt, m_Q1D*m_D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D,  m_Q1D*m_Q1D*m_Q1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X,  m_D1D*m_D1D*m_D1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y,  m_D1D*m_D1D*m_D1D*m_NE, Real_type(0.0), vid);
}

void MASS3DPA::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Y, m_D1D*m_D1D*m_D1D*m_NE, vid);
}

void MASS3DPA::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}

} // end namespace apps
} // end namespace rajaperf
