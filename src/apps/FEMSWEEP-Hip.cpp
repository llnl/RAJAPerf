//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"
#include "FEMSWEEP-SOLVER.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_HIP)

#include "common/HipDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void FEMSweep3D( const Real_ptr Bdat,
                            const Real_ptr Adat,
                            const Real_ptr Fdat,
                            Real_ptr Xdat,
                            const Real_ptr Sgdat,
                            const Real_ptr M0dat,
                            const Index_type ne,
                            const Index_type ng,
                            const Index_type sharedinteriorfaces,
                            const Index_ptr nhpaa_r,
                            const Index_ptr ohpaa_r,
                            const Index_ptr phpaa_r,
                            const Index_ptr order_r,
                            const Index_ptr AngleElem2FaceType,
                            const Index_ptr elem_to_faces,
                            const Index_ptr F_g2l,
                            const Index_ptr idx1,
                            const Index_ptr idx2 )
{
  const Index_type ag = hipBlockIdx_x * block_size + hipThreadIdx_x;
  FEMSWEEP_KERNEL;
}


template < size_t block_size >
__launch_bounds__(block_size)
__global__ void FEMSweep3DParts( const Real_ptr Bdat,
                            const Real_ptr Adat,
                            const Real_ptr Fdat,
                            Real_ptr Xdat,
                            const Real_ptr Sgdat,
                            const Real_ptr M0dat,
                            const Index_type ne,
                            const Index_type ng,
                            const Index_type sharedinteriorfaces,
                            const Index_ptr nhpaa_r,
                            const Index_ptr ohpaa_r,
                            const Index_ptr phpaa_r,
                            const Index_ptr order_r,
                            const Index_ptr AngleElem2FaceType,
                            const Index_ptr elem_to_faces,
                            const Index_ptr F_g2l,
                            const Index_ptr idx1,
                            const Index_ptr idx2 )
{
  const Index_type ag = hipBlockIdx_x * block_size + hipThreadIdx_x;
  FEMSWEEP_KERNEL_FROM_PARTS;
}


#define g_block_sz (32)
#define a_block_sz (block_size/g_block_sz)

template < size_t block_size_x, size_t block_size_y >
__launch_bounds__(block_size_x*block_size_y)
__global__ void FEMSweep3D2dParts( const Real_ptr Bdat,
                            const Real_ptr Adat,
                            const Real_ptr Fdat,
                            Real_ptr Xdat,
                            const Real_ptr Sgdat,
                            const Real_ptr M0dat,
                            const Index_type ne,
                            const Index_type na,
                            const Index_type ng,
                            const Index_type sharedinteriorfaces,
                            const Index_ptr nhpaa_r,
                            const Index_ptr ohpaa_r,
                            const Index_ptr phpaa_r,
                            const Index_ptr order_r,
                            const Index_ptr AngleElem2FaceType,
                            const Index_ptr elem_to_faces,
                            const Index_ptr F_g2l,
                            const Index_ptr idx1,
                            const Index_ptr idx2 )
{
  const Index_type g = hipBlockIdx_x * block_size_x + hipThreadIdx_x;
  const Index_type a = hipBlockIdx_y * block_size_y + hipThreadIdx_y;
  if (a >= na || g >= ng) { return; }
  FEMSWEEP_KERNEL_SETUP;
  Index_type nehp_pos = 0;
  for (Index_type hp = 0; hp < nhp; ++hp)
  {
     const Index_type nehp = phpaa_r[ohp + hp];
     for (Index_type k = 0; k < nehp; ++k)
     {
        FEMSWEEP_KERNEL_PER_ELEMENT;
     }
     nehp_pos += nehp;
  }
}

template < size_t block_size >
__launch_bounds__(block_size)
__global__ void FEMSweep3DHpPar2dParts( const Real_ptr Bdat,
                            const Real_ptr Adat,
                            const Real_ptr Fdat,
                            Real_ptr Xdat,
                            const Real_ptr Sgdat,
                            const Real_ptr M0dat,
                            const Index_type ne,
                            const Index_type ng,
                            const Index_type sharedinteriorfaces,
                            const Index_ptr nhpaa_r,
                            const Index_ptr ohpaa_r,
                            const Index_ptr phpaa_r,
                            const Index_ptr order_r,
                            const Index_ptr AngleElem2FaceType,
                            const Index_ptr elem_to_faces,
                            const Index_ptr F_g2l,
                            const Index_ptr idx1,
                            const Index_ptr idx2 )
{
  const Index_type a = hipBlockIdx_y;
  const Index_type g = hipBlockIdx_x;
  FEMSWEEP_KERNEL_SETUP;
  Index_type nehp_pos = 0;
  for (Index_type hp = 0; hp < nhp; ++hp)
  {
     const Index_type nehp = phpaa_r[ohp + hp];
     for (Index_type k = hipThreadIdx_x; k < nehp; k+=block_size)
     {
        FEMSWEEP_KERNEL_PER_ELEMENT;
     } __syncthreads();
     nehp_pos += nehp;
  }
}

