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
  m_ni = params.getGrid3DMeshX() + 2;
  m_nj = params.getGrid3DMeshY() + 2;
  m_nk = params.getGrid3DMeshZ() + 2;
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
  if (!run_params.useGrid3DMeshDims())
  {
    m_ni = m_nj = m_nk = std::cbrt( target_size ) + 2 + std::cbrt(3)-1;
  }

  setActualProblemSize( (m_ni-2)*(m_nj-2)*(m_nk-2) );
  setRunReps( target_reps );

  setItsPerRep( 2 * getActualProblemSize() );
  setKernelsPerRep( 2 );

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * m_ni*m_nj*m_nk ); // A, B
  setBytesReadPerRep( 1*sizeof(Real_type) *
                      (m_ni*m_nj*m_nk - 4*(m_ni-2) - 4*(m_nj-2) - 4*(m_nk-2) - 8) + // A (7 point stencil)

                      1*sizeof(Real_type) *
                      (m_ni*m_nj*m_nk - 4*(m_ni-2) - 4*(m_nj-2) - 4*(m_nk-2) - 8) ); // B (7 point stencil)
  setBytesWrittenPerRep( 1*sizeof(Real_type) * (m_ni-2)*(m_nj-2)*(m_nk-2) +  // B

                         1*sizeof(Real_type) * (m_ni-2)*(m_nj-2)*(m_nk-2) ); // A
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(15 * (m_ni-2)*(m_nj-2)*(m_nk-2) +
                  15 * (m_ni-2)*(m_nj-2)*(m_nk-2) );
}

POLYBENCH_HEAT_3D::~POLYBENCH_HEAT_3D()
{
}

void POLYBENCH_HEAT_3D::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitData(m_A, m_ni*m_nj*m_nk, vid);
  allocAndInitData(m_B, m_ni*m_nj*m_nk, vid);
}

void POLYBENCH_HEAT_3D::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_A, m_ni*m_nj*m_nk, vid);
  addToChecksum(m_B, m_ni*m_nj*m_nk, vid);
}

void POLYBENCH_HEAT_3D::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_A, vid);
  deallocData(m_B, vid);
}

} // end namespace polybench
} // end namespace rajaperf
