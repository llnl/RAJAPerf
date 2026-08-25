##############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

##
## Note that we build with 'make -j 16' for newer targets and 'make -j 6' 
## for older targets on GitHub Actions. This is reflected in the 'make' 
## commands below. This seems to work best for throughput.
##

FROM ghcr.io/llnl/radiuss:ubuntu-24.04-gcc-12 AS gcc12
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DRAJA_ENABLE_WARNINGS=On -DRAJA_ENABLE_WARNINGS_AS_ERRORS=On -DENABLE_OPENMP=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

FROM ghcr.io/llnl/radiuss:ubuntu-24.04-gcc-12 AS gcc12_debug
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DRAJA_ENABLE_WARNINGS=On -DRAJA_ENABLE_WARNINGS_AS_ERRORS=On -DENABLE_OPENMP=On -DPERFSUITE_RUN_SHORT_TEST=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

FROM ghcr.io/llnl/radiuss:ubuntu-24.04-gcc-13 AS gcc13
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DRAJA_ENABLE_WARNINGS=On -DRAJA_ENABLE_WARNINGS_AS_ERRORS=On -DENABLE_OPENMP=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

FROM ghcr.io/llnl/radiuss:ubuntu-24.04-gcc-13 AS gcc13_desul
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DRAJA_ENABLE_WARNINGS=On -DRAJA_ENABLE_WARNINGS_AS_ERRORS=On -DENABLE_OPENMP=On -DRAJA_ENABLE_DESUL_ATOMICS=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

# SGS Testing if this fixes the pipeline
# Is there a better way to do this, likely want a new radiuss image so we don't rebuild this as part of the CI.
FROM ghcr.io/llnl/radiuss:ubuntu-24.04-clang-14 AS clang14_debug
ENV GTEST_COLOR=1
ENV DEBIAN_FRONTEND=noninteractive

