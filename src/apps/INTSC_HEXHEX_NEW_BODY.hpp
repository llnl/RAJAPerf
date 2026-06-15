//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_INTSC_HEXHEX_NEW_BODY_HPP
#define RAJAPerf_Apps_INTSC_HEXHEX_NEW_BODY_HPP

#if defined(RAJA_ENABLE_CUDA)
#include <math.h>
#endif

namespace rajaperf {

constexpr int hexhex_new_max_poly_vertices = 9;

// Initial polygon vertex order
static constexpr unsigned long long HEXHEX_NEW_NEXT_INIT = 0xF87654F21ULL;

enum class HexHexPackedPlaneNew {
  H2,   // 1 - x - y >= 0
  X,    // x >= 0
  Y,    // y >= 0
  Z,    // z >= 0
  H,    // h >= 0
  NEG_H // -h - eps >= 0
};

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_new_next_get(unsigned long long pack,
                                         Int_type idx) {
  unsigned int const v =
      static_cast<unsigned int>((pack >> (4 * idx)) & 0xFULL);

  return (v == 0xFULL) ? Int_type(-1) : Int_type(v);
}

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_new_next_set(unsigned long long &pack, Int_type idx,
                                     Int_type next) {
  unsigned long long const shift = static_cast<unsigned long long>(4 * idx);
  unsigned long long const mask = 0xFULL << shift;

  unsigned long long const v = static_cast<unsigned long long>(
      next < 0 ? 0xFULL : static_cast<unsigned int>(next));

  pack = (pack & ~mask) | ((v & 0xFULL) << shift);
}

// Plane representation to reduce local memory
template <HexHexPackedPlaneNew P>
RAJA_HOST_DEVICE RAJA_INLINE Real_type hexhex_plane_value_packed_new(
    Real_type const x[hexhex_new_max_poly_vertices],
    Real_type const y[hexhex_new_max_poly_vertices],
    Real_type const z[hexhex_new_max_poly_vertices], Int_type const j) {
  switch (P) {
  case HexHexPackedPlaneNew::H2:
    return Real_type(1.0) - x[j] - y[j];
  case HexHexPackedPlaneNew::X:
    return x[j];
  case HexHexPackedPlaneNew::Y:
    return y[j];
  case HexHexPackedPlaneNew::Z:
    return z[j];
  case HexHexPackedPlaneNew::H:
    return Real_type(1.0) - x[j] - y[j] - z[j];
  case HexHexPackedPlaneNew::NEG_H:
    return x[j] + y[j] + z[j] - Real_type(1.0) - Real_type(1.0e-50);
  }

  return Real_type(0.0);
}

// only interpolate x,y,z to avoid saving h
RAJA_HOST_DEVICE
RAJA_INLINE void
hexhex_interp_zero_packed_new(Real_type x[hexhex_new_max_poly_vertices],
                              Real_type y[hexhex_new_max_poly_vertices],
                              Real_type z[hexhex_new_max_poly_vertices],
                              Int_type out, Int_type a, Int_type b,
                              Real_type ca, Real_type cb) {
  Real_type t = ca / (ca - cb);
  Real_type omt = Real_type(1.0) - t;

  x[out] = x[a] * omt + x[b] * t;
  y[out] = y[a] * omt + y[b] * t;
  z[out] = z[a] * omt + z[b] * t;
}

template <HexHexPackedPlaneNew P>
RAJA_HOST_DEVICE RAJA_INLINE void
clip_polygon_ge_0_packed_new(Real_type x[hexhex_new_max_poly_vertices],
                             Real_type y[hexhex_new_max_poly_vertices],
                             Real_type z[hexhex_new_max_poly_vertices],
                             Int_type &first, Int_type &avail,
                             unsigned long long &next_pack) {
  Int_type j = first;

  if (j < 0) {
    return;
  }

  Int_type first0 = first;
  Int_type j1 = -1, j2 = -1;
  Int_type jj1 = -1, jj2 = -1;

  Real_type c0 = hexhex_plane_value_packed_new<P>(x, y, z, j);
  Real_type c00 = c0;
  Real_type clast = c0;

// gives better performance that aggressive loop unrolling
#pragma unroll 1
  while (j >= 0) {
    Int_type jj = hexhex_new_next_get(next_pack, j);
    Int_type jp = jj;
    if (jj < 0) {
      jj = first0;
    }

    Real_type c1 = hexhex_plane_value_packed_new<P>(x, y, z, jj);
    if ((c0 >= Real_type(0.0)) && (c1 < Real_type(0.0))) {
      j1 = j;
      jj1 = jj;
    }
    if ((c0 < Real_type(0.0)) && (c1 >= Real_type(0.0))) {
      j2 = j;
      jj2 = jj;
    }
    j = jp;
    clast = c0;
    c0 = c1;
  }

  if (j1 < 0) {
    if (c00 >= Real_type(0.0)) {
      return;
    } else {
      first = -1;
      return;
    }
  }

  Int_type jr1 = -1, jr2 = -1;

  if (j1 >= 0) {
    jr1 = avail;
    avail = hexhex_new_next_get(next_pack, avail);
    hexhex_interp_zero_packed_new(
        x, y, z, jr1, j1, jj1, hexhex_plane_value_packed_new<P>(x, y, z, j1),
        hexhex_plane_value_packed_new<P>(x, y, z, jj1));

    jr2 = avail;
    avail = hexhex_new_next_get(next_pack, avail);
    hexhex_interp_zero_packed_new(
        x, y, z, jr2, j2, jj2, hexhex_plane_value_packed_new<P>(x, y, z, j2),
        hexhex_plane_value_packed_new<P>(x, y, z, jj2));
  }

  first = -1;

  j = first0;

// gives better performance that aggressive loop unrolling
#pragma unroll 1
  while (j >= 0) {
    Int_type jp = hexhex_new_next_get(next_pack, j);
    if (hexhex_plane_value_packed_new<P>(x, y, z, j) < Real_type(0.0)) {
      hexhex_new_next_set(next_pack, j, avail);
      avail = j;
    } else if (first == -1) {
      first = j;
    }
    j = jp;
  }

  if (j1 >= 0) {
    hexhex_new_next_set(next_pack, j1, jr1);
    hexhex_new_next_set(next_pack, jr1, jr2);
    hexhex_new_next_set(next_pack, jr2,
                        ((clast < Real_type(0.0)) || (c00 < Real_type(0.0)))
                            ? Int_type(-1)
                            : jj2);
  }
}

//   Simplified volume calculation, for the area under one
//   polygonal face, on the Cuda device.
//   Planar polygon.
//   Compute volume, moments between polygon and the z=0 plane.
RAJA_HOST_DEVICE
RAJA_INLINE void cuda_hex_volpolyh_1poly_packed_new(
    Real_type const x[hexhex_new_max_poly_vertices],
    Real_type const y[hexhex_new_max_poly_vertices],
    Real_type const z[hexhex_new_max_poly_vertices], Int_type const first,
    unsigned long long const next_pack, Real_type &vv, Real_type &vx,
    Real_type &vy, Real_type &vz) {
  if (first < 0) {
    return;
  }

  Int_type const j0 = first;
  Real_type x0 = x[j0];
  Real_type y0 = y[j0];
  Real_type z0 = z[j0];

  Int_type const j1 = hexhex_new_next_get(next_pack, j0);
  if (j1 < 0) {
    return;
  }

  Real_type x1 = x[j1];
  Real_type y1 = y[j1];
  Real_type z1 = z[j1];
  Real_type dx1 = x1 - x0;
  Real_type dy1 = y1 - y0;

  Int_type j2 = hexhex_new_next_get(next_pack, j1);

#pragma unroll 1
  while (j2 >= 0) {
    Real_type x2 = x[j2];
    Real_type y2 = y[j2];
    Real_type z2 = z[j2];
    Real_type dx2 = x2 - x0;
    Real_type dy2 = y2 - y0;

    Real_type area2 = (dx1 * dy2 - dx2 * dy1);
    Real_type v0 = (z0 + z1 + z2) * area2;
    vv += v0;
    vx += v0 * (x0 + x1 + x2) + area2 * (x0 * z0 + x1 * z1 + x2 * z2);
    vy += v0 * (y0 + y1 + y2) + area2 * (y0 * z0 + y1 * z1 + y2 * z2);
    vz += (z0 * z0 + z1 * z1 + z2 * z2 + z0 * z1 + z0 * z2 + z1 * z2) * area2;

    x1 = x2;
    y1 = y2;
    z1 = z2; // Rotate.
    dx1 = dx2;
    dy1 = dy2;

    j2 = hexhex_new_next_get(next_pack, j2);
  }
}

struct HexHexTargetTetMapNew {
  Real_type x0, y0, z0;

  Real_type e1x, e1y, e1z;
  Real_type e2x, e2y, e2z;
  Real_type e3x, e3y, e3z;

  Real_type r0x, r0y, r0z;
  Real_type r1x, r1y, r1z;
  Real_type r2x, r2y, r2z;

