###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################


blt_add_library(
  NAME lcals-kokkos
  SOURCES
          DIFF_PREDICT-Kokkos.cpp
          EOS-Kokkos.cpp
          FIRST_DIFF-Kokkos.cpp
          FIRST_MIN-Kokkos.cpp
          FIRST_SUM-Kokkos.cpp
          GEN_LIN_RECUR-Kokkos.cpp
          HYDRO_1D-Kokkos.cpp
          HYDRO_2D-Kokkos.cpp
          INT_PREDICT-Kokkos.cpp
          PLANCKIAN-Kokkos.cpp
          TRIDIAG_ELIM-Kokkos.cpp
  INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../lcals
  DEPENDS_ON common ${RAJA_PERFSUITE_DEPENDS}
  )
