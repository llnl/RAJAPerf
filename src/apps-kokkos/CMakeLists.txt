###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
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
