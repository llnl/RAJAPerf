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

if [[ $# -lt 4 ]]; then
  echo
  echo "You must pass 4 or more arguments to the script (in this order): "
  echo "   1) mvapich2 version number"
  echo "   2) NVCC compiler version number"
  echo "   3) CUDA compute architecture"
  echo "   4) GCC compiler version number"
  echo "   5) optional cmake version
  echo
  echo "For example: "
  echo "    toss4_mvapich2_nvcc_gcc.sh 2.3.7 12.2.2 90 10.3.1 [3.27.4]"
  exit 1
fi

MPI_VER=$1
NVCC_COMP_VER=$2
NVCC_COMP_ARCH=$3
GCC_COMP_VER=$4

# Detect optional fourth positional argument as a CMake version if it looks like N.M or N.M.P
# Otherwise, treat it as a normal CMake argument.
if [ -n "$4" ] && [[ "$4" =~ ^[0-9]+(\.[0-9]+)*$ ]]; then
  CMAKE_VER=$4
  shift 4
else
  CMAKE_VER=$DEFAULT_CMAKE_VER
  shift 3
fi

BUILD_SUFFIX=lc_toss4-mvapich2-${MPI_VER}-nvcc-${NVCC_COMP_VER}-${NVCC_COMP_ARCH}-gcc-${GCC_COMP_VER}
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
  -DMPI_C_COMPILER="/usr/tce/packages/mvapich2/mvapich2-${MPI_VER}-gcc-${GCC_COMP_VER}/bin/mpicc" \
  -DMPI_CXX_COMPILER="/usr/tce/packages/mvapich2/mvapich2-${MPI_VER}-gcc-${GCC_COMP_VER}/bin/mpicxx" \
  -DCUDA_TOOLKIT_ROOT_DIR=/usr/tce/packages/cuda/cuda-${NVCC_COMP_VER} \
  -DCMAKE_CUDA_COMPILER=/usr/tce/packages/cuda/cuda-${NVCC_COMP_VER}/bin/nvcc \
  -DCMAKE_CUDA_ARCHITECTURES="${NVCC_COMP_ARCH}" \
  -DCMAKE_C_COMPILER=/usr/tce/packages/gcc/gcc-${GCC_COMP_VER}/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/tce/packages/gcc/gcc-${GCC_COMP_VER}/bin/g++ \
  -DBLT_CXX_STD=c++20 \
  -C ${RAJA_HOSTCONFIG} \
  -DENABLE_MPI=ON \
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
