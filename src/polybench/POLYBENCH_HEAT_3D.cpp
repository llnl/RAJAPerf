//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "POLYBENCH_HEAT_3D.hpp"

#include "RAJA/RAJA.hpp"
#include "common/DataUtils.hpp"

#include <cmath>

namespace rajaperf
{
namespace polybench
{


POLYBENCH_HEAT_3D::POLYBENCH_HEAT_3D(const RunParams& params)
  : KernelBase(rajaperf::Polybench_HEAT_3D, params)
{
  Index_type N_default = 102;
  setDefaultProblemSize( (N_default-2)*(N_default-2)*(N_default-2) );
  setDefaultReps(400);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(3);

  setUsesFeature(Kernel);

  addVariantTunings();
}

void POLYBENCH_HEAT_3D::setSize(Index_type target_size, Index_type target_reps)
{
  m_N = std::cbrt( target_size ) + 2 + std::cbrt(3)-1;

  setActualProblemSize( (m_N-2) * (m_N-2) * (m_N-2) );
  setRunReps( target_reps );

  setItsPerRep( 2 * getActualProblemSize() );
  setKernelsPerRep( 2 );
  setBytesReadPerRep( 1*sizeof(Real_type ) * (m_N * m_N * m_N - 12*(m_N-2) - 8) + // A (7 point stencil)

                      1*sizeof(Real_type ) * (m_N * m_N * m_N - 12*(m_N-2) - 8)); // B (7 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type ) * (m_N-2) * (m_N-2) * (m_N-2) + // B

                         1*sizeof(Real_type ) * (m_N-2) * (m_N-2) * (m_N-2) ); // A
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 15 * (m_N-2) * (m_N-2) * (m_N-2) +
                  15 * (m_N-2) * (m_N-2) * (m_N-2) );
}

POLYBENCH_HEAT_3D::~POLYBENCH_HEAT_3D()
{
}

void POLYBENCH_HEAT_3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_Ainit, m_N*m_N*m_N, vid);
  allocAndInitData(m_Binit, m_N*m_N*m_N, vid);
  allocData(m_A, m_N*m_N*m_N, vid);
  allocData(m_B, m_N*m_N*m_N, vid);
}

void POLYBENCH_HEAT_3D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_A, m_N*m_N*m_N, vid);
  addToChecksum(m_B, m_N*m_N*m_N, vid);
}

void POLYBENCH_HEAT_3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
  deallocData(m_Ainit, vid);
  deallocData(m_Binit, vid);
}

} // end namespace polybench
} // end namespace rajaperf
