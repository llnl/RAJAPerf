#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

# Default CMake version if not provided
DEFAULT_CMAKE_VER=3.25.2

if [[ $# -lt 3 ]]; then
  echo
  echo "You must pass 3 or more arguments to the script (in this order): "
  echo "   1) NVCC compiler version number"
  echo "   2) CUDA compute architecture"
  echo "   3) GCC compiler version number"
  echo "   4) optional cmake version"
  echo
  echo "For example: "
  echo "    toss4_nvcc_gcc.sh 12.2.2 90 10.3.1 [3.27.4]"
  exit 1
fi

NVCC_COMP_VER=$1
NVCC_COMP_ARCH=$2
GCC_COMP_VER=$3

# Pick a default C++ standard compatible with the CUDA toolchain.
# CUDA 11.x does not support C++20 in NVCC mode (BLT enforces this).
NVCC_MAJOR="${NVCC_COMP_VER%%.*}"
BLT_CXX_STD="c++17"
if [[ "${NVCC_MAJOR}" =~ ^[0-9]+$ && "${NVCC_MAJOR}" -ge 12 ]]; then
  BLT_CXX_STD="c++20"
fi

# Detect optional fourth positional argument as a CMake version if it looks like N.M or N.M.P
# Otherwise, treat it as a normal CMake argument.
if [ -n "$4" ] && [[ "$4" =~ ^[0-9]+(\.[0-9]+)*$ ]]; then
  CMAKE_VER=$4
  shift 4
else
  CMAKE_VER=$DEFAULT_CMAKE_VER
  shift 3
fi

BUILD_SUFFIX=lc_toss4-nvcc-${NVCC_COMP_VER}-${NVCC_COMP_ARCH}-gcc-${GCC_COMP_VER}
RAJA_HOSTCONFIG=../tpl/RAJA/host-configs/lc-builds/toss4/gcc_X.cmake

echo
echo "Creating build directory build_${BUILD_SUFFIX} and generating configuration in it"
echo "Using CMake version: ${CMAKE_VER}"
echo "Configuration extra arguments:"
echo "   $@"
echo

rm -rf build_${BUILD_SUFFIX} 2>/dev/null
mkdir build_${BUILD_SUFFIX} && cd build_${BUILD_SUFFIX}

module load cmake/${CMAKE_VER}

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCUDA_TOOLKIT_ROOT_DIR=/usr/tce/packages/cuda/cuda-${NVCC_COMP_VER} \
  -DCMAKE_CUDA_COMPILER=/usr/tce/packages/cuda/cuda-${NVCC_COMP_VER}/bin/nvcc \
  -DCMAKE_CUDA_ARCHITECTURES="${NVCC_COMP_ARCH}" \
  -DCMAKE_CXX_COMPILER=/usr/tce/packages/gcc/gcc-${GCC_COMP_VER}/bin/g++ \
  -DBLT_CXX_STD=${BLT_CXX_STD} \
  -C ${RAJA_HOSTCONFIG} \
  -DENABLE_OPENMP=On \
  -DENABLE_CUDA=On \
  -DCMAKE_INSTALL_PREFIX=../install_${BUILD_SUFFIX} \
  "$@" \
  ..

echo
echo "***********************************************************************"
echo
echo "cd into directory build_${BUILD_SUFFIX} and run make to build RAJA"
echo
echo "***********************************************************************"
