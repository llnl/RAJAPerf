//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MULTI_REDUCE.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>
#include <stdlib.h>

namespace rajaperf
{
namespace basic
{


MULTI_REDUCE::MULTI_REDUCE(const RunParams& params)
  : KernelBase(rajaperf::Basic_MULTI_REDUCE, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  setActualProblemSize( getTargetProblemSize() );

  m_num_bins = params.getMultiReduceNumBins();
  m_bin_assignment_algorithm = params.getMultiReduceBinAssignmentAlgorithm();

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Data_type) * m_num_bins +
                      1*sizeof(Data_type) * getActualProblemSize() +
                      1*sizeof(Index_type) * getActualProblemSize() );
  setBytesWrittenPerRep( 1*sizeof(Data_type) * m_num_bins );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep(1 * getActualProblemSize());

  setChecksumConsistency(ChecksumConsistency::Inconsistent);

  setComplexity(Complexity::N);

  setUsesFeature(Forall);
  setUsesFeature(Atomic);

  addVariantTunings();
}

MULTI_REDUCE::~MULTI_REDUCE()
{
}

void MULTI_REDUCE::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  allocAndInitDataRandValue(m_data, getActualProblemSize(), vid);
  auto reset_bins = allocDataForInit(m_bins, getActualProblemSize(), vid);

  const bool init_random_per_iterate =
      (m_bin_assignment_algorithm == RunParams::BinAssignmentAlgorithm::Random);
  const bool init_random_sizes =
      (m_bin_assignment_algorithm == RunParams::BinAssignmentAlgorithm::RunsRandomSizes);
  const bool init_even_sizes =
      (m_bin_assignment_algorithm == RunParams::BinAssignmentAlgorithm::RunsEvenSizes);
  const bool init_all_one =
      (m_bin_assignment_algorithm == RunParams::BinAssignmentAlgorithm::Single);

  if (init_even_sizes || init_random_sizes || init_all_one) {
    Real_ptr data = nullptr;
    if (init_even_sizes) {
      allocData(data, m_num_bins, Base_Seq);
      for (Index_type b = 0; b < m_num_bins; ++b) {
        data[b] = static_cast<Real_type>(b+1) / m_num_bins;
      }
    } else if (init_random_sizes) {
      allocAndInitDataRandValue(data, m_num_bins, Base_Seq);
      std::sort(data, data+m_num_bins);
    } else if (init_all_one) {
      allocData(data, m_num_bins, Base_Seq);
      for (Index_type b = 0; b < m_num_bins; ++b) {
        data[b] = static_cast<Real_type>(0);
      }
    }

    Index_type actual_prob_size = getActualProblemSize();
    Index_type bin = 0;
    for (Index_type i = 0; i < actual_prob_size; ++i) {
      Real_type pos = static_cast<Real_type>(i) / actual_prob_size;
      while (bin+1 < m_num_bins && pos >= data[bin]) {
        bin += 1;
      }
      m_bins[i] = bin;
    }

    deallocData(data, Base_Seq);

  } else if (init_random_per_iterate) {
    Real_ptr data;
    allocAndInitDataRandValue(data, getActualProblemSize(), Base_Seq);

    for (Index_type i = 0; i < getActualProblemSize(); ++i) {
      m_bins[i] = static_cast<Index_type>(data[i] * m_num_bins);
      if (m_bins[i] >= m_num_bins) {
        m_bins[i] = m_num_bins - 1;
      }
      if (m_bins[i] < 0) {
        m_bins[i] = 0;
      }
    }

    deallocData(data, Base_Seq);
  } else {
    throw 1;
  }

  allocAndInitDataConst(DataSpace::Host, m_values_init, m_num_bins, 0.0);
  allocAndInitDataConst(DataSpace::Host, m_values_final, m_num_bins, 0.0);
}

void MULTI_REDUCE::updateChecksum(VariantID vid, size_t tune_idx)
{
  checksum[vid][tune_idx] += calcChecksum(DataSpace::Host, m_values_final, m_num_bins);
}

void MULTI_REDUCE::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  (void) vid;
  deallocData(m_bins, vid);
  deallocData(m_data, vid);
  deallocData(DataSpace::Host, m_values_init);
  deallocData(DataSpace::Host, m_values_final);
}

} // end namespace basic
} // end namespace rajaperf