template < size_t block_size >
void FEMSWEEP::runHipVariantImpl(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_HIP : {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         const size_t grid_size = RAJA_DIVIDE_CEILING_INT(na*ng, block_size);
         constexpr size_t shmem = 0;

         RPlaunchHipKernel( (FEMSweep3D<block_size>),
                            grid_size, block_size,
                            shmem, res.get_stream(),
                            Bdat,
                            Adat,
                            Fdat,
                            Xdat,
                            Sgdat,
                            M0dat,
                            ne,
                            ng,
                            sharedinteriorfaces,
                            nhpaa_r,
                            ohpaa_r,
                            phpaa_r,
                            order_r,
                            AngleElem2FaceType,
                            elem_to_faces,
                            F_g2l,
                            idx1,
                            idx2 );

      }
      stopTimer();

      break;
    }

    case RAJA_HIP : {

      const size_t grid_size = RAJA_DIVIDE_CEILING_INT(na*ng, block_size);
      constexpr bool async = true;

      using launch_policy =
          RAJA::LaunchPolicy<RAJA::hip_launch_t<async, block_size>>;

      using outer_x =
          RAJA::LoopPolicy<RAJA::hip_global_size_x_direct<block_size>>;

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         RAJA::launch<launch_policy>( res,
             RAJA::LaunchParams(RAJA::Teams(grid_size),
                                RAJA::Threads(block_size)),
             [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx) {
               RAJA::loop<outer_x>(ctx, RAJA::RangeSegment(0, na * ng),
                 [&](Index_type ag) {
                   FEMSWEEP_KERNEL;
                 });  // ag loop
         });  // RAJA Launch

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown HIP variant id = " << vid << std::endl;
    }

  }

}

template < size_t block_size >
void FEMSWEEP::runHipVariantImplParts(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_HIP : {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         const size_t grid_size = RAJA_DIVIDE_CEILING_INT(na*ng, block_size);
         constexpr size_t shmem = 0;

         RPlaunchHipKernel( (FEMSweep3DParts<block_size>),
                            grid_size, block_size,
                            shmem, res.get_stream(),
                            Bdat,
                            Adat,
                            Fdat,
                            Xdat,
                            Sgdat,
                            M0dat,
                            ne,
                            ng,
                            sharedinteriorfaces,
                            nhpaa_r,
                            ohpaa_r,
                            phpaa_r,
                            order_r,
                            AngleElem2FaceType,
                            elem_to_faces,
                            F_g2l,
                            idx1,
                            idx2 );

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown HIP variant id = " << vid << std::endl;
    }

  }

}


template < size_t block_size >
void FEMSWEEP::runHipVariantImpl2dParts(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_HIP : {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         const dim3 grid_size {
              (decltype(dim3{}.x))RAJA_DIVIDE_CEILING_INT(ng, g_block_sz),
              (decltype(dim3{}.x))RAJA_DIVIDE_CEILING_INT(na, a_block_sz)
            };
         constexpr size_t shmem = 0;

         RPlaunchHipKernel( (FEMSweep3D2dParts<g_block_sz, a_block_sz>),
                            grid_size, {g_block_sz, a_block_sz},
                            shmem, res.get_stream(),
                            Bdat,
                            Adat,
                            Fdat,
                            Xdat,
                            Sgdat,
                            M0dat,
                            ne,
                            na,
                            ng,
                            sharedinteriorfaces,
                            nhpaa_r,
                            ohpaa_r,
                            phpaa_r,
                            order_r,
                            AngleElem2FaceType,
                            elem_to_faces,
                            F_g2l,
                            idx1,
                            idx2 );

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown HIP variant id = " << vid << std::endl;
    }

  }

}

template < size_t block_size >
void FEMSWEEP::runHipVariantImplHpPar2dParts(VariantID vid)
{
  setBlockSize(block_size);

  const Index_type run_reps = getRunReps();

  auto res{getHipResource()};

  FEMSWEEP_DATA_SETUP;

  switch ( vid ) {

    case Base_HIP : {

      startTimer();
      // Loop counter increment uses macro to quiet C++20 compiler warning
      for (RepIndex_type irep = 0; irep < run_reps; RP_REPCOUNTINC(irep)) {

         const dim3 grid_size {(decltype(dim3{}.x))ng, (decltype(dim3{}.x))na};
         constexpr size_t shmem = 0;

         RPlaunchHipKernel( (FEMSweep3DHpPar2dParts<block_size>),
                            grid_size, block_size,
                            shmem, res.get_stream(),
                            Bdat,
                            Adat,
                            Fdat,
                            Xdat,
                            Sgdat,
                            M0dat,
                            ne,
                            ng,
                            sharedinteriorfaces,
                            nhpaa_r,
                            ohpaa_r,
                            phpaa_r,
                            order_r,
                            AngleElem2FaceType,
                            elem_to_faces,
                            F_g2l,
                            idx1,
                            idx2 );

      }
      stopTimer();

      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown HIP variant id = " << vid << std::endl;
    }

  }

}

// RAJAPERF_GPU_BLOCK_SIZE_TUNING_DEFINE_BOILERPLATE(FEMSWEEP, Hip, Base_HIP, RAJA_HIP)
void FEMSWEEP::defineHipVariantTunings()
{
  for (VariantID vid : {Base_HIP, RAJA_HIP}) {
    seq_for(gpu_block_sizes_type{}, [&](auto block_size) {
      if (run_params.numValidGPUBlockSize() == 0u ||
          run_params.validGPUBlockSize(block_size)) {
        if (block_size == 0u) {
          addVariantTuning<&FEMSWEEP::runHipVariantImpl<block_size>>(
              vid, "block_auto");
        } else {
          addVariantTuning<&FEMSWEEP::runHipVariantImpl<block_size>>(
              vid, "block_"+std::to_string(block_size));
          if (vid == Base_HIP) {
            addVariantTuning<&FEMSWEEP::runHipVariantImplParts<block_size>>(
                vid, "parts_"+std::to_string(block_size));
            addVariantTuning<&FEMSWEEP::runHipVariantImpl2dParts<block_size>>(
                vid, "2dParts_"+std::to_string(block_size));
            addVariantTuning<&FEMSWEEP::runHipVariantImplHpPar2dParts<block_size>>(
                vid, "hpPar2dParts_"+std::to_string(block_size));
          }
        }
      }
    });
  }
}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_HIP
