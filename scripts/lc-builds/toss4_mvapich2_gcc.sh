#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

if [[ $# -lt 2 ]]; then
  echo
  echo "You must pass 2 or more arguments to the script (in this order): "
  echo "   1) mvapich2 compiler version number"
  echo "   2) gcc compiler version number"
  echo "   3...) optional arguments to cmake"
  echo
  echo "You must pass a compiler version number to script. For example,"
  echo "    toss4_mvapich2_gcc.sh 2.3.7 10.3.1"
  exit
fi

MPI_VER=$1
COMP_VER=$2
shift 2

BUILD_SUFFIX=lc_toss4-mvapich2-${MPI_VER}-gcc-${COMP_VER}
RAJA_HOSTCONFIG=../tpl/RAJA/host-configs/lc-builds/toss4/gcc_X.cmake

echo
echo "Creating build directory build_${BUILD_SUFFIX} and generating configuration in it"
echo "Configuration extra arguments:"
echo "   $@"
echo

rm -rf build_${BUILD_SUFFIX} 2>/dev/null
mkdir build_${BUILD_SUFFIX} && cd build_${BUILD_SUFFIX}

module load cmake/3.23.1

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DMPI_C_COMPILER="/usr/tce/packages/mvapich2/mvapich2-${MPI_VER}-gcc-${COMP_VER}/bin/mpicc" \
  -DMPI_CXX_COMPILER="/usr/tce/packages/mvapich2/mvapich2-${MPI_VER}-gcc-${COMP_VER}/bin/mpicxx" \
  -DCMAKE_C_COMPILER=/usr/tce/packages/gcc/gcc-${COMP_VER}/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/tce/packages/gcc/gcc-${COMP_VER}/bin/g++ \
  -DBLT_CXX_STD=c++17 \
  -C ${RAJA_HOSTCONFIG} \
  -DENABLE_MPI=ON \
  -DENABLE_OPENMP=On \
  -DCMAKE_INSTALL_PREFIX=../install_${BUILD_SUFFIX} \
  "$@" \
  ..

echo
echo "***********************************************************************"
echo
echo "cd into directory build_${BUILD_SUFFIX} and run make to build RAJA Perf Suite"
echo
echo "  Please note that you have to run with mpi when you run"
echo "  the RAJA Perf Suite; for example,"
echo
echo "    srun -n2 ./bin/raja-perf.exe"
echo
echo "***********************************************************************"
