#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

if [[ $# -lt 1 ]]; then
  echo
  echo "You must pass a compiler version number to script. For example,"
  echo "    toss4_icpx.sh 2022.1.0"
  exit
fi

COMP_VER=$1
shift 1

BUILD_SUFFIX=lc_toss4-icpx-${COMP_VER}
RAJA_HOSTCONFIG=../tpl/RAJA/host-configs/lc-builds/toss4/icpx_X.cmake

echo
echo "Creating build directory build_${BUILD_SUFFIX} and generating configuration in it"
echo "Configuration extra arguments:"
echo "   $@"
echo

rm -rf build_${BUILD_SUFFIX} 2>/dev/null
mkdir build_${BUILD_SUFFIX} && cd build_${BUILD_SUFFIX}

module load cmake/3.23.1

##
# CMake option -DRAJA_ENABLE_FORCEINLINE_RECURSIVE=Off used to speed up compile
# times at a potential cost of slower 'forall' execution.
##

source /usr/tce/packages/intel/intel-${COMP_VER}/setvars.sh

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=icpx \
  -DCMAKE_C_COMPILER=icx \
  -DBLT_CXX_STD=c++17 \
  -C ${RAJA_HOSTCONFIG} \
  -DRAJA_ENABLE_FORCEINLINE_RECURSIVE=Off \
  -DENABLE_OPENMP=On \
  -DCMAKE_INSTALL_PREFIX=../install_${BUILD_SUFFIX} \
  "$@" \
  ..
