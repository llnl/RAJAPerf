#------------------------------------------------------------------------------
# !!!! This is a generated file, edit at own risk !!!!
#------------------------------------------------------------------------------
# CMake executable path: /usr/bin/cmake
#------------------------------------------------------------------------------

set(CMAKE_PREFIX_PATH "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/blt-0.7.2-ooo3mtoxl3sqlmtlhxomybiy5we2vp3f;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/caliper-master-ilpr5ougvnd4lxc5kj63jldcw2abkmro;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/adiak-0.5.0-wf75jf2wsnfkvwnnk7io7peojnjrbdv3;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/libpfm4-4.13.0-2si4lb4hlakmttruzyqz7yafyivlh6bl;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/intel-oneapi-runtime-2025.2.0-2qx77zfl2erzkxfxxkaqshkxk55aivhz;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/gcc-runtime-13.3.1-nbxzaybnoicwx6tbg4k2lqumfynjffzk;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/compiler-wrapper-1.1.0-fx6x5jqjpwl6sqbvu6rznnuhsjwabuoy;/usr/tce/packages/intel/intel-2025.2.0/compiler/2025.2;/usr/tce/packages/mvapich2/mvapich2-2.3.7-intel-2025.2.0" CACHE STRING "")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH "ON" CACHE STRING "")

set(CMAKE_BUILD_RPATH "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/raja-perf-2025.12.1-yuxnhrq3yzymjezgxgfozqqjepw7rnem/lib;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/raja-perf-2025.12.1-yuxnhrq3yzymjezgxgfozqqjepw7rnem/lib64;;/usr/tce/backend/installations/linux-rhel8-x86_64/gcc-13.3.1/intel-oneapi-compilers-2025.2.0-ngszoacyp2i5d5nawitlyuh37jswjtie/compiler/2025.2/lib;/opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13" CACHE STRING "")

set(CMAKE_INSTALL_RPATH "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/raja-perf-2025.12.1-yuxnhrq3yzymjezgxgfozqqjepw7rnem/lib;/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/raja-perf-2025.12.1-yuxnhrq3yzymjezgxgfozqqjepw7rnem/lib64;;/usr/tce/backend/installations/linux-rhel8-x86_64/gcc-13.3.1/intel-oneapi-compilers-2025.2.0-ngszoacyp2i5d5nawitlyuh37jswjtie/compiler/2025.2/lib;/opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13" CACHE STRING "")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")

#------------------------------------------------------------------------------
# Compilers
#------------------------------------------------------------------------------
# Compiler Spec: intel-oneapi-compilers@2025.2.0/lnbf4kh6qlm2esza4vjfwbqzvewdmmi4
#------------------------------------------------------------------------------
if(DEFINED ENV{SPACK_CC})

  set(CMAKE_C_COMPILER "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/compiler-wrapper-1.1.0-fx6x5jqjpwl6sqbvu6rznnuhsjwabuoy/libexec/spack/oneapi/icx" CACHE PATH "")

  set(CMAKE_CXX_COMPILER "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/compiler-wrapper-1.1.0-fx6x5jqjpwl6sqbvu6rznnuhsjwabuoy/libexec/spack/oneapi/icpx" CACHE PATH "")

  # No Fortran compiler defined in spec
else()

  set(CMAKE_C_COMPILER "/usr/tce/packages/intel/intel-2025.2.0/compiler/2025.2/bin/icx" CACHE PATH "")

  set(CMAKE_CXX_COMPILER "/usr/tce/packages/intel/intel-2025.2.0/compiler/2025.2/bin/icpx" CACHE PATH "")

  # No Fortran compiler defined in spec
endif()

#------------------------------------------------------------------------------
# MPI
#------------------------------------------------------------------------------

set(MPI_C_COMPILER "/usr/tce/packages/mvapich2/mvapich2-2.3.7-intel-2025.2.0/bin/mpicc" CACHE PATH "")

set(MPI_CXX_COMPILER "/usr/tce/packages/mvapich2/mvapich2-2.3.7-intel-2025.2.0/bin/mpicxx" CACHE PATH "")

set(MPI_Fortran_COMPILER "/usr/tce/packages/mvapich2/mvapich2-2.3.7-intel-2025.2.0/bin/mpif90" CACHE PATH "")

set(MPIEXEC_EXECUTABLE "/usr/bin/flux;run" CACHE PATH "")

set(MPIEXEC_NUMPROC_FLAG "-n" CACHE STRING "")

set(ENABLE_MPI ON CACHE BOOL "")

#------------------------------------------------------------------------------
# Hardware
#------------------------------------------------------------------------------

#------------------------------------------------
# Package custom hardware settings
#------------------------------------------------

set(ENABLE_OPENMP OFF CACHE BOOL "")

set(ENABLE_CUDA OFF CACHE BOOL "")

set(ENABLE_HIP OFF CACHE BOOL "")

set(RAJA_ENABLE_TARGET_OPENMP OFF CACHE BOOL "")

#------------------------------------------------------------------------------
# TPLs
#------------------------------------------------------------------------------

set(BLT_SOURCE_DIR "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/blt-0.7.2-ooo3mtoxl3sqlmtlhxomybiy5we2vp3f" CACHE PATH "")

set(caliper_DIR "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/caliper-master-ilpr5ougvnd4lxc5kj63jldcw2abkmro/share/cmake/caliper/" CACHE PATH "")

set(adiak_DIR "/g/g19/gupta23/dane/benchpark/XSBenching/spack/opt/spack/linux-sapphirerapids/adiak-0.5.0-wf75jf2wsnfkvwnnk7io7peojnjrbdv3/lib/cmake/adiak/" CACHE PATH "")

#------------------------------------------------------------------------------
# Build Options
#------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")

set(RAJA_RANGE_ALIGN "4" CACHE STRING "")

set(RAJA_RANGE_MIN_LENGTH "32" CACHE STRING "")

set(RAJA_DATA_ALIGN "64" CACHE STRING "")

set(RAJA_HOST_CONFIG_LOADED ON CACHE BOOL "")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")

set(ENABLE_OPENMP OFF CACHE BOOL "")

set(RAJA_ENABLE_OPENMP_TASK OFF CACHE BOOL "")

set(RAJA_ENABLE_SYCL OFF CACHE BOOL "")

set(BLT_CXX_STD "c++17" CACHE STRING "")

set(ENABLE_BENCHMARKS OFF CACHE BOOL "")

set(ENABLE_TESTS ON CACHE BOOL "")

set(RAJA_PERFSUITE_USE_CALIPER ON CACHE BOOL "")


