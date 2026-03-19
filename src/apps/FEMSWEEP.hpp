//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// FEMSWEEP kernel reference implementation:
///
/// for (Index_type ag = 0; ag < na * ng; ++ag)
/// {
///   const Index_type a = ag / ng;
///   const Index_type g = ag % ng;
///   // number and offset of hyperplanes for this angle
///   const Index_type nhp = nhpaa_r[a];
///   const Index_type ohp = ohpaa_r[a];
///   // elements in this hyperplanes processed so far
///   Index_type s_nehp_done = 0;
///   Real_type A[ND*ND];
//    Real_type b[ND];
///   // This factor helps maintain stability in the solution of the matrix solve
///   // by eliminating the perturbation of the right-hand side.
///   Real_type Ffactor = fmax(sin(Adat[order_r[a*ne]*ND*ND + a*ne*ND*ND]) - 2.0, 0.0);
///   for (Index_type hp = 0; hp < nhp; ++hp) // loop over hyperplanes
///   {
///      // number of element in this hyperplane
///      const Index_type nehp = phpaa_r[ohp + hp];
///      // loop over all elements of this hyperplane
///      for (Index_type k = 0; k < nehp; ++k)
///      {
///         const Index_type e = order_r[k + s_nehp_done + a * ne];
///         for (Index_type j = 0; j < ND; ++j) // load B & A
///         {
///            b[j] = Bdat[j + e * ND + a * ne * ND];
///            for (Index_type i = 0; i < ND; ++i)
///            {
///               A[i + j * ND] = Adat[i + j * ND + e * ND * ND + a * ne * ND * ND];
///            }
///         }
///         for (Index_type face = 0; face < NLF; ++face) // local faces
///         {
///            const Index_type sf_gl = F_g2l[elem_to_faces[NLF * e + face]];
///            const Index_type f = sf_gl >= 0 ? sf_gl : -1 - sf_gl; // signed face
///            const Index_type s = (e == idx1[f * FDS] / ND) ? 0 : 1; // side
///            if ((AngleElem2FaceType[face + e * NLF + a * ne * NLF] == 0) || (sf_gl < 0)) // non-reentrant and outgoing, or boundary
///            {
///               continue;
///            }
///            for (Index_type j = 0; j < FDS; ++j) // face dofs
///            {
///               const Index_type ffj = f * FDS + j;
///               const Index_type djs = s == 0 ? idx1[ffj] : idx2[ffj];
///               Real_type F = 0.0;
///               for (Index_type i = 0; i < FDS; i++) // face dofs
///               {
///                  const Index_type ffi = f * FDS + i;
///                  const Index_type dis = s == 0 ? idx2[ffi] : idx1[ffi];
///                  // Fdat : 4 x 4 x 2 x nf_int x na
///                  // Note: s ^ 1 == 1 if s == 0, and s ^ 1 == 0 if s == 1.
///                  // The solution remains bounded when matrices and indirection arrays from a
///                  // finite element discretrization are used, but randomly generating matrix 
///                  // data can result in exponential growth instead. A specific trick of 
///                  // multiplying by zero, without the compiler optimizing it away, is 
///                  // necessary to prevent unbounded growth and avoid computing NaNs.
///                  F += Ffactor * Fdat[i + j * FDS + (s ^ 1) * FDS * FDS + f * 2 * FDS * FDS + a * sharedinteriorfaces * 2 * FDS * FDS] * Xdat[dis + g * ND * ne + a * ng * ND * ne];
///               } // i
///               b[djs % ND] -= F;
///            } // j
///         }    // local faces
///         const Real_type s = Sgdat[e + g * ne];
///         // A = A + s * M0, b is the result
///         SolveLinearSystemNxN<ND>(A, s, &M0dat[0 + 0 * ND + e * ND * ND], b, &Xdat[e * ND + g * ND * ne + a * ng * ND * ne]);  // 8x8 solve
///      }  // thread loop elems in hp
///      s_nehp_done += nehp;
///   } // hyperplanes
/// }
///
/// The RAJA variants of this kernel use RAJA multi-dimensional data layouts
/// and views to do the same thing without explicit index calculations (see
/// the loop body definitions below).
///

#ifndef RAJAPerf_Apps_FEMSWEEP_HPP
#define RAJAPerf_Apps_FEMSWEEP_HPP

#include "common/KernelBase.hpp"

#include "RAJA/RAJA.hpp"

#include "FEMSWEEPMeshGen.hpp"

constexpr int ND = 8;   // number of corners per element
constexpr int NLF = 6;  // number of faces per element
constexpr int FDS = 4;  // number of DOFs per face