  Real_type det;
};

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_new_vert_cyc(Int_type k) {
  switch (k) {
  case 0:
    return 1;
  case 1:
    return 3;
  case 2:
    return 2;
  case 3:
    return 6;
  case 4:
    return 4;
  default:
    return 5;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_new_vert_cyc_next(Int_type k) {
  switch (k) {
  case 0:
    return 3;
  case 1:
    return 2;
  case 2:
    return 6;
  case 3:
    return 4;
  case 4:
    return 5;
  default:
    return 1;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_new_tet_node(Int_type const tet,
                                         Int_type const local) {
  if (local == 0) {
    return 0;
  }
  if (local == 1) {
    return hexhex_new_vert_cyc(tet);
  }
  if (local == 2) {
    return hexhex_new_vert_cyc_next(tet);
  }
  return 7;
}

RAJA_HOST_DEVICE
RAJA_INLINE void donor_tet_nodes_fast(Int_type const dtet,
                                      Int_type &n0,
                                      Int_type &n1,
                                      Int_type &n2,
                                      Int_type &n3) {
  n0 = 0;
  n3 = 7;

  switch (dtet) {
  case 0:
    n1 = 1;
    n2 = 3;
    break;
  case 1:
    n1 = 3;
    n2 = 2;
    break;
  case 2:
    n1 = 2;
    n2 = 6;
    break;
  case 3:
    n1 = 6;
    n2 = 4;
    break;
  case 4:
    n1 = 4;
    n2 = 5;
    break;
  default:
    n1 = 5;
    n2 = 1;
    break;
  }
}

template <Int_type TET>
struct HexHexTetNodesFixedNew {
  static_assert(TET >= 0 && TET < 6, "tet id must be in [0, 5]");

  static constexpr Int_type n0 = 0;
  static constexpr Int_type n1 =
      TET == 0 ? 1 :
      TET == 1 ? 3 :
      TET == 2 ? 2 :
      TET == 3 ? 6 :
      TET == 4 ? 4 : 5;
  static constexpr Int_type n2 =
      TET == 0 ? 3 :
      TET == 1 ? 2 :
      TET == 2 ? 6 :
      TET == 3 ? 4 :
      TET == 4 ? 5 : 1;
  static constexpr Int_type n3 = 7;
};

RAJA_HOST_DEVICE
RAJA_INLINE void make_target_tet_map_new(Real_const_ptr xts,
                                         Int_type const ttet,
                                         HexHexTargetTetMapNew &m) {
  Real_const_ptr yts = xts + 8;
  Real_const_ptr zts = yts + 8;

  Int_type const v1 = hexhex_new_vert_cyc(ttet);
  Int_type const v2 = hexhex_new_vert_cyc_next(ttet);

  m.x0 = xts[0];
  m.y0 = yts[0];
  m.z0 = zts[0];

  m.e1x = xts[v1] - xts[0];
  m.e1y = yts[v1] - yts[0];
  m.e1z = zts[v1] - zts[0];

  m.e2x = xts[v2] - xts[0];
  m.e2y = yts[v2] - yts[0];
  m.e2z = zts[v2] - zts[0];

  m.e3x = xts[7] - xts[0];
  m.e3y = yts[7] - yts[0];
  m.e3z = zts[7] - zts[0];

  m.det = m.e1x * m.e2y * m.e3z - m.e1x * m.e3y * m.e2z +
          m.e2x * m.e3y * m.e1z - m.e2x * m.e1y * m.e3z +
          m.e3x * m.e1y * m.e2z - m.e3x * m.e2y * m.e1z;

  Real_type const deti = m.det / (m.det * m.det + Real_type(1.0e-100));

  m.r0x = (m.e2y * m.e3z - m.e2z * m.e3y) * deti;
  m.r0y = (m.e2z * m.e3x - m.e2x * m.e3z) * deti;
  m.r0z = (m.e2x * m.e3y - m.e2y * m.e3x) * deti;

  m.r1x = (m.e3y * m.e1z - m.e3z * m.e1y) * deti;
  m.r1y = (m.e3z * m.e1x - m.e3x * m.e1z) * deti;
  m.r1z = (m.e3x * m.e1y - m.e3y * m.e1x) * deti;

  m.r2x = (m.e1y * m.e2z - m.e1z * m.e2y) * deti;
  m.r2y = (m.e1z * m.e2x - m.e1x * m.e2z) * deti;
  m.r2z = (m.e1x * m.e2y - m.e1y * m.e2x) * deti;
}

template <Int_type TTET>
RAJA_HOST_DEVICE
RAJA_INLINE void make_target_tet_map_new_fixed(
    Real_const_ptr xts,
    HexHexTargetTetMapNew &m) {
  Real_const_ptr yts = xts + 8;
  Real_const_ptr zts = yts + 8;

  constexpr Int_type v1 = HexHexTetNodesFixedNew<TTET>::n1;
  constexpr Int_type v2 = HexHexTetNodesFixedNew<TTET>::n2;

  m.x0 = xts[0];
  m.y0 = yts[0];
  m.z0 = zts[0];

  m.e1x = xts[v1] - xts[0];
  m.e1y = yts[v1] - yts[0];
  m.e1z = zts[v1] - zts[0];

  m.e2x = xts[v2] - xts[0];
  m.e2y = yts[v2] - yts[0];
  m.e2z = zts[v2] - zts[0];

  m.e3x = xts[7] - xts[0];
  m.e3y = yts[7] - yts[0];
  m.e3z = zts[7] - zts[0];

  m.det = m.e1x * m.e2y * m.e3z - m.e1x * m.e3y * m.e2z +
          m.e2x * m.e3y * m.e1z - m.e2x * m.e1y * m.e3z +
          m.e3x * m.e1y * m.e2z - m.e3x * m.e2y * m.e1z;

  Real_type const deti = m.det / (m.det * m.det + Real_type(1.0e-100));

  m.r0x = (m.e2y * m.e3z - m.e2z * m.e3y) * deti;
  m.r0y = (m.e2z * m.e3x - m.e2x * m.e3z) * deti;
  m.r0z = (m.e2x * m.e3y - m.e2y * m.e3x) * deti;

  m.r1x = (m.e3y * m.e1z - m.e3z * m.e1y) * deti;
  m.r1y = (m.e3z * m.e1x - m.e3x * m.e1z) * deti;
  m.r1z = (m.e3x * m.e1y - m.e3y * m.e1x) * deti;

  m.r2x = (m.e1y * m.e2z - m.e1z * m.e2y) * deti;
  m.r2y = (m.e1z * m.e2x - m.e1x * m.e2z) * deti;
  m.r2z = (m.e1x * m.e2y - m.e1y * m.e2x) * deti;
}

RAJA_HOST_DEVICE
RAJA_INLINE void target_map_to_ref_new(HexHexTargetTetMapNew const &m,
                                       Real_type const x, Real_type const y,
                                       Real_type const z, Real_type &xr,
                                       Real_type &yr, Real_type &zr) {
  Real_type const dx = x - m.x0;
  Real_type const dy = y - m.y0;
  Real_type const dz = z - m.z0;

  xr = dx * m.r0x + dy * m.r0y + dz * m.r0z;
  yr = dx * m.r1x + dy * m.r1y + dz * m.r1z;
  zr = dx * m.r2x + dy * m.r2y + dz * m.r2z;
}

template <Int_type TTET, Int_type DTET>
RAJA_HOST_DEVICE
RAJA_INLINE void transform_donor_tet_fixed(
    Real_const_ptr xds,
    HexHexTargetTetMapNew const &m,
    Real_type x[4],
    Real_type y[4],
    Real_type z[4]) {
  static_assert(TTET >= 0 && TTET < 6, "target tet id must be in [0, 5]");

  constexpr Int_type n0 = HexHexTetNodesFixedNew<DTET>::n0;
  constexpr Int_type n1 = HexHexTetNodesFixedNew<DTET>::n1;
  constexpr Int_type n2 = HexHexTetNodesFixedNew<DTET>::n2;
  constexpr Int_type n3 = HexHexTetNodesFixedNew<DTET>::n3;

  target_map_to_ref_new(
      m, xds[n0], xds[8 + n0], xds[16 + n0], x[0], y[0], z[0]);
  target_map_to_ref_new(
      m, xds[n1], xds[8 + n1], xds[16 + n1], x[1], y[1], z[1]);
  target_map_to_ref_new(
      m, xds[n2], xds[8 + n2], xds[16 + n2], x[2], y[2], z[2]);
  target_map_to_ref_new(
      m, xds[n3], xds[8 + n3], xds[16 + n3], x[3], y[3], z[3]);
}

#ifndef HEXHEX_TETTET_COMPAT_EPS
#define HEXHEX_TETTET_COMPAT_EPS 0
#endif

struct HexHexPlaneNew {
  Real_type nx, ny, nz, d;
};

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_abs_new(Real_type const x) {
#if defined(RAJA_ENABLE_CUDA) && defined(REAL_TYPE_IS_FLOAT)
  return fabsf(x);
#elif defined(RAJA_ENABLE_CUDA)
  return fabs(x);
#else
  return x < Real_type(0.0) ? -x : x;
#endif
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_max4_new(Real_type const a,
                                      Real_type const b,
                                      Real_type const c,
                                      Real_type const d) {
  Real_type ab = a > b ? a : b;
  Real_type cd = c > d ? c : d;
  return ab > cd ? ab : cd;
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_det3_new(Real_type const ax,
                                      Real_type const ay,
                                      Real_type const az,
                                      Real_type const bx,
                                      Real_type const by,
                                      Real_type const bz,
                                      Real_type const cx,
                                      Real_type const cy,
                                      Real_type const cz) {
  return ax * (by * cz - bz * cy) -
         ay * (bx * cz - bz * cx) +
         az * (bx * cy - by * cx);
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_plane_value_new(HexHexPlaneNew const &p,
                                             Real_type const x,
                                             Real_type const y,
                                             Real_type const z) {
  return p.nx * x + p.ny * y + p.nz * z + p.d;
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_tettet_scale_eps_new(Real_type const x[4],
                                                  Real_type const y[4],
                                                  Real_type const z[4]) {
  Real_type s = Real_type(1.0);

  for (Int_type i = 0; i < 4; ++i) {
    Real_type const ax = hexhex_abs_new(x[i]);
    Real_type const ay = hexhex_abs_new(y[i]);
    Real_type const az = hexhex_abs_new(z[i]);
    s = s > ax ? s : ax;
    s = s > ay ? s : ay;
    s = s > az ? s : az;
  }

#if defined(REAL_TYPE_IS_FLOAT)
  return Real_type(1.0e-5) * s;
#else
  return Real_type(1.0e-11) * s;
#endif
}

RAJA_HOST_DEVICE
RAJA_INLINE void signed_tet_moments_new(Real_type const x[4],
                                        Real_type const y[4],
                                        Real_type const z[4],
                                        Real_type &V,
                                        Real_type &Mx,
                                        Real_type &My,
                                        Real_type &Mz) {
  Real_type const det = hexhex_det3_new(
      x[1] - x[0], y[1] - y[0], z[1] - z[0],
      x[2] - x[0], y[2] - y[0], z[2] - z[0],
      x[3] - x[0], y[3] - y[0], z[3] - z[0]);
  V = det / Real_type(6.0);
  Mx = V * (x[0] + x[1] + x[2] + x[3]) / Real_type(4.0);
  My = V * (y[0] + y[1] + y[2] + y[3]) / Real_type(4.0);
  Mz = V * (z[0] + z[1] + z[2] + z[3]) / Real_type(4.0);
}

struct FaceBits8New {
  unsigned short b0;
  unsigned short b1;
  unsigned short b2;
  unsigned short b3;
  unsigned short b4;
  unsigned short b5;
  unsigned short b6;
  unsigned short b7;
};

RAJA_HOST_DEVICE
RAJA_INLINE void facebits_clear_new(FaceBits8New &fb) {
  fb.b0 = 0;
  fb.b1 = 0;
  fb.b2 = 0;
  fb.b3 = 0;
  fb.b4 = 0;
  fb.b5 = 0;
  fb.b6 = 0;
  fb.b7 = 0;
}

RAJA_HOST_DEVICE
RAJA_INLINE void mark_facebits_new(FaceBits8New &fb,
                                   Int_type const id,
                                   unsigned char const mask) {
  unsigned short const vbit = static_cast<unsigned short>(1u << id);

  if (mask & (1u << 0)) fb.b0 = static_cast<unsigned short>(fb.b0 | vbit);
  if (mask & (1u << 1)) fb.b1 = static_cast<unsigned short>(fb.b1 | vbit);
  if (mask & (1u << 2)) fb.b2 = static_cast<unsigned short>(fb.b2 | vbit);
  if (mask & (1u << 3)) fb.b3 = static_cast<unsigned short>(fb.b3 | vbit);
  if (mask & (1u << 4)) fb.b4 = static_cast<unsigned short>(fb.b4 | vbit);
  if (mask & (1u << 5)) fb.b5 = static_cast<unsigned short>(fb.b5 | vbit);
  if (mask & (1u << 6)) fb.b6 = static_cast<unsigned short>(fb.b6 | vbit);
  if (mask & (1u << 7)) fb.b7 = static_cast<unsigned short>(fb.b7 | vbit);
}

RAJA_HOST_DEVICE
RAJA_INLINE void mark_one_facebit_new(FaceBits8New &fb,
                                      Int_type const id,
                                      Int_type const face) {
  unsigned short const vbit = static_cast<unsigned short>(1u << id);

  switch (face) {
  case 0:
    fb.b0 = static_cast<unsigned short>(fb.b0 | vbit);
    break;
  case 1:
    fb.b1 = static_cast<unsigned short>(fb.b1 | vbit);
    break;
  case 2:
    fb.b2 = static_cast<unsigned short>(fb.b2 | vbit);
    break;
  case 3:
    fb.b3 = static_cast<unsigned short>(fb.b3 | vbit);
    break;
  case 4:
    fb.b4 = static_cast<unsigned short>(fb.b4 | vbit);
    break;
  case 5:
    fb.b5 = static_cast<unsigned short>(fb.b5 | vbit);
    break;
  case 6:
    fb.b6 = static_cast<unsigned short>(fb.b6 | vbit);
    break;
  default:
    fb.b7 = static_cast<unsigned short>(fb.b7 | vbit);
    break;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void add_unique_tettet_vertex_bits_new(
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const x,
    Real_type const y,
    Real_type const z,
    unsigned char const mask,
    Real_type const eps) {
  Real_type const eps2 = eps * eps;

  for (Int_type i = 0; i < nv; ++i) {
    Real_type const dx = vx[i] - x;
    Real_type const dy = vy[i] - y;
    Real_type const dz = vz[i] - z;

    if (dx * dx + dy * dy + dz * dz <= eps2) {
      mark_facebits_new(facebits, i, mask);
      return;
    }
  }

  if (nv < 12) {
    vx[nv] = x;
    vy[nv] = y;
    vz[nv] = z;
    mark_facebits_new(facebits, nv, mask);
    ++nv;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Int_type facebits_popcount_new(unsigned short bits) {
#if defined(RAJA_ENABLE_CUDA) && defined(__CUDA_ARCH__)
  return __popc(static_cast<unsigned int>(bits));
#else
  return __builtin_popcount(static_cast<unsigned int>(bits));
#endif
}

RAJA_HOST_DEVICE
RAJA_INLINE void facebits_extract_three_new(unsigned short bits,
                                           Int_type &i0,
                                           Int_type &i1,
                                           Int_type &i2) {
  unsigned int b = static_cast<unsigned int>(bits);

#if defined(RAJA_ENABLE_CUDA) && defined(__CUDA_ARCH__)
  i0 = __ffs(b) - 1;
  b &= b - 1;

  i1 = __ffs(b) - 1;
  b &= b - 1;

  i2 = __ffs(b) - 1;
#else
  i0 = __builtin_ctz(b);
  b &= b - 1;

  i1 = __builtin_ctz(b);
  b &= b - 1;

  i2 = __builtin_ctz(b);
#endif
}

RAJA_HOST_DEVICE
RAJA_INLINE void mark_three_facebits_new(FaceBits8New &fb,
                                         Int_type const id,
                                         unsigned char const mask) {
  Int_type f0, f1, f2;
  facebits_extract_three_new(static_cast<unsigned short>(mask), f0, f1, f2);
  mark_one_facebit_new(fb, id, f0);
  mark_one_facebit_new(fb, id, f1);
  mark_one_facebit_new(fb, id, f2);
}

RAJA_HOST_DEVICE
RAJA_INLINE void add_tettet_vertex_direct_three_bits_new(
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const x,
    Real_type const y,
    Real_type const z,
    unsigned char const mask) {
  if (nv < 12) {
    vx[nv] = x;
    vy[nv] = y;
    vz[nv] = z;
    mark_three_facebits_new(facebits, nv, mask);
    ++nv;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE bool hexhex_tettet_angle_less_new(Real_type const ax,
                                              Real_type const ay,
                                              Real_type const bx,
                                              Real_type const by) {
  bool const ah = (ay > Real_type(0.0)) ||
                  (ay == Real_type(0.0) && ax >= Real_type(0.0));
  bool const bh = (by > Real_type(0.0)) ||
                  (by == Real_type(0.0) && bx >= Real_type(0.0));

  if (ah != bh) {
    return ah;
  }

  Real_type const cross = ax * by - ay * bx;
  if (cross != Real_type(0.0)) {
    return cross > Real_type(0.0);
  }

  return ax * ax + ay * ay < bx * bx + by * by;
}

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_tettet_face_basis_new(HexHexPlaneNew const &p,
                                              Real_type &ux,
                                              Real_type &uy,
                                              Real_type &uz,
                                              Real_type &vx,
                                              Real_type &vy,
                                              Real_type &vz) {
  if (hexhex_abs_new(p.nx) > hexhex_abs_new(p.nz)) {
    ux = -p.ny;
    uy = p.nx;
    uz = Real_type(0.0);
  } else {
    ux = Real_type(0.0);
    uy = -p.nz;
    uz = p.ny;
  }

  vx = p.ny * uz - p.nz * uy;
  vy = p.nz * ux - p.nx * uz;
  vz = p.nx * uy - p.ny * ux;
}

RAJA_HOST_DEVICE
RAJA_INLINE void reverse_tettet_ids_new(Int_type ids[12], Int_type const nids) {
  for (Int_type i = 0; i < nids / 2; ++i) {
    Int_type const t = ids[i];
    ids[i] = ids[nids - 1 - i];
    ids[nids - 1 - i] = t;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type target_plane_value_new(Int_type const p,
                                             Real_type const x,
                                             Real_type const y,
                                             Real_type const z) {
  switch (p) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    return Real_type(1.0) - x - y - z;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type target_plane_eps_new(Int_type const p,
                                           Real_type const eps) {
  return p == 3 ? Real_type(5.0) * eps : Real_type(2.0) * eps;
}

RAJA_HOST_DEVICE
RAJA_INLINE void get_target_vertex_new(Int_type const v,
                                       Real_type &x,
                                       Real_type &y,
                                       Real_type &z) {
  x = v == 1 ? Real_type(1.0) : Real_type(0.0);
  y = v == 2 ? Real_type(1.0) : Real_type(0.0);
  z = v == 3 ? Real_type(1.0) : Real_type(0.0);
}

RAJA_HOST_DEVICE
RAJA_INLINE unsigned char target_vertex_mask_new(Int_type const v) {
  switch (v) {
  case 0:
    return static_cast<unsigned char>((1u << 0) | (1u << 1) | (1u << 2));
  case 1:
    return static_cast<unsigned char>((1u << 1) | (1u << 2) | (1u << 3));
  case 2:
    return static_cast<unsigned char>((1u << 0) | (1u << 2) | (1u << 3));
  default:
    return static_cast<unsigned char>((1u << 0) | (1u << 1) | (1u << 3));
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE unsigned char donor_vertex_mask_new(Int_type const v) {
  switch (v) {
  case 0:
    return static_cast<unsigned char>((1u << 4) | (1u << 5) | (1u << 6));
  case 1:
    return static_cast<unsigned char>((1u << 4) | (1u << 5) | (1u << 7));
  case 2:
    return static_cast<unsigned char>((1u << 4) | (1u << 6) | (1u << 7));
  default:
    return static_cast<unsigned char>((1u << 5) | (1u << 6) | (1u << 7));
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void make_donor_tet_plane_one_new(Real_type const x[4],
                                              Real_type const y[4],
                                              Real_type const z[4],
                                              Int_type const ia,
                                              Int_type const ib,
                                              Int_type const ic,
                                              Int_type const io,
                                              HexHexPlaneNew &p) {
  Real_type const bax = x[ib] - x[ia];
  Real_type const bay = y[ib] - y[ia];
  Real_type const baz = z[ib] - z[ia];
  Real_type const cax = x[ic] - x[ia];
  Real_type const cay = y[ic] - y[ia];
  Real_type const caz = z[ic] - z[ia];

  p.nx = bay * caz - baz * cay;
  p.ny = baz * cax - bax * caz;
  p.nz = bax * cay - bay * cax;

  Real_type const oax = x[io] - x[ia];
  Real_type const oay = y[io] - y[ia];
  Real_type const oaz = z[io] - z[ia];

  if (p.nx * oax + p.ny * oay + p.nz * oaz < Real_type(0.0)) {
    p.nx = -p.nx;
    p.ny = -p.ny;
    p.nz = -p.nz;
  }

  p.d = -(p.nx * x[ia] + p.ny * y[ia] + p.nz * z[ia]);
}

RAJA_HOST_DEVICE
RAJA_INLINE void make_donor_tet_planes_only_new(Real_type const x[4],
                                                Real_type const y[4],
                                                Real_type const z[4],
                                                HexHexPlaneNew &dp0,
                                                HexHexPlaneNew &dp1,
                                                HexHexPlaneNew &dp2,
                                                HexHexPlaneNew &dp3) {
  make_donor_tet_plane_one_new(x, y, z, 0, 1, 2, 3, dp0);
  make_donor_tet_plane_one_new(x, y, z, 0, 3, 1, 2, dp1);
  make_donor_tet_plane_one_new(x, y, z, 0, 2, 3, 1, dp2);
  make_donor_tet_plane_one_new(x, y, z, 1, 3, 2, 0, dp3);
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type scaled_plane_eps_new(HexHexPlaneNew const &p,
                                           Real_type const eps) {
  return eps * (hexhex_abs_new(p.nx) + hexhex_abs_new(p.ny) +
                hexhex_abs_new(p.nz) + hexhex_abs_new(p.d) +
                Real_type(1.0));
}

RAJA_HOST_DEVICE
RAJA_INLINE void add_candidate_bits_if_inside_all_planes_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const px,
    Real_type const py,
    Real_type const pz,
    unsigned char mask,
    Real_type const eps) {
  Real_type c = px;
  Real_type peps = target_plane_eps_new(0, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 0));
  }

  c = py;
  peps = target_plane_eps_new(1, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 1));
  }

  c = pz;
  peps = target_plane_eps_new(2, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 2));
  }

  c = Real_type(1.0) - px - py - pz;
  peps = target_plane_eps_new(3, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 3));
  }

  c = hexhex_plane_value_new(dp0, px, py, pz);
  peps = scaled_plane_eps_new(dp0, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 4));
  }

  c = hexhex_plane_value_new(dp1, px, py, pz);
  peps = scaled_plane_eps_new(dp1, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 5));
  }

  c = hexhex_plane_value_new(dp2, px, py, pz);
  peps = scaled_plane_eps_new(dp2, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 6));
  }

  c = hexhex_plane_value_new(dp3, px, py, pz);
  peps = scaled_plane_eps_new(dp3, eps);
  if (c < -peps) {
    return;
  }
  if (hexhex_abs_new(c) <= peps) {
    mask = static_cast<unsigned char>(mask | (1u << 7));
  }

  if (facebits_popcount_new(static_cast<unsigned short>(mask)) == 3) {
    add_tettet_vertex_direct_three_bits_new(vx, vy, vz, facebits, nv,
                                            px, py, pz, mask);
  } else {
    add_unique_tettet_vertex_bits_new(vx, vy, vz, facebits, nv,
                                      px, py, pz, mask, eps);
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void orient_triangle_to_normal_new(Real_type const vx[12],
                                               Real_type const vy[12],
                                               Real_type const vz[12],
                                               Int_type ia,
                                               Int_type &ib,
                                               Int_type &ic,
                                               Real_type const nx,
                                               Real_type const ny,
                                               Real_type const nz) {
  Real_type const e1x = vx[ib] - vx[ia];
  Real_type const e1y = vy[ib] - vy[ia];
  Real_type const e1z = vz[ib] - vz[ia];
  Real_type const e2x = vx[ic] - vx[ia];
  Real_type const e2y = vy[ic] - vy[ia];
  Real_type const e2z = vz[ic] - vz[ia];
  Real_type const nfx = e1y * e2z - e1z * e2y;
  Real_type const nfy = e1z * e2x - e1x * e2z;
  Real_type const nfz = e1x * e2y - e1y * e2x;

  if (nfx * nx + nfy * ny + nfz * nz < Real_type(0.0)) {
    Int_type const t = ib;
    ib = ic;
    ic = t;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void integrate_oriented_triangle_new(Real_type const vx[12],
                                                Real_type const vy[12],
                                                Real_type const vz[12],
                                                Int_type const ia,
                                                Int_type const ib,
                                                Int_type const ic,
                                                Real_type &V,
                                                Real_type &Mx,
                                                Real_type &My,
                                                Real_type &Mz) {
  Real_type const ax = vx[ia];
  Real_type const ay = vy[ia];
  Real_type const az = vz[ia];
  Real_type const bx = vx[ib];
  Real_type const by = vy[ib];
  Real_type const bz = vz[ib];
  Real_type const cx = vx[ic];
  Real_type const cy = vy[ic];
  Real_type const cz = vz[ic];

  Real_type const det = hexhex_det3_new(ax, ay, az, bx, by, bz, cx, cy, cz);
  Real_type const vol = det / Real_type(6.0);

  V += vol;
  Mx += vol * (ax + bx + cx) / Real_type(4.0);
  My += vol * (ay + by + cy) / Real_type(4.0);
  Mz += vol * (az + bz + cz) / Real_type(4.0);
}

template <Int_type P>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz);

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<0>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = Real_type(-1.0);
  ny = Real_type(0.0);
  nz = Real_type(0.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<1>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = Real_type(0.0);
  ny = Real_type(-1.0);
  nz = Real_type(0.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<2>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = Real_type(0.0);
  ny = Real_type(0.0);
  nz = Real_type(-1.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<3>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = Real_type(1.0);
  ny = Real_type(1.0);
  nz = Real_type(1.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<4>(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = -dp0.nx;
  ny = -dp0.ny;
  nz = -dp0.nz;
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<5>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = -dp1.nx;
  ny = -dp1.ny;
  nz = -dp1.nz;
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<6>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = -dp2.nx;
  ny = -dp2.ny;
  nz = -dp2.nz;
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_outward_normal_new<7>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp3,
    Real_type &nx,
    Real_type &ny,
    Real_type &nz) {
  nx = -dp3.nx;
  ny = -dp3.ny;
  nz = -dp3.nz;
}

template <Int_type P>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz);

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<0>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  ux = Real_type(0.0);
  uy = Real_type(0.0);
  uz = Real_type(1.0);
  vx = Real_type(0.0);
  vy = Real_type(1.0);
  vz = Real_type(0.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<1>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  ux = Real_type(1.0);
  uy = Real_type(0.0);
  uz = Real_type(0.0);
  vx = Real_type(0.0);
  vy = Real_type(0.0);
  vz = Real_type(1.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<2>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  ux = Real_type(0.0);
  uy = Real_type(1.0);
  uz = Real_type(0.0);
  vx = Real_type(1.0);
  vy = Real_type(0.0);
  vz = Real_type(0.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<3>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  ux = Real_type(1.0);
  uy = Real_type(-1.0);
  uz = Real_type(0.0);
  vx = Real_type(1.0);
  vy = Real_type(1.0);
  vz = Real_type(-2.0);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<4>(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  hexhex_tettet_face_basis_new(dp0, ux, uy, uz, vx, vy, vz);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<5>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  hexhex_tettet_face_basis_new(dp1, ux, uy, uz, vx, vy, vz);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<6>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  hexhex_tettet_face_basis_new(dp2, ux, uy, uz, vx, vy, vz);
}

template <>
RAJA_HOST_DEVICE
RAJA_INLINE void tettet_face_basis_new<7>(
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &,
    HexHexPlaneNew const &dp3,
    Real_type &ux,
    Real_type &uy,
    Real_type &uz,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  hexhex_tettet_face_basis_new(dp3, ux, uy, uz, vx, vy, vz);
}

template <Int_type P>
RAJA_HOST_DEVICE
RAJA_INLINE void integrate_one_triangle_face_new(
    Real_type const vx[12],
    Real_type const vy[12],
    Real_type const vz[12],
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Int_type ia,
    Int_type ib,
    Int_type ic,
    Real_type &V,
    Real_type &Mx,
    Real_type &My,
    Real_type &Mz) {
  Real_type nx, ny, nz;
  tettet_face_outward_normal_new<P>(dp0, dp1, dp2, dp3, nx, ny, nz);
  orient_triangle_to_normal_new(vx, vy, vz, ia, ib, ic, nx, ny, nz);
  integrate_oriented_triangle_new(vx, vy, vz, ia, ib, ic, V, Mx, My, Mz);
}

template <Int_type P>
RAJA_HOST_DEVICE
RAJA_INLINE void integrate_general_face_from_bits_new(
    unsigned short const bits,
    Real_type const vx[12],
    Real_type const vy[12],
    Real_type const vz[12],
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type &V,
    Real_type &Mx,
    Real_type &My,
    Real_type &Mz) {
  Int_type ids[12];
  Int_type n = 0;

#pragma unroll
  for (Int_type i = 0; i < 12; ++i) {
    if ((bits & (1u << i)) != 0) {
      ids[n++] = i;
    }
  }

  Real_type cx = Real_type(0.0);
  Real_type cy = Real_type(0.0);
  Real_type cz = Real_type(0.0);
  for (Int_type i = 0; i < n; ++i) {
    cx += vx[ids[i]];
    cy += vy[ids[i]];
    cz += vz[ids[i]];
  }
  cx /= static_cast<Real_type>(n);
  cy /= static_cast<Real_type>(n);
  cz /= static_cast<Real_type>(n);

  Real_type ux, uy, uz, bx, by, bz;
  tettet_face_basis_new<P>(dp0, dp1, dp2, dp3, ux, uy, uz, bx, by, bz);

  for (Int_type i = 1; i < n; ++i) {
    Int_type const id = ids[i];
    Real_type const iax =
        (vx[id] - cx) * ux + (vy[id] - cy) * uy + (vz[id] - cz) * uz;
    Real_type const iay =
        (vx[id] - cx) * bx + (vy[id] - cy) * by + (vz[id] - cz) * bz;

    Int_type j = i - 1;
    for (; j >= 0; --j) {
      Int_type const jd = ids[j];
      Real_type const jax =
          (vx[jd] - cx) * ux + (vy[jd] - cy) * uy + (vz[jd] - cz) * uz;
      Real_type const jay =
          (vx[jd] - cx) * bx + (vy[jd] - cy) * by + (vz[jd] - cz) * bz;
      if (!hexhex_tettet_angle_less_new(iax, iay, jax, jay)) {
        break;
      }
      ids[j + 1] = ids[j];
    }
    ids[j + 1] = id;
  }

  Real_type const e1x = vx[ids[1]] - vx[ids[0]];
  Real_type const e1y = vy[ids[1]] - vy[ids[0]];
  Real_type const e1z = vz[ids[1]] - vz[ids[0]];
  Real_type const e2x = vx[ids[2]] - vx[ids[0]];
  Real_type const e2y = vy[ids[2]] - vy[ids[0]];
  Real_type const e2z = vz[ids[2]] - vz[ids[0]];
  Real_type const nfx = e1y * e2z - e1z * e2y;
  Real_type const nfy = e1z * e2x - e1x * e2z;
  Real_type const nfz = e1x * e2y - e1y * e2x;

  Real_type nx, ny, nz;
  tettet_face_outward_normal_new<P>(dp0, dp1, dp2, dp3, nx, ny, nz);
  if (nfx * nx + nfy * ny + nfz * nz < Real_type(0.0)) {
    reverse_tettet_ids_new(ids, n);
  }

  for (Int_type k = 1; k + 1 < n; ++k) {
    integrate_oriented_triangle_new(
        vx, vy, vz, ids[0], ids[k], ids[k + 1], V, Mx, My, Mz);
  }
}

template <Int_type P>
RAJA_HOST_DEVICE
RAJA_INLINE void integrate_face_from_bits_new(
    unsigned short const bits,
    Real_type const vx[12],
    Real_type const vy[12],
    Real_type const vz[12],
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type &V,
    Real_type &Mx,
    Real_type &My,
    Real_type &Mz) {
  Int_type const nids = facebits_popcount_new(bits);

  if (nids < 3) {
    return;
  }

  if (nids == 3) {
    Int_type ia, ib, ic;
    facebits_extract_three_new(bits, ia, ib, ic);

    integrate_one_triangle_face_new<P>(
        vx, vy, vz, dp0, dp1, dp2, dp3, ia, ib, ic, V, Mx, My, Mz);

    return;
  }

  integrate_general_face_from_bits_new<P>(
      bits, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
}

RAJA_HOST_DEVICE
RAJA_INLINE void integrate_vertices_by_facebits_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type const vx[12],
    Real_type const vy[12],
    Real_type const vz[12],
    FaceBits8New const &fb,
    Real_type &V,
    Real_type &Mx,
    Real_type &My,
    Real_type &Mz) {
  V = Real_type(0.0);
  Mx = Real_type(0.0);
  My = Real_type(0.0);
  Mz = Real_type(0.0);

  integrate_face_from_bits_new<0>(
      fb.b0, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<1>(
      fb.b1, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<2>(
      fb.b2, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<3>(
      fb.b3, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<4>(
      fb.b4, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<5>(
      fb.b5, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<6>(
      fb.b6, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
  integrate_face_from_bits_new<7>(
      fb.b7, vx, vy, vz, dp0, dp1, dp2, dp3, V, Mx, My, Mz);
}

RAJA_HOST_DEVICE
RAJA_INLINE void process_donor_edge_target_plane_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const ax,
    Real_type const ay,
    Real_type const az,
    Real_type const bx,
    Real_type const by,
    Real_type const bz,
    Real_type const ca,
    Real_type const cb,
    Int_type const plane,
    unsigned char const edge_mask,
    Real_type const eps) {
  Real_type const peps = target_plane_eps_new(plane, eps);

  if ((ca < -peps && cb < -peps) ||
      (ca >  peps && cb >  peps)) {
    return;
  }

  Real_type const denom = ca - cb;
  if (hexhex_abs_new(denom) <= peps) {
    return;
  }

  Real_type const t = ca / denom;
  if (t < -eps || t > Real_type(1.0) + eps) {
    return;
  }

  Real_type const px = ax + t * (bx - ax);
  Real_type const py = ay + t * (by - ay);
  Real_type const pz = az + t * (bz - az);
  unsigned char const mask =
      static_cast<unsigned char>(edge_mask | (1u << plane));

  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      px, py, pz, mask, eps);
}

RAJA_HOST_DEVICE
RAJA_INLINE void process_donor_edge_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const ax,
    Real_type const ay,
    Real_type const az,
    Real_type const ah,
    Real_type const bx,
    Real_type const by,
    Real_type const bz,
    Real_type const bh,
    unsigned char const edge_mask,
    Real_type const eps) {
  process_donor_edge_target_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, ax, bx, 0, edge_mask, eps);
  process_donor_edge_target_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, ay, by, 1, edge_mask, eps);
  process_donor_edge_target_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, az, bz, 2, edge_mask, eps);
  process_donor_edge_target_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, ah, bh, 3, edge_mask, eps);
}

RAJA_HOST_DEVICE
RAJA_INLINE void process_target_edge_donor_plane_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const ax,
    Real_type const ay,
    Real_type const az,
    Real_type const bx,
    Real_type const by,
    Real_type const bz,
    Real_type const ca,
    Real_type const cb,
    HexHexPlaneNew const &plane,
    Int_type const p,
    unsigned char const edge_mask,
    Real_type const eps) {
  Real_type const peps = scaled_plane_eps_new(plane, eps);

  if ((ca < -peps && cb < -peps) ||
      (ca >  peps && cb >  peps)) {
    return;
  }

  Real_type const denom = ca - cb;
  if (hexhex_abs_new(denom) <= peps) {
    return;
  }

  Real_type const t = ca / denom;
  if (t < -eps || t > Real_type(1.0) + eps) {
    return;
  }

  Real_type const px = ax + t * (bx - ax);
  Real_type const py = ay + t * (by - ay);
  Real_type const pz = az + t * (bz - az);
  unsigned char const mask =
      static_cast<unsigned char>(edge_mask | (1u << (4 + p)));

  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      px, py, pz, mask, eps);
}

RAJA_HOST_DEVICE
RAJA_INLINE void process_target_edge_new(
    HexHexPlaneNew const &dp0,
    HexHexPlaneNew const &dp1,
    HexHexPlaneNew const &dp2,
    HexHexPlaneNew const &dp3,
    Real_type vx[12],
    Real_type vy[12],
    Real_type vz[12],
    FaceBits8New &facebits,
    Int_type &nv,
    Real_type const ax,
    Real_type const ay,
    Real_type const az,
    Real_type const bx,
    Real_type const by,
    Real_type const bz,
    Real_type const a0,
    Real_type const a1,
    Real_type const a2,
    Real_type const a3,
    Real_type const b0,
    Real_type const b1,
    Real_type const b2,
    Real_type const b3,
    unsigned char const edge_mask,
    Real_type const eps) {
  process_target_edge_donor_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, a0, b0, dp0, 0, edge_mask, eps);
  process_target_edge_donor_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, a1, b1, dp1, 1, edge_mask, eps);
  process_target_edge_donor_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, a2, b2, dp2, 2, edge_mask, eps);
  process_target_edge_donor_plane_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      ax, ay, az, bx, by, bz, a3, b3, dp3, 3, edge_mask, eps);
}

RAJA_HOST_DEVICE
RAJA_INLINE void intersect_tettet_edgeface_new(
    Real_type const x[4],
    Real_type const y[4],
    Real_type const z[4],
    Real_type &V,
    Real_type &Mx,
    Real_type &My,
    Real_type &Mz) {
  Real_type const eps = hexhex_tettet_scale_eps_new(x, y, z);

  Real_type const det_donor = hexhex_det3_new(
      x[1] - x[0], y[1] - y[0], z[1] - z[0],
      x[2] - x[0], y[2] - y[0], z[2] - z[0],
      x[3] - x[0], y[3] - y[0], z[3] - z[0]);
  Real_type const donor_sign =
      det_donor >= Real_type(0.0) ? Real_type(1.0) : Real_type(-1.0);

  if (hexhex_abs_new(det_donor) <= eps * eps * eps) {
    V = Real_type(0.0);
    Mx = Real_type(0.0);
    My = Real_type(0.0);
    Mz = Real_type(0.0);
    return;
  }

  Real_type const h0 = Real_type(1.0) - x[0] - y[0] - z[0];
  Real_type const h1 = Real_type(1.0) - x[1] - y[1] - z[1];
  Real_type const h2 = Real_type(1.0) - x[2] - y[2] - z[2];
  Real_type const h3 = Real_type(1.0) - x[3] - y[3] - z[3];

  if (hexhex_max4_new(x[0], x[1], x[2], x[3]) <= target_plane_eps_new(0, eps) ||
      hexhex_max4_new(y[0], y[1], y[2], y[3]) <= target_plane_eps_new(1, eps) ||
      hexhex_max4_new(z[0], z[1], z[2], z[3]) <= target_plane_eps_new(2, eps) ||
      hexhex_max4_new(h0, h1, h2, h3) <= target_plane_eps_new(3, eps)) {
    V = Real_type(0.0);
    Mx = Real_type(0.0);
    My = Real_type(0.0);
    Mz = Real_type(0.0);
    return;
  }

  if (x[0] >= -target_plane_eps_new(0, eps) &&
      x[1] >= -target_plane_eps_new(0, eps) &&
      x[2] >= -target_plane_eps_new(0, eps) &&
      x[3] >= -target_plane_eps_new(0, eps) &&
      y[0] >= -target_plane_eps_new(1, eps) &&
      y[1] >= -target_plane_eps_new(1, eps) &&
      y[2] >= -target_plane_eps_new(1, eps) &&
      y[3] >= -target_plane_eps_new(1, eps) &&
      z[0] >= -target_plane_eps_new(2, eps) &&
      z[1] >= -target_plane_eps_new(2, eps) &&
      z[2] >= -target_plane_eps_new(2, eps) &&
      z[3] >= -target_plane_eps_new(2, eps) &&
      h0 >= -target_plane_eps_new(3, eps) &&
      h1 >= -target_plane_eps_new(3, eps) &&
      h2 >= -target_plane_eps_new(3, eps) &&
      h3 >= -target_plane_eps_new(3, eps)) {
    signed_tet_moments_new(x, y, z, V, Mx, My, Mz);
    return;
  }

  HexHexPlaneNew dp0, dp1, dp2, dp3;
  make_donor_tet_planes_only_new(x, y, z, dp0, dp1, dp2, dp3);

  Real_type const d00 = dp0.d;
  Real_type const d01 = dp0.nx + dp0.d;
  Real_type const d02 = dp0.ny + dp0.d;
  Real_type const d03 = dp0.nz + dp0.d;
  Real_type const d10 = dp1.d;
  Real_type const d11 = dp1.nx + dp1.d;
  Real_type const d12 = dp1.ny + dp1.d;
  Real_type const d13 = dp1.nz + dp1.d;
  Real_type const d20 = dp2.d;
  Real_type const d21 = dp2.nx + dp2.d;
  Real_type const d22 = dp2.ny + dp2.d;
  Real_type const d23 = dp2.nz + dp2.d;
  Real_type const d30 = dp3.d;
  Real_type const d31 = dp3.nx + dp3.d;
  Real_type const d32 = dp3.ny + dp3.d;
  Real_type const d33 = dp3.nz + dp3.d;

  Real_type const de0 = scaled_plane_eps_new(dp0, eps);
  Real_type const de1 = scaled_plane_eps_new(dp1, eps);
  Real_type const de2 = scaled_plane_eps_new(dp2, eps);
  Real_type const de3 = scaled_plane_eps_new(dp3, eps);

  if (hexhex_max4_new(d00, d01, d02, d03) <= de0 ||
      hexhex_max4_new(d10, d11, d12, d13) <= de1 ||
      hexhex_max4_new(d20, d21, d22, d23) <= de2 ||
      hexhex_max4_new(d30, d31, d32, d33) <= de3) {
    V = Real_type(0.0);
    Mx = Real_type(0.0);
    My = Real_type(0.0);
    Mz = Real_type(0.0);
    return;
  }

  if (d00 >= -de0 && d01 >= -de0 && d02 >= -de0 && d03 >= -de0 &&
      d10 >= -de1 && d11 >= -de1 && d12 >= -de1 && d13 >= -de1 &&
      d20 >= -de2 && d21 >= -de2 && d22 >= -de2 && d23 >= -de2 &&
      d30 >= -de3 && d31 >= -de3 && d32 >= -de3 && d33 >= -de3) {
    V = donor_sign * Real_type(1.0 / 6.0);
    Mx = donor_sign * Real_type(1.0 / 24.0);
    My = donor_sign * Real_type(1.0 / 24.0);
    Mz = donor_sign * Real_type(1.0 / 24.0);
    return;
  }

  Real_type vx[12], vy[12], vz[12];
  FaceBits8New facebits;
  facebits_clear_new(facebits);
  Int_type nv = 0;

  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[0], y[0], z[0], donor_vertex_mask_new(0), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[1], y[1], z[1], donor_vertex_mask_new(1), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[2], y[2], z[2], donor_vertex_mask_new(2), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[3], y[3], z[3], donor_vertex_mask_new(3), eps);

  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(0.0), Real_type(0.0),
      target_vertex_mask_new(0), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(1.0), Real_type(0.0), Real_type(0.0),
      target_vertex_mask_new(1), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(1.0), Real_type(0.0),
      target_vertex_mask_new(2), eps);
  add_candidate_bits_if_inside_all_planes_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(0.0), Real_type(1.0),
      target_vertex_mask_new(3), eps);

  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[0], y[0], z[0], h0, x[1], y[1], z[1], h1,
      static_cast<unsigned char>(donor_vertex_mask_new(0) & donor_vertex_mask_new(1)), eps);
  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[0], y[0], z[0], h0, x[2], y[2], z[2], h2,
      static_cast<unsigned char>(donor_vertex_mask_new(0) & donor_vertex_mask_new(2)), eps);
  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[0], y[0], z[0], h0, x[3], y[3], z[3], h3,
      static_cast<unsigned char>(donor_vertex_mask_new(0) & donor_vertex_mask_new(3)), eps);
  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[1], y[1], z[1], h1, x[2], y[2], z[2], h2,
      static_cast<unsigned char>(donor_vertex_mask_new(1) & donor_vertex_mask_new(2)), eps);
  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[1], y[1], z[1], h1, x[3], y[3], z[3], h3,
      static_cast<unsigned char>(donor_vertex_mask_new(1) & donor_vertex_mask_new(3)), eps);
  process_donor_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      x[2], y[2], z[2], h2, x[3], y[3], z[3], h3,
      static_cast<unsigned char>(donor_vertex_mask_new(2) & donor_vertex_mask_new(3)), eps);

  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(0.0), Real_type(0.0),
      Real_type(1.0), Real_type(0.0), Real_type(0.0),
      d00, d10, d20, d30, d01, d11, d21, d31,
      static_cast<unsigned char>(target_vertex_mask_new(0) & target_vertex_mask_new(1)), eps);
  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(0.0), Real_type(0.0),
      Real_type(0.0), Real_type(1.0), Real_type(0.0),
      d00, d10, d20, d30, d02, d12, d22, d32,
      static_cast<unsigned char>(target_vertex_mask_new(0) & target_vertex_mask_new(2)), eps);
  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(0.0), Real_type(0.0),
      Real_type(0.0), Real_type(0.0), Real_type(1.0),
      d00, d10, d20, d30, d03, d13, d23, d33,
      static_cast<unsigned char>(target_vertex_mask_new(0) & target_vertex_mask_new(3)), eps);
  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(1.0), Real_type(0.0), Real_type(0.0),
      Real_type(0.0), Real_type(1.0), Real_type(0.0),
      d01, d11, d21, d31, d02, d12, d22, d32,
      static_cast<unsigned char>(target_vertex_mask_new(1) & target_vertex_mask_new(2)), eps);
  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(1.0), Real_type(0.0), Real_type(0.0),
      Real_type(0.0), Real_type(0.0), Real_type(1.0),
      d01, d11, d21, d31, d03, d13, d23, d33,
      static_cast<unsigned char>(target_vertex_mask_new(1) & target_vertex_mask_new(3)), eps);
  process_target_edge_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, nv,
      Real_type(0.0), Real_type(1.0), Real_type(0.0),
      Real_type(0.0), Real_type(0.0), Real_type(1.0),
      d02, d12, d22, d32, d03, d13, d23, d33,
      static_cast<unsigned char>(target_vertex_mask_new(2) & target_vertex_mask_new(3)), eps);

  if (nv < 4) {
    V = Real_type(0.0);
    Mx = Real_type(0.0);
    My = Real_type(0.0);
    Mz = Real_type(0.0);
    return;
  }

  integrate_vertices_by_facebits_new(
      dp0, dp1, dp2, dp3, vx, vy, vz, facebits, V, Mx, My, Mz);

  V *= donor_sign;
  Mx *= donor_sign;
  My *= donor_sign;
  Mz *= donor_sign;
}

