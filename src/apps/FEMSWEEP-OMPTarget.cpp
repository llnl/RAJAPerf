//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-24, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEP.hpp"

#include "RAJA/RAJA.hpp"

#if defined(RAJA_ENABLE_TARGET_OPENMP)

#include "common/OpenMPTargetDataUtils.hpp"

#include <iostream>

namespace rajaperf
{
namespace apps
{


void FEMSWEEP::runOpenMPTargetVariant(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  switch ( vid ) {

    case Base_OpenMPTarget : {
      break;
    }

    case RAJA_OpenMPTarget : {
      break;
    }

    default : {
      getCout() << "\n FEMSWEEP : Unknown OpenMPTarget variant id = " << vid << std::endl;
    }

  }
}

} // end namespace apps
} // end namespace rajaperf

#endif // RAJA_ENABLE_TARGET_OPENMP
