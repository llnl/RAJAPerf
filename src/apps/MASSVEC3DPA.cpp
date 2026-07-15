//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASSVEC3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>
#include <string>

namespace rajaperf {
namespace apps {

MASSVEC3DPA::Geometry
MASSVEC3DPA::getGeometryForTuning(VariantID vid, size_t tune_idx) const
{
  if (hasVariantTuningDefined(vid, tune_idx)) {
    const std::string& tuning_name = getVariantTuningName(vid, tune_idx);

#define MASSVEC3DPA_GEOMETRY_LOOKUP(name, geometry_tuning_name, d1d, q1d)     \
    if (tuning_name.find(geometry_tuning_name) != std::string::npos) {         \
      return {d1d, q1d};                                                       \
    }
    MASSVEC3DPA_GEOMETRIES(MASSVEC3DPA_GEOMETRY_LOOKUP)
#undef MASSVEC3DPA_GEOMETRY_LOOKUP
  }

  return {mvpa::D1D, mvpa::Q1D};
}

void MASSVEC3DPA::configureGeometryForTuning(VariantID vid, size_t tune_idx)
{
  Geometry geometry = getGeometryForTuning(vid, tune_idx);
  m_D1D = geometry.d1d;
  m_Q1D = geometry.q1d;
  setSize(m_target_size, m_target_reps);
}

MASSVEC3DPA::MASSVEC3DPA(const RunParams &params)
    : KernelBase(rajaperf::Apps_MASSVEC3DPA, params)
{
  m_D1D = mvpa::D1D;
  m_Q1D = mvpa::Q1D;

  const Index_type NE_initial = 5208;
  setDefaultProblemSize(NE_initial * mvpa::DIM * m_D1D * m_D1D * m_D1D);
  setDefaultReps(50);

  m_target_size = params.getTargetSize(getDefaultProblemSize());
  m_target_reps = params.getReps(getDefaultReps());

  setSize(m_target_size, m_target_reps);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(3);

  setUsesFeature(Launch);

  addVariantTunings();
}

void MASSVEC3DPA::setSize(Index_type target_size, Index_type target_reps)
{
  m_NE =
      std::max((target_size + (mvpa::DIM * m_Q1D * m_Q1D * m_Q1D) / 2) /
                   (mvpa::DIM * m_Q1D * m_Q1D * m_Q1D),
               Index_type(1));

  setActualProblemSize(m_NE * mvpa::DIM * m_Q1D * m_Q1D * m_Q1D);
  setRunReps( target_reps );

  setItsPerRep(getActualProblemSize());
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 1*sizeof(Real_type) * m_Q1D*m_D1D + // B
                           1*sizeof(Real_type) * m_Q1D*m_Q1D*m_Q1D*m_NE + // D
                           2*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*mvpa::DIM*m_NE ); // X, Y
  setBytesReadPerRep( 1*sizeof(Real_type) * m_Q1D*m_D1D + // B
                      1*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*mvpa::DIM*m_NE + // X
                      1*sizeof(Real_type) * m_Q1D*m_Q1D*m_Q1D*m_NE ); // D
  setBytesWrittenPerRep( 1*sizeof(Real_type) * m_D1D*m_D1D*m_D1D*mvpa::DIM*m_NE ); // Y
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep(0);

  //3 for the dimension loop
  setFLOPsPerRep( m_NE*mvpa::DIM *
                  ( 2 * m_D1D*m_Q1D*m_D1D*m_D1D +
                    2 * m_D1D*m_Q1D*m_Q1D*m_D1D +
                    2 * m_D1D*m_Q1D*m_Q1D*m_Q1D +
                    m_Q1D*m_Q1D*m_Q1D +
                    2 * m_Q1D*m_D1D*m_Q1D*m_Q1D +
                    2 * m_Q1D*m_D1D*m_D1D*m_Q1D +
                    2 * m_Q1D*m_D1D*m_D1D*m_D1D ) );
}

MASSVEC3DPA::~MASSVEC3DPA() {}

void MASSVEC3DPA::setUp(VariantID vid, size_t tune_idx)
{
  configureGeometryForTuning(vid, tune_idx);

  allocAndInitDataConst(m_B, m_Q1D*m_D1D, 1.0, vid);
  allocAndInitDataConst(m_D, m_Q1D*m_Q1D*m_Q1D*m_NE, 1.0, vid);
  allocAndInitDataConst(m_X, m_D1D*m_D1D*m_D1D*mvpa::DIM*m_NE, 1.0, vid);
  allocAndInitDataConst(m_Y, m_D1D*m_D1D*m_D1D*mvpa::DIM*m_NE, 0.0, vid);

}

void MASSVEC3DPA::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  const Size_type y_len = mvpa::DIM * m_D1D * m_D1D * m_D1D * m_NE;
  const Checksum_type y_checksum =
      calcChecksum(getDataSpace(vid), m_Y, y_len, getDataAlignment());
  const Checksum_type x_checksum =
      calcChecksum(getDataSpace(vid), m_X, y_len, getDataAlignment());
  const Checksum_type expected_value =
      Checksum_type(m_D1D) * Checksum_type(m_D1D) * Checksum_type(m_D1D) *
      Checksum_type(m_Q1D) * Checksum_type(m_Q1D) * Checksum_type(m_Q1D);

  addToChecksum(y_checksum / (x_checksum * expected_value));
}

void MASSVEC3DPA::tearDown(VariantID vid,
                           size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}

} // end namespace apps
} // end namespace rajaperf
