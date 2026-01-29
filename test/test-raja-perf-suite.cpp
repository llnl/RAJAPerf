//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "gtest/gtest.h"

#if defined(RUN_KOKKOS)
#include <Kokkos_Core.hpp>
#endif

#include "common/Executor.hpp"
#include "common/KernelBase.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#if defined(RAJA_PERFSUITE_ENABLE_MPI)
#include <mpi.h>
#endif

int main( int argc, char** argv )
{
  testing::InitGoogleTest(&argc, argv);

#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Init(&argc, &argv);
#endif
#if defined(RUN_KOKKOS)
  Kokkos::initialize(argc, argv);
#endif

  int res = RUN_ALL_TESTS();

#if defined(RUN_KOKKOS)
  Kokkos::finalize();
#endif
#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Finalize();
#endif

  return res;
}

TEST(ShortSuiteTest, Basic)
{

// Assemble command line args for basic test

  std::vector< std::string > sargv{};
  sargv.emplace_back(std::string("dummy "));  // for executable name
  sargv.emplace_back(std::string("--checkrun"));
#if defined(RUN_RAJAPERF_SHORT_TEST)
  sargv.emplace_back(std::string("1"));
#else
  sargv.emplace_back(std::string("3"));
#endif
  sargv.emplace_back(std::string("--show-progress"));
  sargv.emplace_back(std::string("--disable-warmup"));

#if defined(RAJA_ENABLE_HIP) && \
     (HIP_VERSION_MAJOR < 5 || \
     (HIP_VERSION_MAJOR == 5 && HIP_VERSION_MINOR < 1))
  sargv.emplace_back(std::string("--exclude-kernels"));
  sargv.emplace_back(std::string("HALO_PACKING_FUSED"));
#endif

#if !defined(_WIN32)

#if defined(RAJA_ENABLE_TARGET_OPENMP)
  sargv.emplace_back(std::string("--exclude-kernels"));
  sargv.emplace_back(std::string("Comm"));
  sargv.emplace_back(std::string("EDGE3D"));
  sargv.emplace_back(std::string("MATVEC_3D_STENCIL"));
#else

#if ( (defined(RAJA_COMPILER_CLANG) && __clang_major__ == 11) || \
      defined(RUN_RAJAPERF_SHORT_TEST) )
  sargv.emplace_back(std::string("--exclude-kernels"));
#if (defined(RAJA_COMPILER_CLANG) && __clang_major__ == 11)  
  sargv.emplace_back(std::string("FIRST_MIN"));
#endif
#if defined(RUN_RAJAPERF_SHORT_TEST)
  sargv.emplace_back(std::string("Polybench"));
#endif
#endif

#endif // else

#endif // !defined(_WIN32)


  char *unit_test = getenv("RAJA_PERFSUITE_UNIT_TEST");
  if (unit_test != NULL) {
    sargv.emplace_back(std::string("-k"));
    sargv.emplace_back(std::string(unit_test));
  }

  char** argv = new char* [sargv.size()];
  for (size_t is = 0; is < sargv.size(); ++is) {
    argv[is] = const_cast<char*>(sargv[is].c_str());
  }

  // STEP 1: Create suite executor object with input args defined above
  rajaperf::Executor executor(sargv.size(), argv);

  // STEP 2: Assemble kernels and variants to run
  executor.setupSuite();

  // STEP 3: Report suite run summary
  executor.reportRunSummary(std::cout);

  // STEP 4: Execute suite
  executor.runSuite();

  // STEP 5: Access suite run data and run through checks
  std::vector<rajaperf::KernelBase*> kernels = executor.getKernels();
  std::vector<rajaperf::VariantID> variant_ids = executor.getVariantIDs();


  for (size_t ik = 0; ik < kernels.size(); ++ik) {

    rajaperf::KernelBase* kernel = kernels[ik];

    rajaperf::Checksum_type cksum_tol = kernel->getChecksumTolerance();

    //
    // Check execution time is greater than zero and checksum diff is 
    // within tolerance for each variant run.
    // 
    for (size_t iv = 0; iv < variant_ids.size(); ++iv) {

      rajaperf::VariantID vid = variant_ids[iv];

      size_t num_tunings = kernel->getNumVariantTunings(variant_ids[iv]);
      for (size_t tune_idx = 0; tune_idx < num_tunings; ++tune_idx) {
        if ( kernel->wasVariantTuningRun(vid, tune_idx) ) {

          double rtime = kernel->getTotTime(vid, tune_idx);

          rajaperf::Checksum_type cksum_rel_diff =
              kernel->getChecksumMaxRelativeAbsoluteDifference(vid, tune_idx);

          // Print kernel information when running test manually
          std::cout << "Check kernel, variant, tuning : "
                    << kernel->getName() << " , "
                    << rajaperf::getVariantName(vid) << " , "
                    << kernel->getVariantTuningName(vid, tune_idx) 
                    << std::endl;
          EXPECT_GT(rtime, 0.0);
          EXPECT_LE(cksum_rel_diff, cksum_tol);
          
        }
      } 

    }  // loop over variants

  } // loop over kernels

  // clean up 
  delete [] argv; 
}
