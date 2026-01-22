#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################


BUILD_SUFFIX=sycl
: ${BUILD_TYPE:=RelWithDebInfo}
RAJA_HOSTCONFIG=../tpl/RAJA/host-configs/alcf-builds/sycl.cmake

rm -rf build_${BUILD_SUFFIX}_${USER} >/dev/null
mkdir build_${BUILD_SUFFIX}_${USER} && cd build_${BUILD_SUFFIX}_${USER}

cmake \
  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
  -C ${RAJA_HOSTCONFIG} \
  -DENABLE_OPENMP=Off \
  -DENABLE_CUDA=Off \
  -DRAJA_PERFSUITE_GPU_BLOCKSIZES=64,128,256,512,1024 \
  -DENABLE_TARGET_OPENMP=Off \
  -DENABLE_ALL_WARNINGS=Off \
  -DENABLE_SYCL=On \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_LINKER=icpx \
  "$@" \
  ..

make -j 18
