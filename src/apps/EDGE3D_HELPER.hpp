//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_EDGE3D_HELPER_HPP
#define RAJAPerf_Apps_EDGE3D_HELPER_HPP

#include "mixed_fem_helper.hpp"

namespace rajaperf
{

RAJA_HOST_DEVICE
RAJA_INLINE void edge_MpSmatrix(
  Real_array_const_ref<NB> x,
  Real_array_const_ref<NB> y,
  Real_array_const_ref<NB> z,
  Real_type        alpha,
  Real_type        beta,
  const Real_type  detj_tol,
  const Int_type   quad_type,
  const Int_type   quad_order,
  Real_array2_ref<EB, EB> matrix)
{
  // Get integration points and weights
  Real_array<MAX_QUAD_ORDER> qpts_1d;
  Real_array<MAX_QUAD_ORDER> wgts_1d;

  get_quadrature_rule(quad_type, quad_order, qpts_1d, wgts_1d);

  // Compute cell centered Jacobian
  const Real_type jxx_cc = Jxx(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jxy_cc = Jxy(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jxz_cc = Jxz(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jyx_cc = Jyx(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jyy_cc = Jyy(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jyz_cc = Jyz(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jzx_cc = Jzx(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jzy_cc = Jzy(x, y, z, 0.25, 0.25, 0.25, 0.25);
  const Real_type jzz_cc = Jzz(x, y, z, 0.25, 0.25, 0.25, 0.25);

  // Compute cell centered Jacobian determinant
  const Real_type detj_cc = compute_detj(
    jxx_cc, jxy_cc, jxz_cc,
    jyx_cc, jyy_cc, jyz_cc,
    jzx_cc, jzy_cc, jzz_cc);

  // Initialize the stiffness matrix
  for (Int_type m = 0; m < EB; m++) {
    for (Int_type p = m; p < EB; p++) {
      matrix[m][p] = 0.0;
    }
  }

  // Compute values at each quadrature point
  for ( Int_type i = 0; i < quad_order; i++ ) {

    const Real_type xloc = qpts_1d[i];
    const Real_type tmpx = 1. - xloc;

    Real_array<EB> dbasisx = {};
    curl_edgebasis_x(dbasisx, tmpx, xloc);

    for ( Int_type j = 0; j < quad_order; j++ ) {

      const Real_type yloc = qpts_1d[j];
      const Real_type wgtxy = wgts_1d[i]*wgts_1d[j];
      const Real_type tmpy = 1. - yloc;

      Real_type tmpxy    = tmpx*tmpy;
      Real_type xyloc    = xloc*yloc;
      Real_type tmpxyloc = tmpx*yloc;
      Real_type xloctmpy = xloc*tmpy;

      const Real_type jzx = Jzx(x, y, z, tmpxy, xloctmpy, xyloc, tmpxyloc);
      const Real_type jzy = Jzy(x, y, z, tmpxy, xloctmpy, xyloc, tmpxyloc);
      const Real_type jzz = Jzz(x, y, z, tmpxy, xloctmpy, xyloc, tmpxyloc);

      Real_array<EB> ebasisz = {};
      edgebasis_z(ebasisz, tmpxy, xloctmpy, xyloc, tmpxyloc);

      Real_array<EB> dbasisy = {};
      curl_edgebasis_y(dbasisy, tmpy, yloc);

      // Differeniate basis with respect to z at this quadrature point

      for ( Int_type k = 0; k < quad_order; k++ ) {

        const Real_type zloc = qpts_1d[k];
        const Real_type wgts = wgtxy*wgts_1d[k];
        const Real_type tmpz = 1. - zloc;

        const Real_type tmpxz    = tmpx*tmpz;
        const Real_type tmpyz    = tmpy*tmpz;

        const Real_type xzloc    = xloc*zloc;
        const Real_type yzloc    = yloc*zloc;

        const Real_type tmpyzloc = tmpy*zloc;
        const Real_type tmpxzloc = tmpx*zloc;

        const Real_type yloctmpz = yloc*tmpz;
        const Real_type xloctmpz = xloc*tmpz;

        const Real_type jxx = Jxx(x, y, z, tmpyz, yloctmpz, tmpyzloc, yzloc);
        const Real_type jxy = Jxy(x, y, z, tmpyz, yloctmpz, tmpyzloc, yzloc);
        const Real_type jxz = Jxz(x, y, z, tmpyz, yloctmpz, tmpyzloc, yzloc);
        const Real_type jyx = Jyx(x, y, z, tmpxz, xloctmpz, tmpxzloc, xzloc);
        const Real_type jyy = Jyy(x, y, z, tmpxz, xloctmpz, tmpxzloc, xzloc);
        const Real_type jyz = Jyz(x, y, z, tmpxz, xloctmpz, tmpxzloc, xzloc);

        Real_type jinvxx, jinvxy, jinvxz,
                  jinvyx, jinvyy, jinvyz,
                  jinvzx, jinvzy, jinvzz,
                  detj_unfixed, detj, abs_detj, invdetj;

        jacobian_inv(
          jxx, jxy, jxz,
          jyx, jyy, jyz,
          jzx, jzy, jzz,
          detj_cc, detj_tol,
          jinvxx, jinvxy, jinvxz,
          jinvyx, jinvyy, jinvyz,
          jinvzx, jinvzy, jinvzz,
          detj_unfixed, detj, abs_detj, invdetj);

        const Real_type detjwgts = wgts*abs_detj;

        Real_array<EB> ebasisx = {};
        edgebasis_x(ebasisx, tmpyz, yloctmpz, tmpyzloc, yzloc);

        Real_array<EB> ebasisy = {};
        edgebasis_y(ebasisy, tmpxz, xloctmpz, tmpxzloc, xzloc);

        Real_array<EB> dbasisz = {};
        curl_edgebasis_z(dbasisz, tmpz, zloc);

        const Real_type inv_abs_detj = 1./(abs_detj+ptiny);

        Real_array<EB> tebasisx = {};
        Real_array<EB> tebasisy = {};
        Real_array<EB> tebasisz = {};

        transform_edge_basis(
          jinvxx, jinvxy, jinvxz,
          jinvyx, jinvyy, jinvyz,
          jinvzx, jinvzy, jinvzz,
          ebasisx, ebasisy, ebasisz,
          tebasisx, tebasisy, tebasisz);

        Real_array<EB> tdbasisx = {};
        Real_array<EB> tdbasisy = {};
        Real_array<EB> tdbasisz = {};

        transform_curl_edge_basis(
          jxx, jxy, jxz,
          jyx, jyy, jyz,
          jzx, jzy, jzz,
          inv_abs_detj,
          dbasisx, dbasisy, dbasisz,
          tdbasisx, tdbasisy, tdbasisz);

        // the inner product: alpha*<w_i, w_j>
        inner_product(
          detjwgts*alpha,
          Real_array_const_ref<EB>{tebasisx}, Real_array_const_ref<EB>{tebasisy}, Real_array_const_ref<EB>{tebasisz},
          Real_array_const_ref<EB>{tebasisx}, Real_array_const_ref<EB>{tebasisy}, Real_array_const_ref<EB>{tebasisz},
          matrix, true);

         // the inner product: beta*<Curl(w_i), Curl(w_j)>
        inner_product(
          detjwgts*beta,
          Real_array_const_ref<EB>{tdbasisx}, Real_array_const_ref<EB>{tdbasisy}, Real_array_const_ref<EB>{tdbasisz},
          Real_array_const_ref<EB>{tdbasisx}, Real_array_const_ref<EB>{tdbasisy}, Real_array_const_ref<EB>{tdbasisz},
          matrix, true);
      }
    }
  }
}

} // end namespace rajaperf

#endif // closing endif for header file include guard
