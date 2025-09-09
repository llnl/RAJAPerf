//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

#define WARPSIZE 64
#define __shfl_xor_sync(mask,val,n) __shfl_xor(val,n)

namespace rajaperf
{
namespace apps
{

template < size_t block_size >
__launch_bounds__(block_size,3)
__global__ void intsc_hexrect_hip
    ( Real_ptr xdnode,     // [ndnodes] x coordinates for donor
      Real_ptr ydnode,     // [ndnodes] y coordinates for donor
      Real_ptr zdnode,     // [ndnodes] z coordinates for donor
      Int_ptr znlist,      // [donor zones][8] donor zone node list
      char *ncord_gpu,     //  target dimensions and coordinates
      Int_ptr intsc_d,   // [nrecords] donor zones to intersect
      Int_ptr intsc_t,   // [nrecords] target zones to intersect
      long const nrecords,  // Number of threads (one thread per record)
      Real_ptr records )  // output volumes, moments
{
  long blksize = block_size ;        // blocksize = 64  must <= nth_per_isc
  long blk     = blockIdx.x ;
  long irec    = blk*blksize + threadIdx.x ;   // which thread with offset

  int const max_polygon_pts = 10 ;
  __shared__ double xd_work[ (3 * max_polygon_pts+1) * 64 ] ;

  // polygons (an odd number of doubles per thread to reduce bank conflicts)
  double *my_qx = xd_work + (3 * max_polygon_pts+1) * threadIdx.x ;

  INTSC_HEXRECT_BODY ;
}



template < size_t block_size >
void INTSC_HEXRECT::runHipVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = m_nrecords ;

  auto res{getHipResource()};

  INTSC_HEXRECT_DATA_SETUP;

  if ( vid == Base_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);
      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (intsc_hexrect_hip<block_size>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         m_xdnode, m_ydnode, m_zdnode, m_znlist,
                         m_ncord, m_intsc_d, m_intsc_t,
                         m_nrecords, m_records ) ;
    }
    stopTimer();

  } else if ( vid == Lambda_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(iend, block_size);

      auto intsc_hexrect_lambda = [=] __device__
          ( Index_type i )
         {
           long blksize = block_size ;
           long blk     = blockIdx.x ;
           long irec    = blk*blksize + threadIdx.x ;

           int const max_polygon_pts = 10 ;
           __shared__ double xd_work[ (3 * max_polygon_pts+1) * 64 ] ;

           double *my_qx = xd_work + (3 * max_polygon_pts+1) * threadIdx.x ;

           INTSC_HEXRECT_BODY;
         };

      constexpr size_t shmem = 0;

      RPlaunchHipKernel( (lambda_hip_forall<block_size,
                          decltype(intsc_hexrect_lambda)>),
                         grid_size, block_size,
                         shmem, res.get_stream(),
                         ibegin, iend,
                         intsc_hexrect_lambda );

    }
    stopTimer();

  } else if ( vid == RAJA_HIP ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::hip_exec<block_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] __device__ (Index_type i)
          {
            long blksize = block_size ;
            long blk     = blockIdx.x ;
            long irec    = blk*blksize + threadIdx.x ;

            int const max_polygon_pts = 10 ;
            __shared__ double xd_work[ (3 * max_polygon_pts+1) * 64 ] ;

            double *my_qx = xd_work + (3 * max_polygon_pts+1) * threadIdx.x ;

            INTSC_HEXRECT_BODY;
          }
      ) ;
    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXRECT : Unknown Hip variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXRECT, Hip)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_HIP
