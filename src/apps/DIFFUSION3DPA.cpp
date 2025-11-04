//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DIFFUSION3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


DIFFUSION3DPA::DIFFUSION3DPA(const RunParams& params)
  : KernelBase(rajaperf::Apps_DIFFUSION3DPA, params)
{
  Index_type NE_default = 15625;
  setDefaultProblemSize(NE_default*diff::D1D*diff::D1D*diff::D1D);
  setDefaultReps(50);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::normal);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(3);
  setProblemDimensionality(3);

  setUsesFeature(Launch);

  addVariantTunings();
}

void DIFFUSION3DPA::setSize(Index_type target_size, Index_type target_reps)
{
  m_NE = std::max((target_size + (diff::D1D*diff::D1D*diff::D1D)/2) / (diff::D1D*diff::D1D*diff::D1D), Index_type(1));

  setActualProblemSize( m_NE*diff::D1D*diff::D1D*diff::D1D );
  setRunReps( target_reps );

  setItsPerRep( m_NE*diff::D1D*diff::D1D*diff::D1D );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 2*sizeof(Real_type) * diff::Q1D*diff::D1D + // b, g
               diff::DPA_SYM*sizeof(Real_type) * diff::Q1D*diff::Q1D*diff::Q1D*m_NE + // d
                           2*sizeof(Real_type) * diff::D1D*diff::D1D*diff::D1D*m_NE ); // x, y
  setBytesReadPerRep( 2*sizeof(Real_type) * diff::Q1D*diff::D1D + // b, g
                      1*sizeof(Real_type) * diff::D1D*diff::D1D*diff::D1D*m_NE + // x
          diff::DPA_SYM*sizeof(Real_type) * diff::Q1D*diff::Q1D*diff::Q1D*m_NE ); // d
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * diff::D1D*diff::D1D*diff::D1D*m_NE ); // y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (4 * diff::D1D * diff::D1D * diff::D1D + //DIFFUSION3DPA_3
                         6 * diff::D1D * diff::Q1D * diff::Q1D + //DIFFUSION3DPA_4
                         (6 * diff::D1D  + 15) * diff::Q1D * diff::Q1D * diff::Q1D + //DIFFUSION3DPA_5
                         (6 * diff::Q1D) * diff::D1D * diff::Q1D * diff::Q1D + //DIFFUSION3DPA_7
                         (6 * diff::Q1D) * diff::D1D * diff::D1D * diff::Q1D + //DIFFUSION3DPA_8
                         (6 * diff::Q1D + 1)*diff::D1D*diff::D1D*diff::D1D)); //DIFFUSION3DPA_9
}

DIFFUSION3DPA::~DIFFUSION3DPA()
{
}

void DIFFUSION3DPA::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B, diff::Q1D*diff::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_G, diff::Q1D*diff::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, diff::Q1D*diff::Q1D*diff::Q1D*diff::DPA_SYM*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X, diff::D1D*diff::D1D*diff::D1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, diff::D1D*diff::D1D*diff::D1D*m_NE, Real_type(0.0), vid);
}

void DIFFUSION3DPA::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Y, diff::D1D*diff::D1D*diff::D1D*m_NE, vid);
}

void DIFFUSION3DPA::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_G, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void DIFFUSION3DPA::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    DIFFUSION3DPA_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type e = 0; e < NE; ++e)) {
        RAJAPERF_COUNTERS_TEAM_CONTEXT();

        RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_0_CPU);

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz, z, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx, x, diff::D1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_1);
            }
          }
        }

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx, x, diff::Q1D)) {
            RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_2);
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz, z, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx, x, diff::Q1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_3);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz, z, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy, y, diff::Q1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx, x, diff::Q1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_4);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qz, z, diff::Q1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy, y, diff::Q1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx, x, diff::Q1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_5);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx, x, diff::Q1D)) {
            RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_6);
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qz, z, diff::Q1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy, y, diff::Q1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx, x, diff::D1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_7);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qz, z, diff::Q1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx, x, diff::D1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_8);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz, z, diff::D1D)) {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy, y, diff::D1D)) {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx, x, diff::D1D)) {
              RAJAPERF_COUNTERS_LOOP_BODY(DIFFUSION3DPA_9);
            }
          }
        }

      } // element loop

    }

  }

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  tearDown(vid, tune_idx);
  );

  RAJAPERF_COUNTERS_FINALIZE();
}

} // end namespace apps
} // end namespace rajaperf
