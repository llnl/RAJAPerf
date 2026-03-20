//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Methods for HIP kernel data allocation, initialization, and deallocation.
///


#ifndef RAJAPerf_HipDataUtils_HPP
#define RAJAPerf_HipDataUtils_HPP

#include "RPTypes.hpp"
#include <stdexcept>

#if defined(RAJA_ENABLE_HIP)

#include "common/RAJAPerfSuite.hpp"
#include "common/GPUUtils.hpp"

#include "RAJA/policy/hip/policy.hpp"
#include "RAJA/policy/hip/raja_hiperrchk.hpp"


namespace rajaperf
{

/*!
 * \brief Method for launching a HIP kernel with given configuration.
 *
 *        Note: method checks whether number of args and their types in
 *              kernel signature matches args passed to this method.
 */
template <typename... Args, typename...KernArgs>
void RPlaunchHipKernel(void (*kernel)(KernArgs...),
                       const dim3& numBlocks, const dim3& dimBlocks,
                       std::uint32_t sharedMemBytes, hipStream_t stream,
                       Args const&... args)
{
  static_assert(sizeof...(KernArgs) == sizeof...(Args),
                "Number of kernel args doesn't match what's passed to method");

  static_assert(conjunction<std::is_same<std::decay_t<KernArgs>, std::decay_t<Args>>...>::value,
                "Kernel arg types don't match what's passed to method");

  constexpr size_t count = sizeof...(Args);
  void* arg_arr[count]{(void*)&args...};

  auto k = reinterpret_cast<const void*>(kernel);
  CAMP_HIP_API_INVOKE_AND_CHECK( hipLaunchKernel,
      k, numBlocks, dimBlocks, arg_arr, sharedMemBytes, stream );
}

/*!
 * \brief Simple forall hip kernel that runs a lambda.
 */
template < typename Lambda >
__global__ void lambda_hip_forall(Index_type ibegin, Index_type iend, Lambda body)
{
  Index_type i = ibegin + blockIdx.x * blockDim.x + threadIdx.x;
  if (i < iend) {
    body(i);
  }
}
///
template < size_t block_size, typename Lambda >
__launch_bounds__(block_size)
__global__ void lambda_hip_forall(Index_type ibegin, Index_type iend, Lambda body)
{
  Index_type i = ibegin + blockIdx.x * block_size + threadIdx.x;
  if (i < iend) {
    body(i);
  }
}

/*!
 * \brief Grid stride forall hip kernel that runs a lambda.
 */
template < typename Lambda >
__global__ void lambda_hip_forall_grid_stride(Index_type ibegin, Index_type iend, Lambda body)
{
  Index_type i = ibegin + blockIdx.x * blockDim.x + threadIdx.x;
  Index_type grid_stride = gridDim.x * blockDim.x;
  for ( ; i < iend; i += grid_stride) {
    body(i);
  }
}
///
template < size_t block_size, typename Lambda >
__launch_bounds__(block_size)
__global__ void lambda_hip_forall_grid_stride(Index_type ibegin, Index_type iend, Lambda body)
{
  Index_type i = ibegin + blockIdx.x * block_size + threadIdx.x;
  Index_type grid_stride = gridDim.x * block_size;
  for ( ; i < iend; i += grid_stride) {
    body(i);
  }
}

/*!
 * \brief Simple hip kernel that runs a lambda.
 */
template < typename Lambda >
__global__ void lambda_hip(Lambda body)
{
  body();
}
///
template < size_t block_size, typename Lambda >
__launch_bounds__(block_size)
__global__ void lambda_hip(Lambda body)
{
  body();
}


namespace detail
{

/*!
 * \brief Get current hip device.
 */
inline int getHipDevice()
{
  int device = hipInvalidDeviceId;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipGetDevice, &device );
  return device;
}

/*!
 * \brief Get properties of the current hip device.
 */
inline hipDeviceProp_t getHipDeviceProp()
{
  hipDeviceProp_t prop;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipGetDeviceProperties, &prop, getHipDevice() );
  return prop;
}

/*!
 * \brief Get max occupancy in blocks for the given kernel for the current
 *        hip device.
 */