# Install GCC 10 for improved compatibility with Clang 14 and create isolated install directory to prevent CLANG picking up GCC 13 from /usr
# Note CLANG 14 --gcc-toolchain option searches the directory for latest and Ubuntu install mixes headers/libs together in /usr
# This is rather ugly
RUN sudo apt-get update && \
    sudo apt-get install -y gcc-10 g++-10 && \
    sudo apt-get clean && \
    sudo rm -rf /var/lib/apt/lists/* && \
    sudo mkdir -p /opt/gcc-10/lib/gcc/x86_64-linux-gnu /opt/gcc-10/include/c++ /opt/gcc-10/include/x86_64-linux-gnu/c++ && \
    sudo ln -s /usr/lib/gcc/x86_64-linux-gnu/10 /opt/gcc-10/lib/gcc/x86_64-linux-gnu/10 && \
    sudo ln -s /usr/include/c++/10 /opt/gcc-10/include/c++/10 && \
    sudo ln -s /usr/include/x86_64-linux-gnu/c++/10 /opt/gcc-10/include/x86_64-linux-gnu/c++/10

COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_C_COMPILER=clang-14 \
          -DCMAKE_CXX_COMPILER=clang++-14 \
          -DCMAKE_C_FLAGS="--gcc-toolchain=/opt/gcc-10" \
          -DCMAKE_CXX_FLAGS="--gcc-toolchain=/opt/gcc-10" \
          -DCMAKE_BUILD_TYPE=Debug  -DENABLE_OPENMP=On -DPERFSUITE_RUN_SHORT_TEST=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

# SGS Testing if this fixes the pipeline
# Is there a better way to do this, likely want a new radiuss image so we don't rebuild this as part of the CI.
FROM ghcr.io/llnl/radiuss:ubuntu-24.04-clang-14 AS clang14_desul
ENV GTEST_COLOR=1
ENV DEBIAN_FRONTEND=noninteractive

# Install GCC 10 for improved compatibility with Clang 14 and create isolated install directory to prevent CLANG picking up GCC 13 from /usr
# Note CLANG 14 --gcc-toolchain option searches the directory for latest and Ubuntu install mixes headers/libs together in /usr
# This is rather ugly
RUN sudo apt-get update && \
    sudo apt-get install -y gcc-10 g++-10 && \
    sudo apt-get clean && \
    sudo rm -rf /var/lib/apt/lists/* && \
    sudo mkdir -p /opt/gcc-10/lib/gcc/x86_64-linux-gnu /opt/gcc-10/include/c++ /opt/gcc-10/include/x86_64-linux-gnu/c++ && \
    sudo ln -s /usr/lib/gcc/x86_64-linux-gnu/10 /opt/gcc-10/lib/gcc/x86_64-linux-gnu/10 && \
    sudo ln -s /usr/include/c++/10 /opt/gcc-10/include/c++/10 && \
    sudo ln -s /usr/include/x86_64-linux-gnu/c++/10 /opt/gcc-10/include/x86_64-linux-gnu/c++/10

COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_C_COMPILER=clang-14 \
          -DCMAKE_CXX_COMPILER=clang++-14 \
          -DCMAKE_C_FLAGS="--gcc-toolchain=/opt/gcc-10" \
          -DCMAKE_CXX_FLAGS="--gcc-toolchain=/opt/gcc-10" \
          -DCMAKE_BUILD_TYPE=Release -DENABLE_OPENMP=On -DRAJA_ENABLE_DESUL_ATOMICS=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    ctest -T test --output-on-failure && \
    make clean

## Don't run tests due to failure in RAJA launch tests with new reducer interface.
FROM ghcr.io/llnl/radiuss:ubuntu-24.04-intel-2024.2 AS intel2024_2
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN /bin/bash -c "source /opt/intel/oneapi/setvars.sh 2>&1 && export PATH=/opt/intel/oneapi/compiler/2024.2/bin/:\$PATH && cmake -DCMAKE_CXX_COMPILER=icpx -DCMAKE_BUILD_TYPE=Release -DENABLE_OPENMP=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    make clean"

## Don't run tests due to failure in RAJA launch tests with new reducer interface.
FROM ghcr.io/llnl/radiuss:ubuntu-24.04-intel-2024.2 AS intel2024_2_debug
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN /bin/bash -c "source /opt/intel/oneapi/setvars.sh 2>&1 && export PATH=/opt/intel/oneapi/compiler/2024.2/bin/:\$PATH && cmake -DCMAKE_CXX_COMPILER=icpx -DCMAKE_BUILD_TYPE=Debug -DENABLE_OPENMP=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    make clean"

## Don't run tests due to failure in RAJA launch tests with new reducer interface.
FROM ghcr.io/llnl/radiuss:ubuntu-24.04-intel-2024.2 AS intel2024_2_sycl
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN /bin/bash -c "source /opt/intel/oneapi/setvars.sh 2>&1 && export PATH=/opt/intel/oneapi/compiler/2024.2/bin/:\$PATH && export LD_LIBRARY_PATH=/opt/intel/oneapi/2024.2/lib:\$LD_LIBRARY_PATH && cmake -DCMAKE_CXX_COMPILER=icpx -DCMAKE_CXX_FLAGS=\"-fsycl -fsycl-unnamed-lambda\" -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_OPENMP=Off -DRAJA_ENABLE_SYCL=On -DRAJA_ENABLE_DESUL_ATOMICS=On -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    make clean"

FROM ghcr.io/llnl/radiuss:hip-6.4.3-ubuntu-24.04 AS rocm6_4_3_desul
ENV GTEST_COLOR=1
COPY . /home/raja/workspace
WORKDIR /home/raja/workspace/build
RUN cmake -DCMAKE_CXX_COMPILER=/opt/rocm-6.4.3/bin/amdclang++ -DROCM_PATH=/opt/rocm-6.4.3 -DGPU_TARGETS=gfx906 -DAMDGPU_TARGETS=gfx906 -DCMAKE_BUILD_TYPE=Release -DENABLE_HIP=On -DRAJA_ENABLE_DESUL_ATOMICS=On -DRAJA_ENABLE_WARNINGS_AS_ERRORS=Off -DBLT_CXX_STD=c++20 .. && \
    make -j 16 &&\
    make clean
