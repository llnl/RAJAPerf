//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_FEMSWEEP_SOLVER_HPP
#define RAJAPerf_Apps_FEMSWEEP_SOLVER_HPP

#include "RAJA/RAJA.hpp"

namespace rajaperf
{
  
// LU factorization with no pivoting
template <long N>
RAJA_HOST_DEVICE inline void SolveLinearSystemNxN(Real_ptr A,
                                                  const Real_type s,
                                                  Real_const_ptr M,
                                                  Real_array_ref<N> b, /* Real_array_const_ref */
                                                  Real_ptr x)
{
  Real_array2<N, N> tempA;
  Real_array2<N, N> L;
  Real_array2<N, N> U;
  Real_array<N> D;

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
  x[N-1] = D[N-1];
  for ( Index_type ii = N - 1 - 1; ii > -1; --ii )
  {
    Real_type sum = 0.0;
    for ( Index_type jj = ii+1; jj < N; ++jj )
    {
      sum += U[ii][jj] * x[jj];
    }
    x[ii] = D[ii] - sum;
  }

}

} // end namespace rajaperf

#endif // closing endif for header file include guard
