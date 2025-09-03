//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FIR.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_CUDA)

#include "common/CudaDataUtils.hpp"

#include <algorithm>
#include <iostream>

namespace rajaperf
{
namespace apps
{

__constant__ Real_type coeff_const[FIR_COEFFLEN];

#define FIR_DATA_SETUP_CUDA_CONST \
  Real_type *dcoeff_const_addr; \
  cudaErrchk( cudaGetSymbolAddress((void**)&dcoeff_const_addr, coeff_const) ); \
  cudaErrchk( cudaMemcpyAsync(dcoeff_const_addr, &coeff_array[0], \
                              coefflen * sizeof(Real_type), \
                              cudaMemcpyHostToDevice, res.get_stream()) );


#define FIR_DATA_TEARDOWN_CUDA_CONST


#define FIR_DATA_SETUP_CUDA_MEMORY(dataSpace) \
  Real_ptr coeff_memory; \
  \
  allocData(dataSpace, coeff_memory, coefflen); \
  copyData(dataSpace, coeff_memory, DataSpace::Host, &coeff_array[0], coefflen);


#define FIR_DATA_TEARDOWN_CUDA_MEMORY(dataSpace) \
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
__global__ void fir_param_data_shared(Real_ptr out, Real_ptr in,
                          FIR_Array coeff_param,
                          const Index_type coefflen,
                          Index_type iend)
{
  __shared__ Real_type shared_in[block_size+FIR_COEFFLEN-1];
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  for (Index_type l = threadIdx.x, li = i;
       l < (block_size+coefflen-1) && li < (iend+coefflen-1);
       l += block_size, li += block_size) {
    shared_in[l] = in[li];
  }
  __syncthreads();
  if (i < iend) {
   FIR_BODY_IN(coeff_param.array, shared_in+threadIdx.x);
  }
}

template < size_t block_size, size_t unroll_num >
__launch_bounds__(block_size)
__global__ void fir_param_unroll(Real_ptr out, Real_ptr in,
                          FIR_Array coeff_param,
                          const Index_type coefflen,
                          Index_type iend)
{
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    Real_type sum = 0.0;
    #pragma unroll unroll_num
    for (Index_type j = 0; j < coefflen; ++j ) {
      sum += (coeff_param.array)[j]*in[i+j];
    }
    out[i] = sum;
  }
}

#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void fir_param_grid_const(Real_ptr out, Real_ptr in,
                          const __grid_constant__ FIR_Array coeff_param,
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
__global__ void fir_param_grid_const_data_shared(Real_ptr out, Real_ptr in,
                          const __grid_constant__ FIR_Array coeff_param,
                          const Index_type coefflen,
                          Index_type iend)
{
  __shared__ Real_type shared_in[block_size+FIR_COEFFLEN-1];
  Index_type i = blockIdx.x * block_size + threadIdx.x;
  for (Index_type l = threadIdx.x, li = i;
       l < (block_size+coefflen-1) && li < (iend+coefflen-1);
       l += block_size, li += block_size) {
    shared_in[l] = in[li];
  }
  __syncthreads();
  if (i < iend) {
   FIR_BODY_IN(coeff_param.array, shared_in+threadIdx.x);
  }
}

template < size_t block_size, size_t unroll_num >
__launch_bounds__(block_size)
__global__ void fir_param_grid_const_unroll(Real_ptr out, Real_ptr in,
                          const __grid_constant__ FIR_Array coeff_param,
                          const Index_type coefflen,
                          Index_type iend)
{
   Index_type i = blockIdx.x * block_size + threadIdx.x;
   if (i < iend) {
    Real_type sum = 0.0;
    #pragma unroll unroll_num
    for (Index_type j = 0; j < coefflen; ++j ) {
      sum += (coeff_param.array)[j]*in[i+j];
    }
    out[i] = sum;
   }
}

#endif

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
void FIR::runCudaVariantParam(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin = 0;
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else if ( vid == RAJA_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

       RAJA::forall< RAJA::cuda_exec<block_size, true /*async*/> >( res,
         RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i) {
         FIR_BODY(coeff_array);
       });

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void FIR::runCudaVariantParamDataShared(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param_data_shared<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void FIR::runCudaVariantParamGridConstDataShared(VariantID vid)
{
#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param_grid_const_data_shared<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else
#endif
  {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size, size_t unroll_num >
void FIR::runCudaVariantParamUnroll(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param_unroll<block_size, unroll_num>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void FIR::runCudaVariantParamGridConst(VariantID vid)
{
#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param_grid_const<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else
#endif
  {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size, size_t unroll_num >
void FIR::runCudaVariantParamGridConstUnroll(VariantID vid)
{
#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;

  if ( vid == Base_CUDA ) {

    FIR_Array coeff_param = coeff_array;

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_param_grid_const_unroll<block_size, unroll_num>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_param,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else
#endif
  {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }
}

template < size_t block_size >
void FIR::runCudaVariantConst(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_CUDA_CONST;

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_const<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coefflen,
                         iend );

    }
    stopTimer();


  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_CUDA_CONST;
}

template < size_t block_size >
void FIR::runCudaVariantShared(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_CUDA_MEMORY(dataSpace);

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_shared<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_memory,
                         coefflen,
                         iend );

    }
    stopTimer();


  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_CUDA_MEMORY(dataSpace);
}

template < size_t block_size >
void FIR::runCudaVariantMemory(DataSpace dataSpace, VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type iend = getActualProblemSize();

  auto res{getCudaResource()};

  FIR_DATA_SETUP;
  FIR_COEFF;
  FIR_DATA_SETUP_CUDA_MEMORY(dataSpace);

  if ( vid == Base_CUDA ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchCudaKernel( (fir_memory<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         out, in,
                         coeff_memory,
                         coefflen,
                         iend );

    }
    stopTimer();

  } else {
    getCout() << "\n  FIR : Unknown Cuda variant id = " << vid << std::endl;
  }

  FIR_DATA_TEARDOWN_CUDA_MEMORY(dataSpace);
}


void FIR::runCudaVariant(VariantID vid, size_t tune_idx)
{
  size_t t = 0;

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      if (tune_idx == t) {
        setBlockSize(block_size);
        runCudaVariantParam<block_size>(vid);

      }

      t += 1;

      if (vid == Base_CUDA) {

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantConst<block_size>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantShared<block_size>(DataSpace::CudaDevice, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantShared<block_size>(DataSpace::CudaPinned, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantMemory<block_size>(DataSpace::CudaDevice, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantMemory<block_size>(DataSpace::CudaPinned, vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamDataShared<block_size>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamUnroll<block_size, 1>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamUnroll<block_size, 2>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamUnroll<block_size, 4>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamUnroll<block_size, 8>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamUnroll<block_size, 16>(vid);

        }

        t += 1;

#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT
        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConst<block_size>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstDataShared<block_size>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstUnroll<block_size, 1>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstUnroll<block_size, 2>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstUnroll<block_size, 4>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstUnroll<block_size, 8>(vid);

        }

        t += 1;

        if (tune_idx == t) {
          setBlockSize(block_size);
          runCudaVariantParamGridConstUnroll<block_size, 16>(vid);

        }

        t += 1;
#endif

      }

    }

  });

}

void FIR::setCudaTuningDefinitions(VariantID vid)
{

  seq_for(gpu_block_sizes_type{}, [&](auto block_size) {

    if (run_params.numValidGPUBlockSize() == 0u ||
        run_params.validGPUBlockSize(block_size)) {

      addVariantTuningName(vid, "param_"+std::to_string(block_size));

      if (vid == Base_CUDA) {
        addVariantTuningName(vid, "const_"+std::to_string(block_size));
        addVariantTuningName(vid, "sharedDevice_"+std::to_string(block_size));
        addVariantTuningName(vid, "sharedPinned_"+std::to_string(block_size));
        addVariantTuningName(vid, "device_"+std::to_string(block_size));
        addVariantTuningName(vid, "pinned_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramDataShared_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramUnroll1_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramUnroll2_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramUnroll4_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramUnroll8_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramUnroll16_"+std::to_string(block_size));
#ifdef RAJAPERF_CUDA_HAVE_GRID_CONSTANT
        addVariantTuningName(vid, "paramGridConst_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstDataShared_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstUnroll1_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstUnroll2_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstUnroll4_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstUnroll8_"+std::to_string(block_size));
        addVariantTuningName(vid, "paramGridConstUnroll16_"+std::to_string(block_size));
#endif
      }

    }

  });

}


} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_CUDA
