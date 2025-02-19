//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-24, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// FEMSWEEP kernel reference implementation:
///
/// for (int ag = 0; ag < na * ng; ++ag)
/// {
///   const int a = ag / ng, g = ag % ng;
///   // number and offset of hyperplanes for this angle
///   const int nhp = nhpaa_r[a], ohp = ohpaa_r[a];
///   // elements in this hyperplanes processed so far
///   int s_nehp_done = 0;
///   double A[ND * ND], b[ND];
///   for (int hp = 0; hp < nhp; ++hp) // loop over hyperplanes
///   {
///      // number of element in this hyperplane
///      const int nehp = phpaa_r[ohp + hp];
///      // loop over all elements of this hyperplane
///      for (int k = 0; k < nehp; ++k)
///      {
///         const int e = order_r[k + s_nehp_done + a * ne];
///         for (int j = 0; j < ND; ++j) // load B & A
///         {
///            b[j] = Bdat[j + e * ND + a * ne * ND];
///            for (int i = 0; i < ND; ++i)
///            {
///               A[i + j * ND] = Adat[i + j * ND + e * ND * ND + a * ne * ND * ND];
///            }
///         }
///         for (int face = 0; face < NLF; ++face) // local faces
///         {
///            const int sf_gl = F_g2l[elem_to_faces[NLF * e + face]];
///            const int f = sf_gl >= 0 ? sf_gl : -1 - sf_gl; // signed face
///            const int s = (e == idx1[f * FDS] / ND) ? 0 : 1; // side
///            if ((AngleElem2FaceType[face + e * NLF + a * ne * NLF] == 0) || (sf_gl < 0)) // non-reentrant and outgoing, or boundary
///            {
///               continue;
///            }
///            for (int j = 0; j < FDS; ++j) // face dofs
///            {
///               const int ffj = f * FDS + j;
///               const int djs = s == 0 ? idx1[ffj] : idx2[ffj];
///               double F = 0.0;
///               for (int i = 0; i < FDS; i++) // face dofs
///               {
///                  const int ffi = f * FDS + i;
///                  const int dis = s == 0 ? idx2[ffi] : idx1[ffi];
///                  // Fdat : 4 x 4 x 2 x nf_int x na
///                  // Note: s ^ 1 == 1 if s == 0, and s ^ 1 == 0 if s == 1.
///                  // Instability can happen here
///                  F += Fdat[i + j * FDS + (s ^ 1) * FDS * FDS + f * 2 * FDS * FDS + a * 9450 * 2 * FDS * FDS] * Xdat[dis + g * ND * ne + a * ng * ND * ne];
///               } // i
///               Xdat[djs % ND + e * ND + g * ND * ne + a * ng * ND * ne] -= F;
///            } // j
///         }    // local faces
///         const double s = Sgdat[e + g * ne];
///         // A = A + s * M0, b is the result
///         SolveLinearSystem8x8(A, s, &M0dat[0 + 0 * ND + e * ND * ND], b, &Xdat[e * ND + g * ND * ne + a * ng * ND * ne]);  // 8x8 solve
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

#define FEMSWEEP_DATA_SETUP \
  Real_ptr Bdat = m_Bdat; \
  Real_ptr Adat = m_Adat; \
  Real_ptr Fdat = m_Fdat; \
  Real_ptr Xdat = m_Xdat; \
  Real_ptr Xfinaldat = m_Xfinaldat; \
  Real_ptr Sgdat = m_Sgdat; \
  Real_ptr M0dat = m_M0dat; \
\
  Index_type ne = m_ne; \
  Index_type nd = m_nd; \
  Index_type nfds = m_nfds; \
  Index_type na = m_na; \
  Index_type ng = m_ng; \
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
  
