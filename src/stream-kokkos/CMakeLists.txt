###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

blt_add_library(
  NAME stream-kokkos
  SOURCES ADD-Kokkos.cpp 
          COPY-Kokkos.cpp 
          DOT-Kokkos.cpp 
          MUL-Kokkos.cpp 
          TRIAD-Kokkos.cpp
  INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../stream
  DEPENDS_ON common ${RAJA_PERFSUITE_DEPENDS}
  )
