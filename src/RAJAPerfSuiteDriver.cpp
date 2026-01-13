//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#if defined(RUN_KOKKOS)
#include <Kokkos_Core.hpp>
#endif

#include "common/Executor.hpp"

#include <iostream>

#if defined(RAJA_PERFSUITE_ENABLE_MPI)
#include <mpi.h>
#endif

//------------------------------------------------------------------------------
int main( int argc, char** argv )
{
#if defined(RAJA_PERFSUITE_USE_CALIPER)
  // Retrieve the value of CALI_CONFIG
  const char* caliConfigValue = getenv("CALI_CONFIG");
  if (caliConfigValue) {
    // unset CALI_CONFIG and Copy CALI_CONFIG to DISABLED_CALI_CONFIG
    if (unsetenv("CALI_CONFIG") == 0 && setenv("DISABLED_CALI_CONFIG", caliConfigValue, 1) == 0) {
      std::cout << "Configuration options in CALI_CONFIG will be parsed and added to the internal RAJAPerf Caliper config manager." << std::endl;
    } else {
      throw std::runtime_error("main: Failed to update environment variables. Unable to set DISABLED_CALI_CONFIG or unset CALI_CONFIG.");
    }
  }
#endif

#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Init(&argc, &argv);

  int num_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
  rajaperf::getCout() << "\n\nRunning with " << num_ranks << " MPI ranks..." << std::endl;
#endif
#if defined(RUN_KOKKOS)
  Kokkos::initialize(argc, argv);
#endif

  // STEP 1: Create suite executor object
  rajaperf::Executor executor(argc, argv);

  // STEP 2: Assemble kernels and variants to run
  executor.setupSuite();

  // STEP 3: Report suite run summary
  //         (enable users to catch errors before entire suite is run)
  executor.reportRunSummary(rajaperf::getCout());

  // STEP 4: Execute suite
  executor.runSuite();

  // STEP 5: Generate suite execution reports
  executor.outputRunData();

  rajaperf::getCout() << "\n\nDONE!!!...." << std::endl;

#if defined(RUN_KOKKOS)
  Kokkos::finalize();
#endif
#if defined(RAJA_PERFSUITE_ENABLE_MPI)
  MPI_Finalize();
#endif

  return 0;
}
