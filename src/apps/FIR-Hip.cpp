//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIR.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <algorithm>
#include <iostream>

namespace rajaperf
{
namespace apps
{

__constant__ Real_type coeff_const[FIR_COEFFLEN];

#define FIR_DATA_SETUP_HIP_CONST \
  hipErrchk( hipMemcpyToSymbolAsync(HIP_SYMBOL(coeff_const), &coeff_array[0], \
                                    coefflen * sizeof(Real_type), \
                                    0, hipMemcpyHostToDevice, res.get_stream()) );


#define FIR_DATA_TEARDOWN_HIP_CONST


#define FIR_DATA_SETUP_HIP_MEMORY(dataSpace) \
  Real_ptr coeff_memory; \
  \
  allocData(dataSpace, coeff_memory, coefflen); \
  copyData(dataSpace, coeff_memory, DataSpace::Host, &coeff_array[0], coefflen);


#define FIR_DATA_TEARDOWN_HIP_MEMORY(dataSpace) \
  deallocData(dataSpace, coeff_memory);


template < size_t block_size >
__launch_bounds__(block_size)
__global__ void fir_param(Real_ptr out, Real_ptr in,
                          FIR_Array coeff_param,
                          const Index_type coefflen,
                          Index_type iend)
{
   Index_type i = blockIdx.x * block_size + threadIdx.x;
   if (i < iend) {
     FIR_BODY(coeff_param.array);
   }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void fir_const(Real_ptr out, Real_ptr in,
                          const Index_type coefflen,
                          Index_type iend)
{
   Index_type i = blockIdx.x * block_size + threadIdx.x;
   if (i < iend) {
     FIR_BODY(coeff_const);
   }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void fir_shared(Real_ptr out, Real_ptr in,
                           Real_ptr coeff_memory,
                           const Index_type coefflen,
                           Index_type iend)
{
  __shared__ Real_type coeff_shared[FIR_COEFFLEN];
  for (Index_type l = threadIdx.x; l < coefflen; l += block_size) {
    coeff_shared[l] = coeff_memory[l];
  }
  __syncthreads();
   Index_type i = blockIdx.x * block_size + threadIdx.x;
   if (i < iend) {
     FIR_BODY(coeff_shared);
   }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void fir_memory(Real_ptr out, Real_ptr in,
                           Real_ptr coeff_memory,
                           const Index_type coefflen,
                           Index_type iend)
{
   Index_type i = blockIdx.x * block_size + threadIdx.x;
   if (i < iend) {
     FIR_BODY(coeff_memory);
   }
}


template < size_t block_size >
void FIR::runHipVariantParam(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_HIP ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (fir_param<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else if ( vid == RAJA_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

       RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
         RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i) {
         FIR_BODY(coeff_array);
       });

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Hip variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void FIR::runHipVariantConst(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_HIP_CONST;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (fir_const<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coefflen,
                         iend );

    }
    stopTimer();


  } else {
    getCout() << "\n  FIR : Unknown Hip variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_HIP_CONST;
}

template < size_t block_size >
void FIR::runHipVariantShared(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_HIP_MEMORY(dataSpace);

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (fir_shared<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_memory,
                         coefflen,
                         iend );

    }
    stopTimer();


  } else {
    getCout() << "\n  FIR : Unknown Hip variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_HIP_MEMORY(dataSpace);
}

template < size_t block_size >
void FIR::runHipVariantMemory(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getHipResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_HIP_MEMORY(dataSpace);

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (fir_memory<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_memory,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Hip variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_HIP_MEMORY(dataSpace);
}


void FIR::runHipVariant(VariantID vid, size_t tune_idx)
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

void FIR::setHipTuningDefinitions(VariantID vid)
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

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP
