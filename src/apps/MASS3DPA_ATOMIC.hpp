//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Action of 3D mass matrix via partial assembly
///
///
/// for (int e = 0; e < NE; ++e) {
///
/// constexpr int MQ1 = mpa3d_at::Q1D;
/// constexpr int MD1 = mpa3d_at::D1D;
///
/// constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;
///
///  double sm_B[MQ1][MD1];
///  double sm_Bt[MD1][MQ1];
///
///  double sm0[MDQ * MDQ * MDQ];
///  double sm1[MDQ * MDQ * MDQ];
///  double(*sm_X)[MD1][MD1]   = (double(*)[MD1][MD1])sm0;
///  double(*DDQ)[MD1][MQ1]    = (double(*)[MD1][MQ1])sm1;
///  double(*DQQ)[MQ1][MQ1]    = (double(*)[MQ1][MQ1])sm0;
///  double(*QQQ)[MQ1][MQ1]    = (double(*)[MQ1][MQ1])sm1;
///  double(*QQD)[MQ1][MD1]    = (double(*)[MQ1][MD1])sm0;
///  double(*QDD)[MD1][MD1]    = (double(*)[MD1][MD1])sm1;
///
///   int thread_dofs[MD1 * MD1 * MD1];
///
///   for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
///     for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
///       for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
///         int j          = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);
///         //missing dof_map for lexicographical ordering
///         thread_dofs[j] = m_elemToDoF[j + mpa3d_at::D1D * mpa3d_at::D1D *
///         mpa3d_at::D1D * e]; sm_X[dz][dy][dx]  = X[thread_dofs[j]];
///       }
///     }
///   }
///
///   for(int d=0; d<mpa3d_at::D1D; ++d) {
///     for(int q=0; q<mpa3d_at::Q1D; ++q) {
///       sm_B[q][d]  = MPAT_B(q, d);
///       sm_Bt[d][q] = sm_B[q][d];
///     }
///   }
///
///
///   for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
///     for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
///       for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {
///         double u = 0.0;
///         for (int dx = 0; dx < mpa3d_at::D1D; ++dx)
///         {
///           u += sm_X[dz][dy][dx] * sm_B[qx][dx];
///         }
///         DDQ[dz][dy][qx] = u;
///       }
///     }
///   }
///
///   for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
///     for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
///       for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {
///
///         double u = 0.0;
///         for (int dy = 0; dy < mpa3d_at::D1D; ++dy)
///         {
///           u += DDQ[dz][dy][qx] * sm_B[qy][dy];
///         }
///         DQQ[dz][qy][qx] = u;
///       }
///     }
///   }
///
///   for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
///     for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
///       for(int qx=0; qx<mpa3d_at::Q1D; ++qx) {
///         double u = 0.0;
///         for (int dz = 0; dz < mpa3d_at::D1D; ++dz)
///         {
///           u += DQQ[dz][qy][qx] * sm_B[qz][dz];
///         }
///         QQQ[qz][qy][qx] = u * MPAT_D(qx, qy, qz, e);
///       }
///     }
///   }
///
///   for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
///     for(int qy=0; qy<mpa3d_at::Q1D; ++qy) {
///       for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
///         double u = 0.0;
///         for (int qx = 0; qx < mpa3d_at::Q1D; ++qx)
///         {
///           u += QQQ[qz][qy][qx] * sm_Bt[dx][qx];
///         }
///         QQD[qz][qy][dx] = u;
///         }
///       }
///     }
///
///       for(int qz=0; qz<mpa3d_at::Q1D; ++qz) {
///         for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
///           for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
///             double u = 0.0;
///             for (int qy = 0; qy<mpa3d_at::Q1D; ++qy)
///             {
///             u += QQD[qz][qy][dx] * sm_Bt[dy][qy];
///             }
///             QDD[qz][dy][dx] = u;
///           }
///         }
///       }
///
///       for(int dz=0; dz<mpa3d_at::D1D; ++dz) {
///         for(int dy=0; dy<mpa3d_at::D1D; ++dy) {
///           for(int dx=0; dx<mpa3d_at::D1D; ++dx) {
///             double u = 0.0;
///             for (int qz = 0; qz < mpa3d_at::Q1D; ++qz)
///             {
///                u += QDD[qz][dy][dx] * sm_Bt[dz][qz];
///             }
///             const int j = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);
///             Y[thread_dofs[j]] += u; //atomic add
///           }
///         }
///       }
///
/// } // element loop
///

#ifndef RAJAPerf_Apps_MASS3DPA_ATOMIC_HPP
#define RAJAPerf_Apps_MASS3DPA_ATOMIC_HPP

#define MASS3DPA_ATOMIC_DATA_SETUP                                             \
  Real_ptr B = m_B;                                                            \
  Real_ptr Bt = m_Bt;                                                          \
  Real_ptr D = m_D;                                                            \
  Real_ptr X = m_X;                                                            \
  Real_ptr Y = m_Y;                                                            \
  Index_ptr ElemToDoF = m_ElemToDoF;                                           \
  Index_type NE = m_NE;

#include "FEM_MACROS.hpp"
#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

// Number of Dofs/Qpts in 1D
namespace mpa3d_at {
constexpr int D1D = 3;
constexpr int Q1D = 4;
} // namespace mpa3d_at

#define MPAT_B(x, y) B[x + mpa3d_at::Q1D * y]
#define MPAT_Bt(x, y) Bt[x + mpa3d_at::D1D * y]
#define MPAT_D(qx, qy, qz, e)                                                  \
  D[qx + mpa3d_at::Q1D * qy + mpa3d_at::Q1D * mpa3d_at::Q1D * qz +             \
    mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D * e]

#define MASS3DPA_ATOMIC_0_CPU                                                  \
  constexpr int MQ1 = mpa3d_at::Q1D;                                           \
  constexpr int MD1 = mpa3d_at::D1D;                                           \
  constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;                                 \
  double sm_B[MQ1][MD1];                                                       \
  double sm_Bt[MD1][MQ1];                                                      \
  double sm0[MDQ * MDQ * MDQ];                                                 \
  double sm1[MDQ * MDQ * MDQ];                                                 \
  double (*sm_X)[MD1][MD1] = (double (*)[MD1][MD1])sm0;                        \
  double (*DDQ)[MD1][MQ1] = (double (*)[MD1][MQ1])sm1;                         \
  double (*DQQ)[MQ1][MQ1] = (double (*)[MQ1][MQ1])sm0;                         \
  double (*QQQ)[MQ1][MQ1] = (double (*)[MQ1][MQ1])sm1;                         \
  double (*QQD)[MQ1][MD1] = (double (*)[MQ1][MD1])sm0;                         \
  double (*QDD)[MD1][MD1] = (double (*)[MD1][MD1])sm1;                         \
  int thread_dofs[MD1 * MD1 * MD1];

#define MASS3DPA_ATOMIC_0_GPU                                                  \
  constexpr int MQ1 = mpa3d_at::Q1D;                                           \
  constexpr int MD1 = mpa3d_at::D1D;                                           \
  constexpr int MDQ = (MQ1 > MD1) ? MQ1 : MD1;                                 \
  RAJA_TEAM_SHARED double sm_B[MQ1][MD1];                                      \
  RAJA_TEAM_SHARED double sm_Bt[MD1][MQ1];                                     \
  RAJA_TEAM_SHARED double sm0[MDQ * MDQ * MDQ];                                \
  RAJA_TEAM_SHARED double sm1[MDQ * MDQ * MDQ];                                \
  double (*sm_X)[MD1][MD1] = (double (*)[MD1][MD1])sm0;                        \
  double (*DDQ)[MD1][MQ1] = (double (*)[MD1][MQ1])sm1;                         \
  double (*DQQ)[MQ1][MQ1] = (double (*)[MQ1][MQ1])sm0;                         \
  double (*QQQ)[MQ1][MQ1] = (double (*)[MQ1][MQ1])sm1;                         \
  double (*QQD)[MQ1][MD1] = (double (*)[MQ1][MD1])sm0;                         \
  double (*QDD)[MD1][MD1] = (double (*)[MD1][MD1])sm1;                         \
  RAJA_TEAM_SHARED int thread_dofs[MD1 * MD1 * MD1];

#define MASS3DPA_ATOMIC_1                                                      \
  int j = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);                      \
  thread_dofs[j] =                                                             \
      ElemToDoF[j + mpa3d_at::D1D * mpa3d_at::D1D * mpa3d_at::D1D * e];        \
  sm_X[dz][dy][dx] =                                                           \
      X[thread_dofs[j]]; // missing dof_map for lexicographical ordering

#define MASS3DPA_ATOMIC_2                                                      \
  sm_B[q][d] = MPAT_B(q, d);                                                   \
  sm_Bt[d][q] = sm_B[q][d];

#define MASS3DPA_ATOMIC_3                                                      \
  double u = 0.0;                                                              \
  for (int dx = 0; dx < mpa3d_at::D1D; ++dx) {                                 \
    u += sm_X[dz][dy][dx] * sm_B[qx][dx];                                      \
  }                                                                            \
  DDQ[dz][dy][qx] = u;

#define MASS3DPA_ATOMIC_4                                                      \
  double u = 0.0;                                                              \
  for (int dy = 0; dy < mpa3d_at::D1D; ++dy) {                                 \
    u += DDQ[dz][dy][qx] * sm_B[qy][dy];                                       \
  }                                                                            \
  DQQ[dz][qy][qx] = u;

#define MASS3DPA_ATOMIC_5                                                      \
  double u = 0.0;                                                              \
  for (int dz = 0; dz < mpa3d_at::D1D; ++dz) {                                 \
    u += DQQ[dz][qy][qx] * sm_B[qz][dz];                                       \
  }                                                                            \
  QQQ[qz][qy][qx] = u * MPAT_D(qx, qy, qz, e);

#define MASS3DPA_ATOMIC_6                                                      \
  double u = 0.0;                                                              \
  for (int qx = 0; qx < mpa3d_at::Q1D; ++qx) {                                 \
    u += QQQ[qz][qy][qx] * sm_Bt[dx][qx];                                      \
  }                                                                            \
  QQD[qz][qy][dx] = u;

#define MASS3DPA_ATOMIC_7                                                      \
  double u = 0.0;                                                              \
  for (int qy = 0; qy < mpa3d_at::Q1D; ++qy) {                                 \
    u += QQD[qz][qy][dx] * sm_Bt[dy][qy];                                      \
  }                                                                            \
  QDD[qz][dy][dx] = u;

#define MASS3DPA_ATOMIC_8                                                      \
  double u = 0.0;                                                              \
  for (int qz = 0; qz < mpa3d_at::Q1D; ++qz) {                                 \
    u += QDD[qz][dy][dx] * sm_Bt[dz][qz];                                      \
  }                                                                            \
  const int j = dx + mpa3d_at::D1D * (dy + dz * mpa3d_at::D1D);                \
  RAJA::atomicAdd<RAJA::auto_atomic>(&Y[thread_dofs[j]], u); // atomic add

namespace rajaperf {
class RunParams;

namespace apps {

// Helper function to get global node ID for structured 3D grid
inline int nodeID(int ix, int iy, int iz, int num_nodes_x, int num_nodes_y,
                  int num_nodes_z) {
  return ix + num_nodes_x * (iy + num_nodes_y * iz);
}

/**
 * Build element-to-DOF connectivity for a structured 3D hex mesh
 * with arbitrary polynomial order p and 1 DOF per node.
 *
 * Inputs:
 *   Nx, Ny, Nz    : number of elements in x, y, z directions
 *   p             : polynomial order (>=1)
 *
 * Outputs:
 *   elem_to_dofs  : size = num_elems
 *                   each entry is a vector of size (p+1)^3
 *                   containing the global DOF indices of that element
 *
 * Element numbering:
 *   elem_id = ex + Nx * (ey + Ny * ez)
 */
inline void
buildElemToDofTable(int Nx, int Ny, int Nz, int p,
                    Index_ptr elemToDof) // output buffer, must be preallocated
{
  const int num_nodes_x = Nx * p + 1;
  const int num_nodes_y = Ny * p + 1;
  const int num_nodes_z = Nz * p + 1;

  const int ndof_per_elem = (p + 1) * (p + 1) * (p + 1);

  // Loop over elements
  for (int ez = 0; ez < Nz; ++ez) {
    for (int ey = 0; ey < Ny; ++ey) {
      for (int ex = 0; ex < Nx; ++ex) {
        // Global element index (row in elemToDof)
        int e = ex + Nx * (ey + Ny * ez);

        // Pointer to start of this element's DOF list
        Index_ptr row = elemToDof + e * ndof_per_elem;

        int local = 0;

        // Loop over local nodes of the element
        for (int kz = 0; kz <= p; ++kz) {
          int iz = ez * p + kz;
          for (int ky = 0; ky <= p; ++ky) {
            int iy = ey * p + ky;
            for (int kx = 0; kx <= p; ++kx) {
              int ix = ex * p + kx;

              int nodeID = ix + num_nodes_x * (iy + num_nodes_y * iz);

              // Scalar DOF per node, so dofID == nodeID
              int dofID = nodeID;

              row[local++] = dofID;
            }
          }
        }
      }
    }
  }
}

class MASS3DPA_ATOMIC : public KernelBase {
public:
  MASS3DPA_ATOMIC(const RunParams &params);

  ~MASS3DPA_ATOMIC();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();
  void defineSyclVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);

  template <size_t block_size> void runCudaVariantImpl(VariantID vid);
  template <size_t block_size> void runHipVariantImpl(VariantID vid);
  template <size_t work_group_size> void runSyclVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = mpa3d_at::Q1D * mpa3d_at::Q1D * mpa3d_at::Q1D;
  using gpu_block_sizes_type = integer::list_type<default_gpu_block_size>;

  Real_ptr m_B;
  Real_ptr m_Bt;
  Real_ptr m_D;
  Real_ptr m_X;
  Real_ptr m_Y;

  Index_type m_Nx;       // zones in x dimension
  Index_type m_Ny;       // zones in y dimension
  Index_type m_Nz;       // zones in z dimension
  Index_type m_P;        // polynomial order
  Index_type m_Tot_Dofs; // total number of dofs

  Index_ptr m_ElemToDoF;

  Index_type m_NE;
  Index_type m_NE_default;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
