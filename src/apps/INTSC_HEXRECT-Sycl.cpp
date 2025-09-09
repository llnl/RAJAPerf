//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INTSC_HEXRECT.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_SYCL)

#include "common/SyclDataUtils.hpp"

#include "AppsData.hpp"

#include <iostream>

#define WARPSIZE 32
#define __shfl_xor_sync(mask,val,n) __shfl_xor(val,n)

namespace rajaperf
{
namespace apps
{


template < size_t work_group_size >
void INTSC_HEXRECT::runSyclVariantImpl(VariantID vid)
{
  const Index_type run_reps = getRunReps();
  const Index_type ibegin   = 0 ;
  const Index_type iend     = m_nrecords ;

  auto res{getSyclResource()};
  auto qu = res.get_queue();

  INTSC_HEXRECT_DATA_SETUP;

  if ( vid == Base_SYCL ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      const size_t global_size = work_group_size * RAJA_DIVIDE_CEILING_INT(iend, work_group_size);

      qu->submit([&] (sycl::handler& h) {
        h.parallel_for(sycl::nd_range<1>(global_size, work_group_size),
                       [=] (sycl::nd_item<1> item ) {

          Index_type i = item.get_global_id(0) + ibegin;

          long irec     = i ;
          long blksize = work_group_size ;
          long blk     = i / blksize ;

          int const max_polygon_pts = 10 ;
          double xd_work[ (3 * max_polygon_pts+1) ] ;
          double *my_qx = xd_work ;

          INTSC_HEXRECT_BODY;
        });
      });

    }
    stopTimer();

  } else if ( vid == RAJA_SYCL ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::sycl_exec<work_group_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] (Index_type i) {

          Index_type i = item.get_global_id(0) + ibegin;

          long irec     = i ;
          long blksize = work_group_size ;
          long blk     = i / blksize ;

          int const max_polygon_pts = 10 ;
          double xd_work[ (3 * max_polygon_pts+1) ] ;
          double *my_qx = xd_work ;

          INTSC_HEXRECT_BODY;
      });

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXRECT : Unknown Sycl variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXRECT, Sycl)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_SYCL