template < typename Func >
RAJA_INLINE
int getHipOccupancyMaxBlocks(Func&& func, int num_threads, size_t shmem_size)
{
  int max_blocks = -1;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipOccupancyMaxActiveBlocksPerMultiprocessor,
      &max_blocks, func, num_threads, shmem_size );

  size_t multiProcessorCount = getHipDeviceProp().multiProcessorCount;

  return max_blocks * multiProcessorCount;
}

/*
 * Copy memory len bytes from src to dst.
 */
inline void copyHipData(void* dst_ptr, const void* src_ptr, Size_type len)
{
  CAMP_HIP_API_INVOKE_AND_CHECK( hipMemcpy,
      dst_ptr, src_ptr, len, hipMemcpyDefault );
  CAMP_HIP_API_INVOKE_AND_CHECK( hipDeviceSynchronize );
}

/*!
 * \brief Allocate HIP device data array (dptr).
 */
inline void* allocHipDeviceData(Size_type len)
{
  void* dptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipMalloc, &dptr, len );
  return dptr;
}

/*!
 * \brief Allocate HIP fine-grained device data array (dfptr).
 */
inline void* allocHipDeviceFineData(Size_type len)
{
  void* dfptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipExtMallocWithFlags,
      &dfptr, len, hipDeviceMallocFinegrained );
  return dfptr;
}

/*!
 * \brief Allocate HIP managed data array (mptr).
 */
inline void* allocHipManagedData(Size_type len)
{
  void* mptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipMallocManaged,
      &mptr, len, hipMemAttachGlobal );
  return mptr;
}

/*!
 * \brief Allocate HIP pinned data array (pptr).
 */
inline void* allocHipPinnedData(Size_type len)
{
  void* pptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipHostMalloc,
      &pptr, len, hipHostMallocMapped );
  return pptr;
}

/*!
 * \brief Allocate HIP fine-grained pinned data array (pfptr).
 */
inline void* allocHipPinnedFineData(Size_type len)
{
  void* pfptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipHostMalloc,
      &pfptr, len, hipHostMallocMapped | hipHostMallocCoherent );
  return pfptr;
}

/*!
 * \brief Allocate HIP coarse-grained pinned data array (pcptr).
 */
inline void* allocHipPinnedCoarseData(Size_type len)
{
  void* pcptr = nullptr;
  CAMP_HIP_API_INVOKE_AND_CHECK( hipHostMalloc,
      &pcptr, len, hipHostMallocMapped | hipHostMallocNonCoherent );
  return pcptr;
}

/*!
 * \brief Apply mem advice to HIP data array (ptr).
 */
inline void adviseHipData(void* ptr, size_t len, hipMemoryAdvise advice, int device)
{
  CAMP_HIP_API_INVOKE_AND_CHECK( hipMemAdvise,
      ptr, len, advice, device );
}

#if defined(RAJAPERF_USE_MEMADVISE_COARSE)
inline void adviseHipCoarseData(void* ptr, size_t len)
{
  adviseHipData(ptr, len, hipMemAdviseSetCoarseGrain, getHipDevice());
}
#endif

inline void adviseHipFineData(void* ptr, size_t len)
{
  adviseHipData(ptr, len, hipMemAdviseUnsetCoarseGrain, getHipDevice());
}


/*!
 * \brief Free device data array.
 */
inline void deallocHipDeviceData(void* dptr)
{
  CAMP_HIP_API_INVOKE_AND_CHECK( hipFree, dptr );
}

/*!
 * \brief Free managed data array.
 */
inline void deallocHipManagedData(void* mptr)
{
  CAMP_HIP_API_INVOKE_AND_CHECK( hipFree, mptr );
}

/*!
 * \brief Free pinned data array.
 */
inline void deallocHipPinnedData(void* pptr)
{
  CAMP_HIP_API_INVOKE_AND_CHECK( hipHostFree, pptr );
}

}  // closing brace for detail namespace

}  // closing brace for rajaperf namespace

#endif // RAJA_ENABLE_HIP

#endif  // closing endif for header file include guard