#define FEMSWEEP_KERNEL \
  const int a = ag / ng, g = ag % ng; \
  const int nhp = nhpaa_r[a], ohp = ohpaa_r[a]; \
  int s_nehp_done = 0; \
  double A[ND * ND], b[ND]; \
  for (int hp = 0; hp < nhp; ++hp) \
  { \
     const int nehp = phpaa_r[ohp + hp]; \
     for (int k = 0; k < nehp; ++k) \
     { \
        const int e = order_r[k + s_nehp_done + a * ne]; \
        for (int j = 0; j < ND; ++j) \
        { \
           b[j] = Bdat[j + e * ND + a * ne * ND]; \
           for (int i = 0; i < ND; ++i) \
           { \
              A[i + j * ND] = Adat[i + j * ND + e * ND * ND + a * ne * ND * ND]; \
           } \
        } \
        for (int face = 0; face < NLF; ++face) \
        { \
           const int sf_gl = F_g2l[elem_to_faces[NLF * e + face]]; \
           const int f = sf_gl >= 0 ? sf_gl : -1 - sf_gl; \
           const int s = (e == idx1[f * FDS] / ND) ? 0 : 1; \
           if ((AngleElem2FaceType[face + e * NLF + a * ne * NLF] == 0) || (sf_gl < 0)) \
           { \
              continue; \
           } \
           for (int j = 0; j < FDS; ++j) \
           { \
              const int ffj = f * FDS + j; \
              const int djs = s == 0 ? idx1[ffj] : idx2[ffj]; \
              double F = 0.0; \
              for (int i = 0; i < FDS; i++) \
              { \
                 const int ffi = f * FDS + i; \
                 const int dis = s == 0 ? idx2[ffi] : idx1[ffi]; \
                 F += Fdat[i + j * FDS + (s ^ 1) * FDS * FDS + f * 2 * FDS * FDS + a * 9450 * 2 * FDS * FDS] * Xdat[dis + g * ND * ne + a * ng * ND * ne]; \
              } \
              Xdat[djs % ND + e * ND + g * ND * ne + a * ng * ND * ne] -= F; \
           } \
        } \
        const double s = Sgdat[e + g * ne]; \
        SolveLinearSystem8x8(A, s, &M0dat[0 + 0 * ND + e * ND * ND], b, &Xdat[e * ND + g * ND * ne + a * ng * ND * ne]); \
     } \
     s_nehp_done += nehp; \
  } \

constexpr int ND = 8, NLF = 6, FDS = 4;

// LU factorization with no pivoting
static RAJA_HOST_DEVICE inline void SolveLinearSystem8x8(volatile double *A, 
                                                         const double s,
                                                         volatile const double *M, 
                                                         volatile const double *b, 
                                                         double * x)//,
                                                         //double * xfinal)
{
  double tempA[8][8];
  double L[8][8];
  double U[8][8];
  double D[8];

  // tempA = A + s * M0
  // set L to 0, U to identity
  for ( int ii = 0; ii < 8; ++ii )
  {
    for ( int jj = 0; jj < 8; ++jj )
    {
      tempA[ii][jj] = A[ii * 8 + jj] + s * M[ii * 8 + jj];
      L[ii][jj] = 0;
      if ( ii == jj )
      {
        U[ii][jj] = 1;
      }
      else
      {
        U[ii][jj] = 0;
      }
    }
  }

  // set first column of L, and first row of U
  L[0][0] = tempA[0][0];
  L[1][0] = tempA[1][0];
  L[2][0] = tempA[2][0];
  L[3][0] = tempA[3][0];
  L[4][0] = tempA[4][0];
  L[5][0] = tempA[5][0];
  L[6][0] = tempA[6][0];
  L[7][0] = tempA[7][0];

  U[0][1] = tempA[0][1]/tempA[0][0];
  U[0][2] = tempA[0][2]/tempA[0][0];
  U[0][3] = tempA[0][3]/tempA[0][0];
  U[0][4] = tempA[0][4]/tempA[0][0];
  U[0][5] = tempA[0][5]/tempA[0][0];
  U[0][6] = tempA[0][6]/tempA[0][0];
  U[0][7] = tempA[0][7]/tempA[0][0];

  // form L & U
  // L formed one column at a time
  // U formed one row at a time
  for ( int ii = 1; ii < 8; ++ii )
  {
    // L column formation
    for ( int jj = ii; jj < 8; ++jj )
    {
      double sum = 0.0;
      for ( int kk = 0; kk < jj; ++kk )
      {
        sum += L[jj][kk] * U[kk][ii];
      }
      L[jj][ii] = tempA[jj][ii] - sum;
    }

    // U row formation
    for ( int jj = ii+1; jj < 8; ++jj )
    {
      double sum = 0.0;
      for ( int kk = 0; kk < ii; ++kk )
      {
        sum += L[ii][kk] * U[kk][jj];
      }
      U[ii][jj] = (tempA[ii][jj] - sum)/L[ii][ii];
    }
  }

  // forward substitution
  D[0] = b[0]/L[0][0];
  for ( int ii = 1; ii < 8; ++ii )
  {
    double sum = 0.0;
    for ( int jj = 0; jj < ii; ++jj )
    {
      sum += L[ii][jj] * D[jj];
    }
    D[ii] = (b[ii] - sum)/L[ii][ii];
  }

  // backward substitution
  x[7] = D[7];
  for ( int ii = 7-1; ii > -1; --ii )
  {
    double sum = 0.0;
    for ( int jj = ii+1; jj < 8; ++jj )
    {
      sum += U[ii][jj] * x[jj];
    }
    x[ii] = D[ii] - sum;
  }

  //for ( int ii = 0; ii < 8; ++ii )
  //{
  //  xfinal[ii] = x[ii];
  //}

}


