//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "CONVECTION3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

#include <algorithm>

namespace rajaperf
{
namespace apps
{


CONVECTION3DPA::CONVECTION3DPA(const RunParams& params)
  : KernelBase(rajaperf::Apps_CONVECTION3DPA, params)
{
  Index_type NE_default = 15625;
  setDefaultProblemSize(NE_default*conv::D1D*conv::D1D*conv::D1D);
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

void CONVECTION3DPA::setSize(Index_type target_size, Index_type target_reps)
{
  //Define problem size in terms of DOFS
  m_NE = std::max((target_size + (conv::D1D*conv::D1D*conv::D1D)/2) / (conv::D1D*conv::D1D*conv::D1D), Index_type(1));

  setActualProblemSize( m_NE*conv::D1D*conv::D1D*conv::D1D );
  setRunReps( target_reps );

  setItsPerRep( m_NE*conv::D1D*conv::D1D*conv::D1D );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 3*sizeof(Real_type) * (conv::Q1D*conv::D1D) + // b, bt, g
                  conv::VDIM*sizeof(Real_type) * (conv::Q1D*conv::Q1D*conv::Q1D*m_NE) + // d
                           2*sizeof(Real_type) * (conv::D1D*conv::D1D*conv::D1D*m_NE) ); // x, y
  setBytesReadPerRep( 3*sizeof(Real_type) * conv::Q1D*conv::D1D + // b, bt, g
                      1*sizeof(Real_type) * conv::D1D*conv::D1D*conv::D1D*m_NE + // x
               conv::VDIM*sizeof(Real_type) * conv::Q1D*conv::Q1D*conv::Q1D*m_NE ); // d
  setBytesWrittenPerRep( 0 );
  setBytesModifyWrittenPerRep( 1*sizeof(Real_type) * conv::D1D*conv::D1D*conv::D1D*m_NE ); // y
  setBytesAtomicModifyWrittenPerRep( 0 );

  setFLOPsPerRep(m_NE * (
                         4 * conv::D1D * conv::Q1D * conv::D1D * conv::D1D + //2
                         6 * conv::D1D * conv::Q1D * conv::Q1D * conv::D1D + //3
                         6 * conv::D1D * conv::Q1D * conv::Q1D * conv::Q1D + //4
                         5 * conv::Q1D * conv::Q1D * conv::Q1D +  // 5
                         2 * conv::Q1D * conv::D1D * conv::Q1D * conv::Q1D + // 6
                         2 * conv::Q1D * conv::D1D * conv::Q1D * conv::D1D + // 7
                         (1 + 2*conv::Q1D) * conv::D1D * conv::D1D * conv::D1D // 8
                         ));
}

CONVECTION3DPA::~CONVECTION3DPA()
{
}

void CONVECTION3DPA::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{

  allocAndInitDataConst(m_B,  conv::Q1D*conv::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_Bt, conv::Q1D*conv::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_G, conv::Q1D*conv::D1D, Real_type(1.0), vid);
  allocAndInitDataConst(m_D, conv::Q1D*conv::Q1D*conv::Q1D*conv::VDIM*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_X, conv::D1D*conv::D1D*conv::D1D*m_NE, Real_type(1.0), vid);
  allocAndInitDataConst(m_Y, conv::D1D*conv::D1D*conv::D1D*m_NE, Real_type(0.0), vid);
}

void CONVECTION3DPA::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_Y, conv::D1D*conv::D1D*conv::D1D*m_NE, vid);
}

void CONVECTION3DPA::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_B, vid);
  deallocData(m_Bt, vid);
  deallocData(m_G, vid);
  deallocData(m_D, vid);
  deallocData(m_X, vid);
  deallocData(m_Y, vid);
}


// Only define setCountedAttributes functions past this point
// BEWARE: data types (Index_type, Real_ptr, etc) become wrappers past this point
#include "common/CountingMacros.hpp"

void CONVECTION3DPA::setCountedAttributes()
{
  VariantID vid = VariantID::Base_Seq;
  size_t tune_idx = 0;

  RAJAPERF_COUNTERS_INITIALIZE();

  RAJAPERF_COUNTERS_CODE_WRAPPER(
  setUp(vid, tune_idx);
  );

  {
    RAJAPERF_COUNTERS_CODE_WRAPPER(
    CONVECTION3DPA_DATA_SETUP;
    );

    RAJAPERF_COUNTERS_REP_SCOPE()
    {

      RAJAPERF_COUNTERS_PAR_LOOP(for (Index_type e = 0; e < NE; ++e)) {
        RAJAPERF_COUNTERS_TEAM_CONTEXT();

        RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_0_CPU);

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy,y,conv::D1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx,x,conv::D1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_1);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy,y,conv::D1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_2);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy,y,conv::Q1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_3);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy,y,conv::Q1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qz,z,conv::Q1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_4);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qz,z,conv::Q1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy,y,conv::Q1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_5);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qy,y,conv::Q1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_6);
            }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
        {
           RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(qx,x,conv::Q1D))
           {
              RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy,y,conv::D1D))
              {
                RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_7);
             }
          }
        }

        RAJAPERF_COUNTERS_TEAM_SYNC();

        RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dz,z,conv::D1D))
        {
          RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dy,y,conv::D1D))
          {
            RAJAPERF_COUNTERS_PAR_LOOP(CPU_FOREACH(dx,x,conv::D1D))
            {
              RAJAPERF_COUNTERS_LOOP_BODY(CONVECTION3DPA_8);
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
