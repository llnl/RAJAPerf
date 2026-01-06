//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "LTIMES_NOVIEW.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


LTIMES_NOVIEW::LTIMES_NOVIEW(const RunParams& params)
  : KernelBase(rajaperf::Apps_LTIMES_NOVIEW, params)
{
  m_num_d = params.getLtimesNumD();
  m_num_g = params.getLtimesNumG();
  m_num_m = params.getLtimesNumM();
  Index_type num_z_default = std::max((Index_type{1000000} + (m_num_d * m_num_g)/2) / (m_num_d * m_num_g), Index_type(1));

  setDefaultProblemSize(m_num_d * m_num_g * num_z_default);
  setDefaultReps(50);

  m_num_z = std::max((getTargetProblemSize() + (m_num_d * m_num_g)/2) / (m_num_d * m_num_g), Index_type(1));

  m_philen = m_num_m * m_num_g * m_num_z;
  m_elllen = m_num_d * m_num_m;
  m_psilen = m_num_d * m_num_g * m_num_z;

  setActualProblemSize( m_psilen );

  setItsPerRep( m_philen );
  setKernelsPerRep(1);
  // using total data size instead of writes and reads
  setBytesReadPerRep( 1*sizeof(Real_type) * m_elllen + // ell
                      1*sizeof(Real_type) * m_psilen ); // psi
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * m_philen ); // phi
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(2 * m_num_z * m_num_g * m_num_m * m_num_d);

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning); // Change to Inconsistent if internal reductions use atomics
  setChecksumTolerance(ChecksumTolerance::normal);
  setChecksumScaleFactor(0.001 *
              ( static_cast<Checksum_type>(getDefaultProblemSize()) /
                                           getActualProblemSize() ));

  setComplexity(Complexity::N);

  setUsesFeature(Kernel);
  setUsesFeature(Launch);

  addVariantTunings();
}

LTIMES_NOVIEW::~LTIMES_NOVIEW()
{
}

void LTIMES_NOVIEW::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataConst(m_phidat, int(m_philen), Real_type(0.0), vid);
  allocAndInitData(m_elldat, int(m_elllen), vid);
  allocAndInitData(m_psidat, int(m_psilen), vid);
}

void LTIMES_NOVIEW::updateChecksum(VariantID vid, size_t tune_idx)
{
  addToChecksum(m_phidat, m_philen, vid);
}

void LTIMES_NOVIEW::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;

  deallocData(m_phidat, vid);
  deallocData(m_elldat, vid);
  deallocData(m_psidat, vid);
}

} // end namespace apps
} // end namespace rajaperf
