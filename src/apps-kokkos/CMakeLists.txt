###############################################################################
# Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
# and RAJA Performance Suite project contributors.
# See the RAJAPerf/LICENSE file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################


blt_add_library(
  NAME apps-kokkos
  SOURCES
          NODAL_ACCUMULATION_3D-Kokkos.cpp
  INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../apps
  DEPENDS_ON common ${RAJA_PERFSUITE_DEPENDS}
  )