namespace rajaperf
{
class RunParams;

namespace apps
{

//
// These index value types cannot be defined in function scope for
// RAJA CUDA variant to work.
//

class FEMSWEEP : public KernelBase
{
public:

  FEMSWEEP(const RunParams& params);

  ~FEMSWEEP();

  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void runSeqVariant(VariantID vid, size_t tune_idx);
  //void runOpenMPVariant(VariantID vid, size_t tune_idx);
  void runCudaVariant(VariantID vid, size_t tune_idx);
  void runHipVariant(VariantID vid, size_t tune_idx);

  void setCudaTuningDefinitions(VariantID vid);
  void setHipTuningDefinitions(VariantID vid);

  template < size_t block_size >
  void runCudaVariantImpl(VariantID vid);
  template < size_t block_size >
  void runHipVariantImpl(VariantID vid);

private:
  static const size_t default_gpu_block_size = 128;
  using gpu_block_sizes_type = integer::make_gpu_block_size_list_type<default_gpu_block_size,
                                                         integer::MultipleOf<32>>;

  Index_type m_ne;
  Index_type m_nd;
  Index_type m_nfds;

  Index_type m_na;
  Index_type m_ng;

  Real_ptr m_Bdat;
  Real_ptr m_Adat;
  Real_ptr m_Fdat;
  Real_ptr m_Sgdat;
  Real_ptr m_M0dat;
  Real_ptr m_Xdat;
  Real_ptr m_Xfinaldat;

  Index_ptr m_nhpaa_r;
  Index_ptr m_ohpaa_r;
  Index_ptr m_phpaa_r;
  Index_ptr m_order_r;

  Index_ptr m_AngleElem2FaceType;
  Index_ptr m_elem_to_faces     ;
  Index_ptr m_F_g2l             ;
  Index_ptr m_idx1              ;
  Index_ptr m_idx2              ;

  Index_type m_Blen;
  Index_type m_Alen;
  Index_type m_Flen;
  Index_type m_Sglen;
  Index_type m_M0len;
  Index_type m_Xlen;
  Index_type m_Xfinallen;

  Index_type m_nhpaa_rlen;
  Index_type m_ohpaa_rlen;
  Index_type m_phpaa_rlen;
  Index_type m_order_rlen;

  Index_type m_AngleElem2FaceTypelen;
  Index_type m_elem_to_faceslen;
  Index_type m_F_g2llen;
  Index_type m_idx1len;
  Index_type m_idx2len;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
