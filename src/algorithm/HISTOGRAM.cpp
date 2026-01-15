//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "HISTOGRAM.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>
#include <stdlib.h>

namespace rajaperf
{
namespace algorithm
{


HISTOGRAM::HISTOGRAM(const RunParams& params)
  : KernelBase(rajaperf::Algorithm_HISTOGRAM, params)
{
  setDefaultProblemSize(1000000);
  setDefaultReps(50);

  m_num_bins = params.getMultiReduceNumBins();
  m_bin_assignment_algorithm = params.getMultiReduceBinAssignmentAlgorithm();

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::Consistent); // integer arithmetic
  setChecksumTolerance(ChecksumTolerance::zero);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);
  setUsesFeature(Atomic);

  addVariantTunings();
}

void HISTOGRAM::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);
  setBytesReadPerRep( 1*sizeof(Index_type) * getActualProblemSize() ); // bins
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 1*sizeof(Data_type) * m_num_bins ); // counts
  setFLOPsPerRep( (std::is_floating_point_v<Data_type> ? 1 : 0) * getActualProblemSize() );
}

HISTOGRAM::~HISTOGRAM()
{
}

void HISTOGRAM::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
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
      allocData(DataSpace::Host, data, m_num_bins);
      for (Index_type b = 0; b < m_num_bins; ++b) {
        data[b] = static_cast<Real_type>(b+1) / m_num_bins;
      }
    } else if (init_random_sizes) {
      allocAndInitDataRandValue(DataSpace::Host, data, m_num_bins);
      std::sort(data, data+m_num_bins);
    } else if (init_all_one) {
      allocData(DataSpace::Host, data, m_num_bins);
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

    deallocData(DataSpace::Host, data);

  } else if (init_random_per_iterate) {
    Real_ptr data;
    allocAndInitDataRandValue(DataSpace::Host, data, getActualProblemSize());

    for (Index_type i = 0; i < getActualProblemSize(); ++i) {
      m_bins[i] = static_cast<Index_type>(data[i] * m_num_bins);
      if (m_bins[i] >= m_num_bins) {
        m_bins[i] = m_num_bins - 1;
      }
      if (m_bins[i] < 0) {
        m_bins[i] = 0;
      }
    }

    deallocData(DataSpace::Host, data);
  } else {
    throw 1;
  }

  allocAndInitDataConst(DataSpace::Host, m_counts_init, m_num_bins, static_cast<Data_type>(0));
  allocAndInitDataConst(DataSpace::Host, m_counts_final, m_num_bins, static_cast<Data_type>(0));
}

void HISTOGRAM::updateChecksum(VariantID RAJAPERF_UNUSED_ARG(vid), size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(DataSpace::Host, m_counts_final, m_num_bins);
}

void HISTOGRAM::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_bins, vid);
  deallocData(DataSpace::Host, m_counts_init);
  deallocData(DataSpace::Host, m_counts_final);
}

} // end namespace algorithm
} // end namespace rajaperf