constexpr Int_type hexhex_tettet_max_verts = 16;
constexpr Int_type hexhex_tettet_max_faces = 12;
constexpr Int_type hexhex_tettet_max_face_verts = 8;

struct HexHexRefFaceNew {
  Int_type n;
  Int_type v[hexhex_tettet_max_face_verts];
};

struct HexHexRefPolyNew {
  Int_type nv;
  Int_type nf;

  Real_type x[hexhex_tettet_max_verts];
  Real_type y[hexhex_tettet_max_verts];
  Real_type z[hexhex_tettet_max_verts];

  HexHexRefFaceNew f[hexhex_tettet_max_faces];
};

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_swap_real_new(Real_type &a, Real_type &b) {
  Real_type const t = a;
  a = b;
  b = t;
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_tet_det_new(Real_type const x0,
                                         Real_type const y0,
                                         Real_type const z0,
                                         Real_type const x1,
                                         Real_type const y1,
                                         Real_type const z1,
                                         Real_type const x2,
                                         Real_type const y2,
                                         Real_type const z2,
                                         Real_type const x3,
                                         Real_type const y3,
                                         Real_type const z3) {
  Real_type const ax = x1 - x0;
  Real_type const ay = y1 - y0;
  Real_type const az = z1 - z0;
  Real_type const bx = x2 - x0;
  Real_type const by = y2 - y0;
  Real_type const bz = z2 - z0;
  Real_type const cx = x3 - x0;
  Real_type const cy = y3 - y0;
  Real_type const cz = z3 - z0;

  return ax * (by * cz - bz * cy) -
         ay * (bx * cz - bz * cx) +
         az * (bx * cy - by * cx);
}

RAJA_HOST_DEVICE
RAJA_INLINE Real_type hexhex_ref_plane_value_new(HexHexRefPolyNew const &p,
                                                 HexHexPackedPlaneNew const plane,
                                                 Int_type const v) {
  switch (plane) {
  case HexHexPackedPlaneNew::X:
    return p.x[v];
  case HexHexPackedPlaneNew::Y:
    return p.y[v];
  case HexHexPackedPlaneNew::Z:
    return p.z[v];
  case HexHexPackedPlaneNew::H:
    return Real_type(1.0) - p.x[v] - p.y[v] - p.z[v];
  default:
    return Real_type(0.0);
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_ref_poly_add_vertex_new(HexHexRefPolyNew &p,
                                                    Real_type const x,
                                                    Real_type const y,
                                                    Real_type const z) {
  if (p.nv >= hexhex_tettet_max_verts) {
    return -1;
  }

  Int_type const v = p.nv++;
  p.x[v] = x;
  p.y[v] = y;
  p.z[v] = z;
  return v;
}

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_ref_poly_add_unique_id_new(Int_type ids[],
                                                   Int_type &nids,
                                                   Int_type const id,
                                                   Int_type const max_ids) {
  if (id < 0) {
    return;
  }

  for (Int_type i = 0; i < nids; ++i) {
    if (ids[i] == id) {
      return;
    }
  }

  if (nids < max_ids) {
    ids[nids++] = id;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE Int_type hexhex_ref_poly_intersection_new(
    HexHexRefPolyNew &p,
    Int_type edge_lo[],
    Int_type edge_hi[],
    Int_type edge_v[],
    Int_type &nedge,
    Int_type const va,
    Int_type const vb,
    Real_type const ca,
    Real_type const cb) {
  Int_type const elo = va < vb ? va : vb;
  Int_type const ehi = va < vb ? vb : va;

  for (Int_type i = 0; i < nedge; ++i) {
    if (edge_lo[i] == elo && edge_hi[i] == ehi) {
      return edge_v[i];
    }
  }

  Real_type const t = ca / (ca - cb);
  Real_type const omt = Real_type(1.0) - t;
  Int_type const v = hexhex_ref_poly_add_vertex_new(
      p, p.x[va] * omt + p.x[vb] * t,
         p.y[va] * omt + p.y[vb] * t,
         p.z[va] * omt + p.z[vb] * t);

  if (v >= 0 && nedge < hexhex_tettet_max_verts) {
    edge_lo[nedge] = elo;
    edge_hi[nedge] = ehi;
    edge_v[nedge] = v;
    ++nedge;
  }

  return v;
}

RAJA_HOST_DEVICE
RAJA_INLINE void make_ref_tet_poly_new(Real_type const x[4],
                                       Real_type const y[4],
                                       Real_type const z[4],
                                       HexHexRefPolyNew &p) {
  p.nv = 4;
  p.nf = 4;

  for (Int_type i = 0; i < 4; ++i) {
    p.x[i] = x[i];
    p.y[i] = y[i];
    p.z[i] = z[i];
  }

  p.f[0].n = 3;
  p.f[0].v[0] = 0;
  p.f[0].v[1] = 3;
  p.f[0].v[2] = 2;

  p.f[1].n = 3;
  p.f[1].v[0] = 0;
  p.f[1].v[1] = 1;
  p.f[1].v[2] = 3;

  p.f[2].n = 3;
  p.f[2].v[0] = 0;
  p.f[2].v[1] = 2;
  p.f[2].v[2] = 1;

  p.f[3].n = 3;
  p.f[3].v[0] = 1;
  p.f[3].v[1] = 2;
  p.f[3].v[2] = 3;
}

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_ref_cap_proj_new(HexHexRefPolyNew const &p,
                                         HexHexPackedPlaneNew const plane,
                                         Int_type const v,
                                         Real_type &a,
                                         Real_type &b) {
  switch (plane) {
  case HexHexPackedPlaneNew::X:
    a = p.z[v];
    b = p.y[v];
    break;
  case HexHexPackedPlaneNew::Y:
    a = p.x[v];
    b = p.z[v];
    break;
  case HexHexPackedPlaneNew::Z:
    a = p.y[v];
    b = p.x[v];
    break;
  case HexHexPackedPlaneNew::H:
    a = p.x[v] - p.y[v];
    b = p.x[v] + p.y[v] - Real_type(2.0) * p.z[v];
    break;
  default:
    a = Real_type(0.0);
    b = Real_type(0.0);
    break;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE bool hexhex_ref_angle_less_new(Real_type const ax,
                                           Real_type const ay,
                                           Real_type const bx,
                                           Real_type const by) {
  bool const ah = (ay > Real_type(0.0)) ||
                  (ay == Real_type(0.0) && ax >= Real_type(0.0));
  bool const bh = (by > Real_type(0.0)) ||
                  (by == Real_type(0.0) && bx >= Real_type(0.0));

  if (ah != bh) {
    return ah;
  }

  Real_type const cross = ax * by - ay * bx;
  if (cross != Real_type(0.0)) {
    return cross > Real_type(0.0);
  }

  return ax * ax + ay * ay < bx * bx + by * by;
}

RAJA_HOST_DEVICE
RAJA_INLINE void hexhex_ref_sort_cap_new(HexHexRefPolyNew const &p,
                                         HexHexPackedPlaneNew const plane,
                                         Int_type ids[],
                                         Int_type const nids) {
  Real_type ca = Real_type(0.0);
  Real_type cb = Real_type(0.0);

  for (Int_type i = 0; i < nids; ++i) {
    Real_type a, b;
    hexhex_ref_cap_proj_new(p, plane, ids[i], a, b);
    ca += a;
    cb += b;
  }

  if (nids > 0) {
    ca /= static_cast<Real_type>(nids);
    cb /= static_cast<Real_type>(nids);
  }

  for (Int_type i = 1; i < nids; ++i) {
    Int_type const id = ids[i];
    Real_type ida, idb;
    hexhex_ref_cap_proj_new(p, plane, id, ida, idb);
    ida -= ca;
    idb -= cb;

    Int_type j = i - 1;
    for (; j >= 0; --j) {
      Real_type ja, jb;
      hexhex_ref_cap_proj_new(p, plane, ids[j], ja, jb);
      ja -= ca;
      jb -= cb;
      if (!hexhex_ref_angle_less_new(ida, idb, ja, jb)) {
        break;
      }
      ids[j + 1] = ids[j];
    }
    ids[j + 1] = id;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void clip_ref_poly_ge_0_new(HexHexRefPolyNew &p,
                                        HexHexPackedPlaneNew const plane) {
  if (p.nf <= 0) {
    return;
  }

  HexHexRefFaceNew new_faces[hexhex_tettet_max_faces];
  Int_type nnew_faces = 0;

  Int_type cap_ids[hexhex_tettet_max_verts];
  Int_type ncap = 0;

  Int_type edge_lo[hexhex_tettet_max_verts];
  Int_type edge_hi[hexhex_tettet_max_verts];
  Int_type edge_v[hexhex_tettet_max_verts];
  Int_type nedge = 0;
  bool any_crossing = false;

  Int_type const old_nf = p.nf;

  for (Int_type iface = 0; iface < old_nf; ++iface) {
    HexHexRefFaceNew const face = p.f[iface];
    Int_type out[hexhex_tettet_max_face_verts];
    Int_type nout = 0;

    Int_type vprev = face.v[face.n - 1];
    Real_type cprev = hexhex_ref_plane_value_new(p, plane, vprev);
    bool inprev = cprev >= Real_type(0.0);

    for (Int_type i = 0; i < face.n; ++i) {
      Int_type const vcur = face.v[i];
      Real_type const ccur = hexhex_ref_plane_value_new(p, plane, vcur);
      bool const incur = ccur >= Real_type(0.0);

      if (incur) {
        if (!inprev && ccur > Real_type(0.0)) {
          Int_type const vi = hexhex_ref_poly_intersection_new(
              p, edge_lo, edge_hi, edge_v, nedge, vprev, vcur, cprev, ccur);
          if (vi >= 0 && nout < hexhex_tettet_max_face_verts) {
            out[nout++] = vi;
          }
          hexhex_ref_poly_add_unique_id_new(
              cap_ids, ncap, vi, hexhex_tettet_max_verts);
          any_crossing = true;
        }

        if (nout < hexhex_tettet_max_face_verts) {
          out[nout++] = vcur;
        }
        if (ccur == Real_type(0.0)) {
          hexhex_ref_poly_add_unique_id_new(
              cap_ids, ncap, vcur, hexhex_tettet_max_verts);
        }
      } else if (inprev) {
        if (cprev > Real_type(0.0)) {
          Int_type const vi = hexhex_ref_poly_intersection_new(
              p, edge_lo, edge_hi, edge_v, nedge, vprev, vcur, cprev, ccur);
          if (vi >= 0 && nout < hexhex_tettet_max_face_verts) {
            out[nout++] = vi;
          }
          hexhex_ref_poly_add_unique_id_new(
              cap_ids, ncap, vi, hexhex_tettet_max_verts);
          any_crossing = true;
        } else {
          hexhex_ref_poly_add_unique_id_new(
              cap_ids, ncap, vprev, hexhex_tettet_max_verts);
        }
      }

      vprev = vcur;
      cprev = ccur;
      inprev = incur;
    }

    if (nout >= 3 && nnew_faces < hexhex_tettet_max_faces) {
      new_faces[nnew_faces].n = nout;
      for (Int_type i = 0; i < nout; ++i) {
        new_faces[nnew_faces].v[i] = out[i];
      }
      ++nnew_faces;
    }
  }

  p.nf = nnew_faces;
  for (Int_type iface = 0; iface < nnew_faces; ++iface) {
    p.f[iface] = new_faces[iface];
  }

  if (p.nf == 0) {
    p.nv = 0;
    return;
  }

  if (any_crossing &&
      ncap >= 3 &&
      ncap <= hexhex_tettet_max_face_verts &&
      p.nf < hexhex_tettet_max_faces) {
    hexhex_ref_sort_cap_new(p, plane, cap_ids, ncap);

    p.f[p.nf].n = ncap;
    for (Int_type i = 0; i < ncap; ++i) {
      p.f[p.nf].v[i] = cap_ids[i];
    }
    ++p.nf;
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void integrate_ref_poly_new(HexHexRefPolyNew const &p,
                                        Real_type &vv,
                                        Real_type &vx,
                                        Real_type &vy,
                                        Real_type &vz) {
  vv = Real_type(0.0);
  vx = Real_type(0.0);
  vy = Real_type(0.0);
  vz = Real_type(0.0);

  for (Int_type iface = 0; iface < p.nf; ++iface) {
    HexHexRefFaceNew const &face = p.f[iface];

    for (Int_type k = 1; k + 1 < face.n; ++k) {
      Int_type const ia = face.v[0];
      Int_type const ib = face.v[k];
      Int_type const ic = face.v[k + 1];

      Real_type const ax = p.x[ia];
      Real_type const ay = p.y[ia];
      Real_type const az = p.z[ia];
      Real_type const bx = p.x[ib];
      Real_type const by = p.y[ib];
      Real_type const bz = p.z[ib];
      Real_type const cx = p.x[ic];
      Real_type const cy = p.y[ic];
      Real_type const cz = p.z[ic];

      Real_type const det =
          ax * (by * cz - bz * cy) -
          ay * (bx * cz - bz * cx) +
          az * (bx * cy - by * cx);
      Real_type const vol = det / Real_type(6.0);

      vv += vol;
      vx += vol * (ax + bx + cx) / Real_type(4.0);
      vy += vol * (ay + by + cy) / Real_type(4.0);
      vz += vol * (az + bz + cz) / Real_type(4.0);
    }
  }
}

RAJA_HOST_DEVICE
RAJA_INLINE void stage2_compute_tettet_task_fast(
    Int_type const task,
    Real_type const sd[24],
    HexHexTargetTetMapNew const smap[6],
    Real_type &vv,
    Real_type &vx,
    Real_type &vy,
    Real_type &vz) {
  Int_type const ttet = task / 6;
  Int_type const dtet = task - 6 * ttet;
  HexHexTargetTetMapNew const &m = smap[ttet];

  Real_type x[4], y[4], z[4];
  Int_type n0, n1, n2, n3;
  donor_tet_nodes_fast(dtet, n0, n1, n2, n3);

  target_map_to_ref_new(
      m, sd[n0], sd[8 + n0], sd[16 + n0], x[0], y[0], z[0]);
  target_map_to_ref_new(
      m, sd[n1], sd[8 + n1], sd[16 + n1], x[1], y[1], z[1]);
  target_map_to_ref_new(
      m, sd[n2], sd[8 + n2], sd[16 + n2], x[2], y[2], z[2]);
  target_map_to_ref_new(
      m, sd[n3], sd[8 + n3], sd[16 + n3], x[3], y[3], z[3]);

  Real_type vref = Real_type(0.0);
  Real_type mxref = Real_type(0.0);
  Real_type myref = Real_type(0.0);
  Real_type mzref = Real_type(0.0);

  intersect_tettet_edgeface_new(x, y, z, vref, mxref, myref, mzref);

  vx += m.det * (m.x0  * vref +
                 m.e1x * mxref +
                 m.e2x * myref +
                 m.e3x * mzref);

  vy += m.det * (m.y0  * vref +
                 m.e1y * mxref +
                 m.e2y * myref +
                 m.e3y * mzref);

  vz += m.det * (m.z0  * vref +
                 m.e1z * mxref +
                 m.e2z * myref +
                 m.e3z * mzref);

  vv += m.det * vref;
}

RAJA_HOST_DEVICE
RAJA_INLINE void cuda_intsc_tri_targetmap_new(
    Real_type const (&xdt)[3], Real_type const (&ydt)[3],
    Real_type const (&zdt)[3], HexHexTargetTetMapNew const &m,
    Real_type &vv_thr, Real_type &vx_thr, Real_type &vy_thr,
    Real_type &vz_thr) {
  Real_type vv = 0.0;
  Real_type vx = 0.0;
  Real_type vy = 0.0;
  Real_type vz = 0.0;

  Real_type xa[hexhex_new_max_poly_vertices];
  Real_type ya[hexhex_new_max_poly_vertices];
  Real_type za[hexhex_new_max_poly_vertices];

  target_map_to_ref_new(m, xdt[0], ydt[0], zdt[0], xa[0], ya[0], za[0]);
  target_map_to_ref_new(m, xdt[1], ydt[1], zdt[1], xa[1], ya[1], za[1]);
  target_map_to_ref_new(m, xdt[2], ydt[2], zdt[2], xa[2], ya[2], za[2]);

  Real_type const h20 = Real_type(1.0) - xa[0] - ya[0];
  Real_type const h21 = Real_type(1.0) - xa[1] - ya[1];
  Real_type const h22 = Real_type(1.0) - xa[2] - ya[2];

  if ((h20 < Real_type(0.0) && h21 < Real_type(0.0) && h22 < Real_type(0.0)) ||
      (xa[0] < Real_type(0.0) && xa[1] < Real_type(0.0) &&
       xa[2] < Real_type(0.0)) ||
      (ya[0] < Real_type(0.0) && ya[1] < Real_type(0.0) &&
       ya[2] < Real_type(0.0)) ||
      (za[0] < Real_type(0.0) && za[1] < Real_type(0.0) &&
       za[2] < Real_type(0.0))) {
    return;
  }

  unsigned long long next_pack = HEXHEX_NEW_NEXT_INIT;
  Int_type first = 0;
  Int_type avail = 3;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::H2>(xa, ya, za, first,
                                                         avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::X>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::Y>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::Z>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  Int_type const first_saved = first;
  Int_type const avail_saved = avail;
  unsigned long long const next_saved = next_pack;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::H>(xa, ya, za, first,
                                                        avail, next_pack);
  cuda_hex_volpolyh_1poly_packed_new(xa, ya, za, first, next_pack, vv, vx, vy,
                                     vz);

  first = first_saved;
  avail = avail_saved;
  next_pack = next_saved;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::NEG_H>(xa, ya, za, first,
                                                            avail, next_pack);

#pragma unroll 1
  for (Int_type j = first; j >= 0; j = hexhex_new_next_get(next_pack, j)) {
    za[j] = Real_type(1.0) - xa[j] - ya[j];
  }

  cuda_hex_volpolyh_1poly_packed_new(xa, ya, za, first, next_pack, vv, vx, vy,
                                     vz);

  vv *= 0.16666666666666667;
  vx *= 0.041666666666666667;
  vy *= 0.041666666666666667;
  vz *= 0.041666666666666667;

  vx_thr += m.det * (m.x0 * vv + m.e1x * vx + m.e2x * vy + m.e3x * vz);
  vy_thr += m.det * (m.y0 * vv + m.e1y * vx + m.e2y * vy + m.e3y * vz);
  vz_thr += m.det * (m.z0 * vv + m.e1z * vx + m.e2z * vy + m.e3z * vz);

  vv_thr += m.det * vv;
}

RAJA_HOST_DEVICE
RAJA_INLINE void cuda_intsc_tri_tet_new(
    Real_type const (&xdt)[3], // donor triangle coordinates
    Real_type const (&ydt)[3], Real_type const (&zdt)[3],
    Real_type (&xtt)[4], // target tet coordinates (modified here)
    Real_type (&ytt)[4], Real_type (&ztt)[4],
    Real_type &vv_thr, // volume contribution for this triangle-tet
    Real_type &vx_thr, // x moment contribution for this triangle-tet
    Real_type &vy_thr, // y moment contribution for this triangle-tet
    Real_type &vz_thr) // z moment contribution for this triangle-tet
{
  Real_type det, deti;
  Real_type xa0, xa1, xa2;
  Real_type ya0, ya1, ya2;
  Real_type za0, za1, za2;

  Real_type vv = 0.0, vx = 0.0, vy = 0.0, vz = 0.0; // volume, moments.

  xtt[1] -= xtt[0];
  xtt[2] -= xtt[0];
  xtt[3] -= xtt[0];
  ytt[1] -= ytt[0];
  ytt[2] -= ytt[0];
  ytt[3] -= ytt[0];
  ztt[1] -= ztt[0];
  ztt[2] -= ztt[0];
  ztt[3] -= ztt[0];

  det = xtt[1] * ytt[2] * ztt[3] - xtt[1] * ytt[3] * ztt[2] +
        xtt[2] * ytt[3] * ztt[1] - xtt[2] * ytt[1] * ztt[3] +
        xtt[3] * ytt[1] * ztt[2] - xtt[3] * ytt[2] * ztt[1];
  deti = det / (det * det + 1.0e-100);

  // Cross products.
  Real_type cyz = ytt[2] * ztt[3] - ztt[2] * ytt[3];
  Real_type czx = ztt[2] * xtt[3] - xtt[2] * ztt[3];
  Real_type cxy = xtt[2] * ytt[3] - ytt[2] * xtt[3];

  //   Coordinates of the facet in the transformed frame.
  xa0 = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
        (zdt[0] - ztt[0]) * cxy;
  xa1 = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
        (zdt[1] - ztt[0]) * cxy;
  xa2 = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
        (zdt[2] - ztt[0]) * cxy;

  cyz = ytt[3] * ztt[1] - ztt[3] * ytt[1];
  czx = ztt[3] * xtt[1] - xtt[3] * ztt[1];
  cxy = xtt[3] * ytt[1] - ytt[3] * xtt[1];

  ya0 = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
        (zdt[0] - ztt[0]) * cxy;
  ya1 = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
        (zdt[1] - ztt[0]) * cxy;
  ya2 = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
        (zdt[2] - ztt[0]) * cxy;

  cyz = ytt[1] * ztt[2] - ztt[1] * ytt[2];
  czx = ztt[1] * xtt[2] - xtt[1] * ztt[2];
  cxy = xtt[1] * ytt[2] - ytt[1] * xtt[2];

  za0 = (xdt[0] - xtt[0]) * cyz + (ydt[0] - ytt[0]) * czx +
        (zdt[0] - ztt[0]) * cxy;
  za1 = (xdt[1] - xtt[0]) * cyz + (ydt[1] - ytt[0]) * czx +
        (zdt[1] - ztt[0]) * cxy;
  za2 = (xdt[2] - xtt[0]) * cyz + (ydt[2] - ytt[0]) * czx +
        (zdt[2] - ztt[0]) * cxy;

  xa0 *= deti;
  xa1 *= deti;
  xa2 *= deti;
  ya0 *= deti;
  ya1 *= deti;
  ya2 *= deti;
  za0 *= deti;
  za1 *= deti;
  za2 *= deti;

  Real_type xa[hexhex_new_max_poly_vertices];
  Real_type ya[hexhex_new_max_poly_vertices];
  Real_type za[hexhex_new_max_poly_vertices];

  xa[0] = xa0;
  xa[1] = xa1;
  xa[2] = xa2;

  ya[0] = ya0;
  ya[1] = ya1;
  ya[2] = ya2;

  za[0] = za0;
  za[1] = za1;
  za[2] = za2;

  // NOTE: early exit to see if triangle and plane are clipping

  Real_type const h20 = Real_type(1.0) - xa[0] - ya[0];
  Real_type const h21 = Real_type(1.0) - xa[1] - ya[1];
  Real_type const h22 = Real_type(1.0) - xa[2] - ya[2];

  if ((h20 < Real_type(0.0) && h21 < Real_type(0.0) && h22 < Real_type(0.0)) ||
      (xa[0] < Real_type(0.0) && xa[1] < Real_type(0.0) &&
       xa[2] < Real_type(0.0)) ||
      (ya[0] < Real_type(0.0) && ya[1] < Real_type(0.0) &&
       ya[2] < Real_type(0.0)) ||
      (za[0] < Real_type(0.0) && za[1] < Real_type(0.0) &&
       za[2] < Real_type(0.0))) {
    return;
  }

  unsigned long long next_pack = HEXHEX_NEW_NEXT_INIT;
  Int_type first = 0;
  Int_type avail = 3;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::H2>(xa, ya, za, first,
                                                         avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::X>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::Y>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::Z>(xa, ya, za, first,
                                                        avail, next_pack);
  if (first < 0) {
    return;
  }

  Int_type const first_saved = first;
  Int_type const avail_saved = avail;
  unsigned long long const next_saved = next_pack;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::H>(xa, ya, za, first,
                                                        avail, next_pack);
  cuda_hex_volpolyh_1poly_packed_new(xa, ya, za, first, next_pack, vv, vx, vy,
                                     vz);

  first = first_saved;
  avail = avail_saved;
  next_pack = next_saved;

  clip_polygon_ge_0_packed_new<HexHexPackedPlaneNew::NEG_H>(xa, ya, za, first,
                                                            avail, next_pack);

#pragma unroll 1
  for (Int_type j = first; j >= 0; j = hexhex_new_next_get(next_pack, j)) {
    za[j] = Real_type(1.0) - xa[j] - ya[j];
  }

  cuda_hex_volpolyh_1poly_packed_new(xa, ya, za, first, next_pack, vv, vx, vy,
                                     vz);

  //  Volume, moments of the intersection in the unit tet frame.
  vv *= 0.16666666666666667;
  vx *= 0.041666666666666667;
  vy *= 0.041666666666666667;
  vz *= 0.041666666666666667;

  //   Transform moments to the physical frame.
  vx_thr += det * (xtt[0] * vv + xtt[1] * vx + xtt[2] * vy + xtt[3] * vz);
  vy_thr += det * (ytt[0] * vv + ytt[1] * vx + ytt[2] * vy + ytt[3] * vz);
  vz_thr += det * (ztt[0] * vv + ztt[1] * vx + ztt[2] * vy + ztt[3] * vz);

  //   Transform intersection volume to the physical frame.
  vv_thr += det * vv;
}

RAJA_HOST_DEVICE
RAJA_INLINE void make_donor_facet_new(Real_const_ptr xds,
                                      Int_type const dfacet,
                                      Real_type (&xdt)[3],
                                      Real_type (&ydt)[3],
                                      Real_type (&zdt)[3]) {
  Real_const_ptr yds = xds + 8;
  Real_const_ptr zds = yds + 8;

  Int_type const n_dfacets = 12;
  Int_type const cyc_nod[7] = {1, 5, 4, 6, 2, 3, 1};

  Int_type v0, v1, v2;
  if (dfacet < 6) {
    v0 = 0;
    v1 = cyc_nod[dfacet];
    v2 = cyc_nod[dfacet + 1];
  } else {
    v0 = 7;
    v1 = cyc_nod[n_dfacets - dfacet];
    v2 = cyc_nod[n_dfacets - dfacet - 1];
  }

  xdt[0] = xds[v0];
  xdt[1] = xds[v1];
  xdt[2] = xds[v2];

  ydt[0] = yds[v0];
  ydt[1] = yds[v1];
  ydt[2] = yds[v2];

  zdt[0] = zds[v0];
  zdt[1] = zds[v1];
  zdt[2] = zds[v2];
}

//  Compute the contribution of a donor triangle and a target tet
//         to intersection between hex subzones.
//   Each subzone is twelve triangular facets (six tets).
//
RAJA_HOST_DEVICE
RAJA_INLINE void hex_intsc_subz_new(
    Real_const_ptr xds,    //  [24] donor subzone coords
    Real_const_ptr xts,    //  [24] target subzone coords
    Int_type const dfacet, // which donor facet
    Int_type const ttet,   // which target tet
    Real_type &vv_thr,     // volume contribution for this triangle-tet
    Real_type &vx_thr,     // x moment contribution for this triangle-tet
    Real_type &vy_thr,     // y moment contribution for this triangle-tet
    Real_type &vz_thr)     // z moment contribution for this triangle-tet
{
  Real_const_ptr yts = xts + 8;
  Real_const_ptr zts = yts + 8;

  vv_thr = 0.0;
  vx_thr = 0.0;
  vy_thr = 0.0;
  vz_thr = 0.0;

  //  coordinates of the donor triangle
  Real_type xdt[3], ydt[3], zdt[3];
  make_donor_facet_new(xds, dfacet, xdt, ydt, zdt);

  //   Set up the target tet and do the intersections.

  Real_type xtt[4], ytt[4], ztt[4];

  xtt[0] = xts[0];
  ytt[0] = yts[0];
  ztt[0] = zts[0];

  //  subzone vertices that form the cycle for tets.
  Int_type v1 = hexhex_new_vert_cyc(ttet);
  xtt[1] = xts[v1];
  ytt[1] = yts[v1];
  ztt[1] = zts[v1];
  Int_type v2 = hexhex_new_vert_cyc_next(ttet);
  xtt[2] = xts[v2];
  ytt[2] = yts[v2];
  ztt[2] = zts[v2];
  xtt[3] = xts[7];
  ytt[3] = yts[7];
  ztt[3] = zts[7];

  cuda_intsc_tri_tet_new(xdt, ydt, zdt, xtt, ytt, ztt, vv_thr, vx_thr, vy_thr,
                         vz_thr);
}

} // end namespace rajaperf

#define INTSC_HEXHEX_NEW_BODY_SEQ                                              \
  Index_type ipair = ith / hexhex_new_tri_per_pair;                            \
  Int_type dfacet = (ith / hexhex_new_n_tsz_tets) % hexhex_new_n_dsz_tris;     \
  Int_type ttet = ith % hexhex_new_n_tsz_tets;                                 \
  Index_type pair_base_thr = ipair * hexhex_new_tri_per_pair;                  \
  Index_type blk_base = blk * blksize;                                         \
  Real_type vv_lo = 0.0, vx_lo = 0.0, vy_lo = 0.0, vz_lo = 0.0;                \
  Real_type vv_hi = 0.0, vx_hi = 0.0, vy_hi = 0.0, vz_hi = 0.0;                \
  if (ipair < nisc_stage) {                                                    \
    Real_const_ptr xds = dsubz + 24 * ipair;                                   \
    Real_const_ptr xts = tsubz + 24 * ipair;                                   \
    hex_intsc_subz_new(xds, xts, dfacet, ttet, vv_lo, vx_lo, vy_lo, vz_lo);    \
  }                                                                            \
  if (pair_base_thr > blk_base) {                                              \
    vv_hi = vv_lo;                                                             \
    vx_hi = vx_lo;                                                             \
    vy_hi = vy_lo;                                                             \
    vz_hi = vz_lo;                                                             \
    vv_lo = 0.0;                                                               \
    vx_lo = 0.0;                                                               \
    vy_lo = 0.0;                                                               \
    vz_lo = 0.0;                                                               \
  }

// thridx is threadIdx.x

#define INTSC_HEXHEX_NEW_BODY                                                  \
  INTSC_HEXHEX_NEW_BODY_SEQ                                                    \
                                                                               \
  __syncthreads();                                                             \
  for (Index_type k = 1; k < RAJAPERF_HEXHEX_WARPSIZE; k *= 2) {               \
    vv_hi += RAJAPERF_HEXHEX_shfl_xor(vv_hi, k);                               \
    vx_hi += RAJAPERF_HEXHEX_shfl_xor(vx_hi, k);                               \
    vy_hi += RAJAPERF_HEXHEX_shfl_xor(vy_hi, k);                               \
    vz_hi += RAJAPERF_HEXHEX_shfl_xor(vz_hi, k);                               \
    vv_lo += RAJAPERF_HEXHEX_shfl_xor(vv_lo, k);                               \
    vx_lo += RAJAPERF_HEXHEX_shfl_xor(vx_lo, k);                               \
    vy_lo += RAJAPERF_HEXHEX_shfl_xor(vy_lo, k);                               \
    vz_lo += RAJAPERF_HEXHEX_shfl_xor(vz_lo, k);                               \
  }                                                                            \
  Int_type const nwarps = blksize / RAJAPERF_HEXHEX_WARPSIZE;                  \
  Int_type k = thridx / RAJAPERF_HEXHEX_WARPSIZE;                              \
  if (thridx == k * RAJAPERF_HEXHEX_WARPSIZE) {                                \
    vv_reduce[k + 0 * hexhex_new_max_warps_per_block] = vv_lo;                 \
    vv_reduce[k + 1 * hexhex_new_max_warps_per_block] = vx_lo;                 \
    vv_reduce[k + 2 * hexhex_new_max_warps_per_block] = vy_lo;                 \
    vv_reduce[k + 3 * hexhex_new_max_warps_per_block] = vz_lo;                 \
    vv_reduce[k + 4 * hexhex_new_max_warps_per_block] = vv_hi;                 \
    vv_reduce[k + 5 * hexhex_new_max_warps_per_block] = vx_hi;                 \
    vv_reduce[k + 6 * hexhex_new_max_warps_per_block] = vy_hi;                 \
    vv_reduce[k + 7 * hexhex_new_max_warps_per_block] = vz_hi;                 \
  }                                                                            \
  __syncthreads();                                                             \
  if (thridx < hexhex_new_max_pairs_per_block * hexhex_new_nvals_per_pair) {   \
    for (Index_type k = 1; k < nwarps; ++k) {                                  \
      vv_reduce[hexhex_new_max_warps_per_block * thridx] +=                    \
          vv_reduce[hexhex_new_max_warps_per_block * thridx + k];              \
    }                                                                          \
    vv_int_p[thridx] = vv_reduce[hexhex_new_max_warps_per_block * thridx];     \
  }

#define INTSC_HEXHEX_NEW_SEQ(i, iend)                                          \
  Index_type nisc_stage = iend;                                                \
  Index_type blksize = default_gpu_block_size;                                 \
  Index_type ith = i;                                                          \
  Index_type blk = ith / blksize;                                              \
  Real_ptr vv_int_p = vv_int + hexhex_new_n_vvint_per_block * blk;             \
  if (i == 0) {                                                                \
    Index_type gsize = iend / blksize;                                         \
    Index_type vv_int_len = hexhex_new_n_vvint_per_block * gsize;              \
    for (Index_type k = 0; k < vv_int_len; ++k) {                              \
      vv_int_p[k] = 0.0;                                                       \
    }                                                                          \
  }                                                                            \
  INTSC_HEXHEX_NEW_BODY_SEQ;                                                   \
  vv_int_p[0] += vv_lo;                                                        \
  vv_int_p[1] += vx_lo;                                                        \
  vv_int_p[2] += vy_lo;                                                        \
  vv_int_p[3] += vz_lo;                                                        \
  vv_int_p[4] += vv_hi;                                                        \
  vv_int_p[5] += vx_hi;                                                        \
  vv_int_p[6] += vy_hi;                                                        \
  vv_int_p[7] += vz_hi;

//  Index i is standard intersection, ipair0 = 8*i is the first
//  subzone pair for this intersection.  Initializes 32 output values
//  for the eight pairs in the first loop.
//
#define INTSC_HEXHEX_NEW_OMP(i, iend)                                          \
  Index_type blksize = default_gpu_block_size;                                 \
  Index_type nisc_stage = iend * hexhex_new_tri_per_group;                     \
  Real_ptr vv_int_p0 = vv_int + i * hexhex_new_n_vvint_per_group;              \
  for (Index_type j = 0; j < hexhex_new_n_vvint_per_group; ++j) {              \
    vv_int_p0[j] = 0.0;                                                        \
  }                                                                            \
  Index_type j0 = i * hexhex_new_tri_per_group;                                \
  for (Index_type j = 0; j < hexhex_new_tri_per_group; ++j) {                  \
    Index_type ith = j0 + j;                                                   \
    Index_type blk = ith / blksize;                                            \
    INTSC_HEXHEX_NEW_BODY_SEQ;                                                 \
    Real_ptr vv_int_p = vv_int + hexhex_new_n_vvint_per_block * blk;           \
    vv_int_p[0] += vv_lo;                                                      \
    vv_int_p[1] += vx_lo;                                                      \
    vv_int_p[2] += vy_lo;                                                      \
    vv_int_p[3] += vz_lo;                                                      \
    vv_int_p[4] += vv_hi;                                                      \
    vv_int_p[5] += vx_hi;                                                      \
    vv_int_p[6] += vy_hi;                                                      \
    vv_int_p[7] += vz_hi;                                                      \
  }

//  This is not needed on Seq and OMP CPU variants.
//
#define INTSC_HEXHEX_NEW_FIXUP_VV_BODY                                         \
  Index_type ith = i;                                                          \
  Real_ptr vv = vv_pair + hexhex_new_nvals_per_std_intsc * ith;                \
  Real_const_ptr vv_int_p = vv_int + 72 * ith;                                 \
  Index_type constexpr nvp = hexhex_new_nvals_per_pair;                        \
  Index_type constexpr nvb = hexhex_new_n_vvint_per_block;                     \
  Int_type k = 0;                                                              \
  if (8 * ith + k < n_szpairs) {                                               \
    vv[nvp * k + 0] = vv_int_p[nvb * k + 0] + vv_int_p[nvb * (k + 1) + 0];     \
    vv[nvp * k + 1] = vv_int_p[nvb * k + 1] + vv_int_p[nvb * (k + 1) + 1];     \
    vv[nvp * k + 2] = vv_int_p[nvb * k + 2] + vv_int_p[nvb * (k + 1) + 2];     \
    vv[nvp * k + 3] = vv_int_p[nvb * k + 3] + vv_int_p[nvb * (k + 1) + 3];     \
  }                                                                            \
  for (Index_type k = 1; k < 8; ++k) {                                         \
    if (8 * ith + k < n_szpairs) {                                             \
      vv[nvp * k + 0] = vv_int_p[nvb * k + 4] + vv_int_p[nvb * (k + 1) + 0];   \
      vv[nvp * k + 1] = vv_int_p[nvb * k + 5] + vv_int_p[nvb * (k + 1) + 1];   \
      vv[nvp * k + 2] = vv_int_p[nvb * k + 6] + vv_int_p[nvb * (k + 1) + 2];   \
      vv[nvp * k + 3] = vv_int_p[nvb * k + 7] + vv_int_p[nvb * (k + 1) + 3];   \
    }                                                                          \
  }

#endif // close include guard RAJAPerf_Apps_INTSC_HEXHEX_NEW_BODY_HPP