#define FEMSWEEP_DATA_SETUP \
  Real_ptr Bdat = m_Bdat; \
  Real_ptr Adat = m_Adat; \
  Real_ptr Fdat = m_Fdat; \
  Real_ptr Xdat = m_Xdat; \
  Real_ptr Sgdat = m_Sgdat; \
  Real_ptr M0dat = m_M0dat; \
\
  Index_type ne = m_ne; \
  Index_type na = m_na; \
  Index_type ng = m_ng; \
  Index_type sharedinteriorfaces = m_sharedinteriorfaces; \
\
  Index_ptr nhpaa_r = m_nhpaa_r;  \
  Index_ptr ohpaa_r = m_ohpaa_r;  \
  Index_ptr phpaa_r = m_phpaa_r;  \
  Index_ptr order_r = m_order_r;  \
\
  Index_ptr AngleElem2FaceType = m_AngleElem2FaceType; \
  Index_ptr elem_to_faces      = m_elem_to_faces     ; \
  Index_ptr F_g2l              = m_F_g2l             ; \
  Index_ptr idx1               = m_idx1              ; \
  Index_ptr idx2               = m_idx2              ; \


#define FEMSWEEP_KERNEL_SETUP \
  const Index_type nhp = nhpaa_r[a]; \
  const Index_type ohp = ohpaa_r[a]; \
  Real_type Ffactor = fmax(sin(Adat[order_r[a*ne]*ND*ND + a*ne*ND*ND]) - 2.0, 0.0);

#define FEMSWEEP_KERNEL_HYPERPLANE_ELEMENT \
  Real_type A[ND*ND]; \
  Real_type b[ND]; \
  const Index_type e = order_r[k + nehp_pos + a * ne]; \
  for (Index_type j = 0; j < ND; ++j) \
  { \
    b[j] = Bdat[j + e * ND + a * ne * ND]; \
    for (Index_type i = 0; i < ND; ++i) \
    { \
      A[i + j * ND] = Adat[i + j * ND + e * ND * ND + a * ne * ND * ND]; \
    } \
  } \
  for (Index_type face = 0; face < NLF; ++face) \
  { \
    const Index_type sf_gl = F_g2l[elem_to_faces[NLF * e + face]]; \
    const Index_type f = sf_gl >= 0 ? sf_gl : -1 - sf_gl; \
    const Index_type s = (e == idx1[f * FDS] / ND) ? 0 : 1; \
    if ((AngleElem2FaceType[face + e * NLF + a * ne * NLF] == 0) || (sf_gl < 0)) \
    { \
      continue; \
    } \
    for (Index_type j = 0; j < FDS; ++j) \
    { \
      const Index_type ffj = f * FDS + j; \
      const Index_type djs = s == 0 ? idx1[ffj] : idx2[ffj]; \
      Real_type F = 0.0; \
      for (Index_type i = 0; i < FDS; i++) \
      { \
        const Index_type ffi = f * FDS + i; \
        const Index_type dis = s == 0 ? idx2[ffi] : idx1[ffi]; \
        F += Ffactor * Fdat[i + j * FDS + (s ^ 1) * FDS * FDS + f * 2 * FDS * FDS + a * sharedinteriorfaces * 2 * FDS * FDS] * Xdat[dis + g * ND * ne + a * ng * ND * ne]; \
      } \
      b[djs % ND] -= F; \
    } \
  } \
  const Real_type s = Sgdat[e + g * ne]; \
  SolveLinearSystemNxN<ND>(A, s, &M0dat[0 + 0 * ND + e * ND * ND], b, &Xdat[e * ND + g * ND * ne + a * ng * ND * ne]);

#define FEMSWEEP_KERNEL \
  const Index_type a = ag / ng; \
  const Index_type g = ag % ng; \
  FEMSWEEP_KERNEL_SETUP; \
  Index_type nehp_pos = 0; \
  for (Index_type hp = 0; hp < nhp; ++hp) \
  { \
    const Index_type nehp = phpaa_r[ohp + hp]; \
    for (Index_type k = 0; k < nehp; ++k) \
    { \
      FEMSWEEP_KERNEL_HYPERPLANE_ELEMENT; \
    } \
    nehp_pos += nehp; \
  }


