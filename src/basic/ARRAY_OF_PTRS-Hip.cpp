//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "ARRAY_OF_PTRS.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace basic
{


__constant__ Real_ptr x_array_const[ARRAY_OF_PTRS_MAX_ARRAY_SIZE];

#define ARRAY_OF_PTRS_DATA_SETUP_HIP_CONST \
  hipErrchk( hipMemcpyToSymbolAsync(HIP_SYMBOL(x_array_const), &x[0], \
                                    array_size * sizeof(Real_ptr), \
                                    0, hipMemcpyHostToDevice, res.get_stream()) );

#define ARRAY_OF_PTRS_DATA_TEARDOWN_HIP_CONST


#define ARRAY_OF_PTRS_DATA_SETUP_HIP_MEMORY(dataSpace) \
  Real_ptr* x_array_memory; \
  \
  allocData(dataSpace, x_array_memory, array_size); \
  copyData(dataSpace, x_array_memory, DataSpace::Host, &x[0], array_size);

#define ARRAY_OF_PTRS_DATA_TEARDOWN_HIP_MEMORY(dataSpace) \
  deallocData(dataSpace, x_array_memory);


template < size_t block_size >
__launch_bounds__(block_size)
__global__ void array_of_ptrs_param(Real_ptr y, ARRAY_OF_PTRS_Array x_array,
                                    Index_type array_size,
                                    Index_type iend)
{
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    ARRAY_OF_PTRS_BODY(x_array.array);
  }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void array_of_ptrs_const(Real_ptr y,
                                    Index_type array_size,
                                    Index_type iend)
{
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    ARRAY_OF_PTRS_BODY(x_array_const);
  }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void array_of_ptrs_shared(Real_ptr y, Real_ptr* x_array_memory,
                                     Index_type array_size,
                                     Index_type iend)
{
  __shared__ Real_ptr x_array_shared[ARRAY_OF_PTRS_MAX_ARRAY_SIZE];
  for (Index_type l = threadIdx.x; l < array_size; l += block_size) {
    x_array_shared[l] = x_array_memory[l];
  }
  __syncthreads();
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    ARRAY_OF_PTRS_BODY(x_array_shared);
  }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void array_of_ptrs_memory(Real_ptr y, Real_ptr* x_array_memory,
                                     Index_type array_size,
                                     Index_type iend)
{
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    ARRAY_OF_PTRS_BODY(x_array_memory);
  }
}


template < size_t block_size >
void ARRAY_OF_PTRS::runHipVariantParam(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  ARRAY_OF_PTRS_DATA_SETUP;

  if ( vid == Base_HIP ) {

    ARRAY_OF_PTRS_Array x_array = x;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;
  
      RPlaunchHipKernel( (array_of_ptrs_param<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         y, x_array, array_size, iend );

    }
    stopTimer();

  } else if ( vid == Lambda_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      auto array_of_ptrs_lambda = [=] __device__ (Index_type i) {
        ARRAY_OF_PTRS_BODY(x);
      };

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (lambda_hip_forall<block_size,
                                            decltype(array_of_ptrs_lambda)>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         ibegin, iend, array_of_ptrs_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i) {
        ARRAY_OF_PTRS_BODY(x);
      });

    }
    stopTimer();

  } else {
     getCout() << "\n  ARRAY_OF_PTRS : Unknown Hip variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void ARRAY_OF_PTRS::runHipVariantConst(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  ARRAY_OF_PTRS_DATA_SETUP;
  ARRAY_OF_PTRS_DATA_SETUP_HIP_CONST;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (array_of_ptrs_const<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         y, array_size, iend );

    }
    stopTimer();

  } else {
     getCout() << "\n  ARRAY_OF_PTRS : Unknown Hip variant id = " << vid << std::endl;
  }

  ARRAY_OF_PTRS_DATA_TEARDOWN_HIP_CONST;

}

template < size_t block_size >
void ARRAY_OF_PTRS::runHipVariantShared(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  ARRAY_OF_PTRS_DATA_SETUP;
  ARRAY_OF_PTRS_DATA_SETUP_HIP_MEMORY(dataSpace);

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (array_of_ptrs_shared<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         y, x_array_memory, array_size, iend );

    }
    stopTimer();

  } else {
     getCout() << "\n  ARRAY_OF_PTRS : Unknown Hip variant id = " << vid << std::endl;
  }

  ARRAY_OF_PTRS_DATA_TEARDOWN_HIP_MEMORY(dataSpace);

}

template < size_t block_size >
void ARRAY_OF_PTRS::runHipVariantMemory(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  ARRAY_OF_PTRS_DATA_SETUP;
  ARRAY_OF_PTRS_DATA_SETUP_HIP_MEMORY(dataSpace);

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (array_of_ptrs_memory<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         y, x_array_memory, array_size, iend );

    }
    stopTimer();

  } else {
     getCout() << "\n  ARRAY_OF_PTRS : Unknown Hip variant id = " << vid << std::endl;
  }

  ARRAY_OF_PTRS_DATA_TEARDOWN_HIP_MEMORY(dataSpace);

}


void ARRAY_OF_PTRS::runHipVariant(VariantID vid, size_t tune_idx)
{
  size_t t = 0;

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (tune_idx == t) {
        setBlockSize(block_size);
        runHipVariantParam<block_size>(vid);

      }

      t += 1;

      if (vid == Base_HIP) {

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantConst<block_size>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantShared<block_size>(DataSpace::HipDevice, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantShared<block_size>(DataSpace::HipPinnedCoarse, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantMemory<block_size>(DataSpace::HipDevice, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runHipVariantMemory<block_size>(DataSpace::HipPinnedCoarse, vid);

        }

        t += 1;

      }

    }

  });

}

void ARRAY_OF_PTRS::setHipTuningDefinitions(VariantID vid)
{

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      addVariantTuningName(vid, "param_"+std::to_string(block_size));

      if (vid == Base_HIP) {
        addVariantTuningName(vid, "const_"+std::to_string(block_size));
        addVariantTuningName(vid, "shared_device_"+std::to_string(block_size));
        addVariantTuningName(vid, "shared_pinned_"+std::to_string(block_size));
        addVariantTuningName(vid, "device_"+std::to_string(block_size));
        addVariantTuningName(vid, "pinned_"+std::to_string(block_size));
      }

    }

  });

}

} // end namespace basic
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP
