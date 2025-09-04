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
  const Index_type ibegin = 0 ;
  const Index_type iend     = m_tri_per_intsc * getActualProblemSize() ;

  const Size_type  n_subz_intsc = 8 * getActualProblemSize() ;
  const Size_type  nisc_stage   = n_subz_intsc ;

  const Size_type  n_szgrp     = ( n_subz_intsc + 7 ) / 8 ;
  const size_t     gsize_fixup = RAJA_DIVIDE_CEILING_INT(n_szgrp, work_group_size) ;
  const Index_type iend_fixup  = gsize_fixup * work_group_size ;

  const Size_type  n_szpairs   = n_subz_intsc ;

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

          long ith     = i ;
          long blksize = work_group_size ;
          long blk     = i / blksize ;
          double *vv_out = (double*) vv_int + 8*blk ;
          INTSC_HEXRECT_BODY;
        });
      });

      qu->submit([&] (sycl::handler& h) {
        h.parallel_for(sycl::nd_range<1>(gsize_fixup, work_group_size),
                       [=] (sycl::nd_item<1> item ) {

          Index_type i = item.get_global_id(0) + ibegin;
          int ith     = i ;
          FIXUP_VV_BODY ;
        });
      });

    }
    stopTimer();

  } else if ( vid == RAJA_SYCL ) {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::forall< RAJA::sycl_exec<work_group_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend), [=] (Index_type i) {

        long ith     = i ;
        long blksize = work_group_size ;
        long blk     = i / blksize ;
        double *vv_out = (double*) vv_int + 8*blk ;
        INTSC_HEXRECT_BODY;
      });

      RAJA::forall< RAJA::sycl_exec<work_group_size, true /*async*/> >( res,
        RAJA::RangeSegment(ibegin, iend_fixup), [=] (Index_type i) {
        int ith     = i ;
        FIXUP_VV_BODY ;
        }

    }
    stopTimer();

  } else {
     getCout() << "\n  INTSC_HEXRECT : Unknown Sycl variant id = " << vid << std::endl;
  }
}

RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(INTSC_HEXHEX, Sycl)

} // end namespace apps
} // end namespace rajaperf

#endif  // RAJA_ENABLE_SYCL
