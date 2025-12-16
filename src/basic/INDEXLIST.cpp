//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-22, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INDEXLIST.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


INDEXLIST::INDEXLIST(const RunParams& params)
  : KernelBase(rajaperf::Basic_INDEXLIST, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(100);

  setActualProblemSize( getTargetProblemSize() );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Real_type) * getActualProblemSize() ); // x
  setBytesWrittenPerRep( 1*sizeof(Int_type) * getActualProblemSize() / 2 ); // list (about 50% output)
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(0);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);
  setUsesFeature(Scan);

  if (run_params.getEnableCustomScan()) {

    addVariantTunings();

  }
}

INDEXLIST::~INDEXLIST()
{
}

void INDEXLIST::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandSign(m_x, getActualProblemSize(), vid);
  allocAndInitData(m_list, getActualProblemSize(), vid);
  m_len = -1;
}

void INDEXLIST::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(m_list, getActualProblemSize(), vid);
  checksum[vid][tune_idx] += Checksum_type(m_len);
}

void INDEXLIST::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_x, vid);
  deallocData(m_list, vid);
}

} // end namespace basic
} // end namespace rajaperf