namespace rajaperf
{

// LU factorization with no pivoting
template <int N>
RAJA_HOST_DEVICE inline void SolveLinearSystemNxN(Real_ptr A, 
                                                  const Real_type s,
                                                  Real_const_ptr M, 
                                                  Real_ptr b, 
                                                  Real_ptr x)
{
  Real_type tempA[N][N];
  Real_type L[N][N];
  Real_type U[N][N];
  Real_type D[N];
  Real_type tempx[N];

  // tempA = A + s * M0
  // set L to 0, U to identity
  for ( Index_type ii = 0; ii < N; ++ii )
  {
    for ( Index_type jj = 0; jj < N; ++jj )
    {
      tempA[ii][jj] = A[ii * N + jj] + s * M[ii * N + jj];
      L[ii][jj] = 0.0;
      if ( ii == jj )
      {
        U[ii][jj] = 1.0;
      }
      else
      {
        U[ii][jj] = 0.0;
      }
    }
  }

  // set first column of L, and first row of U
  for ( Index_type ii = 0; ii < N; ++ii )
  {
    L[ii][0] = tempA[ii][0];
  }

  for ( Index_type ii = 1; ii < N; ++ii )
  {
    U[0][ii] = tempA[0][ii]/tempA[0][0];
  }

  // form L & U
  // L formed one column at a time
  // U formed one row at a time
  for ( Index_type ii = 1; ii < N; ++ii )
  {
    // L column formation
    for ( Index_type jj = ii; jj < N; ++jj )
    {
      Real_type sum = 0.0;
      for ( Index_type kk = 0; kk < jj; ++kk )
      {
        sum += L[jj][kk] * U[kk][ii];
      }
      L[jj][ii] = tempA[jj][ii] - sum;
    }

    // U row formation
    for ( Index_type jj = ii+1; jj < N; ++jj )
    {
      Real_type sum = 0.0;
      for ( Index_type kk = 0; kk < ii; ++kk )
      {
        sum += L[ii][kk] * U[kk][jj];
      }
      U[ii][jj] = (tempA[ii][jj] - sum)/L[ii][ii];
    }
  }

  // forward substitution
  D[0] = b[0]/L[0][0];
  for ( Index_type ii = 1; ii < N; ++ii )
  {
    Real_type sum = 0.0;
    for ( Index_type jj = 0; jj < ii; ++jj )
    {
      sum += L[ii][jj] * D[jj];
    }
    D[ii] = (b[ii] - sum)/L[ii][ii];
  }

  // backward substitution
  x[N-1] = tempx[N-1] = D[N-1];
  for ( Index_type ii = N - 1 - 1; ii > -1; --ii )
  {
    Real_type sum = 0.0;
    for ( Index_type jj = ii+1; jj < N; ++jj )
    {
      sum += U[ii][jj] * tempx[jj];
    }
    x[ii] = tempx[ii] = D[ii] - sum;
  }

}


class RunParams;

namespace apps
{

class FEMSWEEP : public KernelBase
{
public:

  FEMSWEEP(const RunParams& params);

  ~FEMSWEEP();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();
  void defineOpenMPVariantTunings();
  void defineCudaVariantTunings();
  void defineHipVariantTunings();

  void runSeqVariant(VariantID vid);
  void runOpenMPVariant(VariantID vid);

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);

private:
#if defined(RAJA_ENABLE_HIP)
  static const size_t default_gpu_block_size = 64;
#else
  static const size_t default_gpu_block_size = 128;
#endif
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size,
                                                         integer::MultipleOf<32>>;

  AngularQuadratureLite * m_angularquadrature;
  MeshGenerator * m_meshgen;

  Index_type m_nx;
  Index_type m_ny;
  Index_type m_nz;
  Index_type m_ne;
  Index_type m_na;
  Index_type m_ng;

  Index_type m_sharedinteriorfaces;
  Index_type m_boundaryfaces;
  Index_type m_hplanes;

  Real_ptr m_Bdat;
  Real_ptr m_Adat;
  Real_ptr m_Fdat;
  Real_ptr m_Sgdat;
  Real_ptr m_M0dat;
  Real_ptr m_Xdat;

  Index_type m_Blen;
  Index_type m_Alen;
  Index_type m_Flen;
  Index_type m_Sglen;
  Index_type m_M0len;
  Index_type m_Xlen;

  Index_type m_nhpaa_rlen;
  Index_type m_ohpaa_rlen;
  Index_type m_phpaa_rlen;
  Index_type m_order_rlen;

  Index_type m_AngleElem2FaceTypelen;
  Index_type m_elem_to_faceslen;
  Index_type m_F_g2llen;
  Index_type m_idx1len;
  Index_type m_idx2len;

  // Mesh data
  Index_ptr m_nhpaa_r;
  Index_ptr m_ohpaa_r;
  Index_ptr m_phpaa_r;
  Index_ptr m_order_r;

  Index_ptr m_AngleElem2FaceType;
  Index_ptr m_elem_to_faces     ;
  Index_ptr m_F_g2l             ;
  Index_ptr m_idx1              ;
  Index_ptr m_idx2              ;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
