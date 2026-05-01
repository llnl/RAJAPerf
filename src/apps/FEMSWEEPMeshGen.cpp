//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "FEMSWEEPMeshGen.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace rajaperf
{

namespace numbers
{
// Universal definition of pi
inline constexpr Real_type pi = 3.1415926535897932385;
}

namespace apps
{

// Derived from the standard Gauss–Legendre quadrature construction:
// - The original MFEM-based `AngularQuadrature` used `mfem::IntRules.Get(SEGMENT, 2*polarAngles-1)`
//   which returns an n-point Gauss–Legendre rule on [-1,1].
// - Here we implement the same rule directly via Newton iterations on Legendre polynomials.
// This is a common textbook algorithm (e.g. Numerical Recipes), not copied from MFEM code.
static void GaussLegendre(Index_type n, std::vector<Real_type> &x, std::vector<Real_type> &w)
{
   const Real_type eps = 1e-14;
   const Index_type m = (n + 1) / 2;
   for (Index_type i = 0; i < m; ++i)
   {
      Real_type z = std::cos(rajaperf::numbers::pi * (i + 0.75) / (n + 0.5));
      Real_type z1 = 0.0;
      while (true)
      {
         Real_type p1 = 1.0;
         Real_type p2 = 0.0;
         for (Index_type j = 1; j <= n; ++j)
         {
            const Real_type p3 = p2;
            p2 = p1;
            p1 = ((2.0 * j - 1.0) * z * p2 - (j - 1.0) * p3) / j;
         }
         const Real_type pp = n * (z * p1 - p2) / (z * z - 1.0);
         z1 = z;
         z = z1 - p1 / pp;
         if (std::abs(z - z1) <= eps) { break; }
      }
      x[static_cast<Size_type>(i)] = -z;
      x[static_cast<Size_type>(n - 1 - i)] = z;

      Real_type p1 = 1.0;
      Real_type p2 = 0.0;
      for (Index_type j = 1; j <= n; ++j)
      {
         const Real_type p3 = p2;
         p2 = p1;
         p1 = ((2.0 * j - 1.0) * z * p2 - (j - 1.0) * p3) / j;
      }
      const Real_type pp = n * (z * p1 - p2) / (z * z - 1.0);
      const Real_type wi = 2.0 / ((1.0 - z * z) * pp * pp);
      w[static_cast<Size_type>(i)] = wi;
      w[static_cast<Size_type>(n - 1 - i)] = wi;
   }
}

// Derived from the original `transport_code/src/transport/AngularQuadrature.cc`:
// - Same polar discretization: `polarAngles = 2 * polar_order` and Gauss–Legendre nodes for cos(theta).
// - Same azimuthal discretization for 3D: `azimuthalAngles = 4 * azimuthal_order`.
// - Same formula for direction vectors:
//     omega = (sin(theta)cos(gamma), sin(theta)sin(gamma), cos(theta))
// We intentionally omit weights and spherical harmonics since the mesh generator only needs omega vectors.
AngularQuadratureLite::AngularQuadratureLite(Index_type polar_order, Index_type azimuthal_order)
{
   const Index_type polarAngles = 2 * polar_order;
   const Index_type azimuthalAngles = 4 * azimuthal_order; // 3D

   std::vector<Real_type> x, w;
   x.assign(static_cast<Size_type>(polarAngles), 0.0);
   w.assign(static_cast<Size_type>(polarAngles), 0.0);
   GaussLegendre(polarAngles, x, w);

   omega_.reserve(static_cast<Size_type>(polarAngles * azimuthalAngles));
   for (Index_type j = 0; j < polarAngles; ++j)
   {
      const Real_type theta = std::acos(x[static_cast<Size_type>(j)]);
      for (Index_type k = 0; k < azimuthalAngles; ++k)
      {
         const Real_type gamma = (-rajaperf::numbers::pi / azimuthalAngles) + (2.0 * rajaperf::numbers::pi / azimuthalAngles) * (k + 1.0);
         const Real_type st = std::sin(theta);
         omega_.push_back({st * std::cos(gamma), st * std::sin(gamma), std::cos(theta)});
      }
   }
}

// Derived from MFEM’s cube face vertex ordering:
// - MFEM source: `mfem_v4.7/fem/geom.cpp` `Geometry::Constants<Geometry::CUBE>::FaceVert`.
// This defines, for each local face id (0..5), the 4 local cube-vertex ids (0..7) in MFEM's order.
constexpr Index_type CUBE_FACE_VERT[6][4] = {
   {3, 2, 1, 0}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}, {4, 5, 6, 7},
};

// Derived from MFEM’s Hexahedron vertex layout:
// - MFEM uses the standard 8-vertex numbering of a cube element.
// We encode each cube vertex id as its (x,y,z) bit triple (0 or 1).
constexpr Index_type CUBE_VERT_BITS[8][3] = {
   {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
};

// Derived from MFEM's lexicographic dof indexing for Q1 hexahedra:
// - For Q1 (order 1), there are 2 nodes in each dimension -> 8 dofs.
// - Lexicographic ordering is x-fastest, then y, then z:
//     dof = x + 2*y + 4*z, with x,y,z in {0,1}.
Index_type DofOfCubeVertex(Index_type cube_vertex)
{
   const Index_type x = CUBE_VERT_BITS[cube_vertex][0];
   const Index_type y = CUBE_VERT_BITS[cube_vertex][1];
   const Index_type z = CUBE_VERT_BITS[cube_vertex][2];
   return x + 2 * y + 4 * z;
}

// Derived from MFEM’s quadrilateral face orientation detection:
// - MFEM source: `mfem_v4.7/mesh/mesh.cpp` `Mesh::GetQuadOrientation`.
// Given `base` vertex ordering and `test` vertex ordering for the same quad, compute orientation id (0..7).
Index_type GetQuadOrientation(const std::array<Index_type, 4> &base, const std::array<Index_type, 4> &test)
{
   Index_type i = 0;
   for (; i < 4; ++i)
   {
      if (test[i] == base[0]) { break; }
   }
   if (i == 4) { return 0; }
   if (test[(i + 1) % 4] == base[1]) { return 2 * i; }
   return 2 * i + 1;
}

struct FaceKey
{
   Index_type a, b, c;
   bool operator==(const FaceKey &o) const { return a == o.a && b == o.b && c == o.c; }
};

struct FaceKeyHash
{
   Size_type operator()(const FaceKey &k) const noexcept
   {
      uint64_t h = 1469598103934665603ull;  // A common hash seed.
      // 0x9e3779b97f4a7c15ull is based on golden ratio.
      h ^= static_cast<uint64_t>(k.a) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
      h ^= static_cast<uint64_t>(k.b) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
      h ^= static_cast<uint64_t>(k.c) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
      return static_cast<Size_type>(h);
   }
};

// Derived from MFEM’s `STable3D::Push4` face-keying rule:
// - MFEM source: `mfem_v4.7/general/stable3d.cpp` `STable3D::Push4`.
// MFEM identifies a quad face by the *three smallest* of its four vertex ids (i.e. drops the maximum).
// This is how `Mesh::GetElementToFaceTable()` assigns global face indices in MFEM.
FaceKey KeyFrom4(Index_type r, Index_type c, Index_type f, Index_type t)
{
   Index_type v[4] = {r, c, f, t};
   Index_type max_i = 0;
   for (Index_type i = 1; i < 4; ++i)
   {
      if (v[i] > v[max_i]) { max_i = i; }
   }
   Index_type u[3];
   for (Index_type i = 0, j = 0; i < 4; ++i)
   {
      if (i == max_i) { continue; }
      u[j++] = v[i];
   }
   std::sort(u, u + 3);
   return {u[0], u[1], u[2]};
}

// Derived from MFEM’s sign helper used in SFC ordering:
// - MFEM source: `mfem_v4.7/mesh/ncmesh.cpp` `static int sgn(int x)`.
Index_type sgn(Index_type x)
{
   return (x < 0) ? -1 : (x > 0) ? 1 : 0;
}

// Derived from MFEM’s 3D Hilbert space-filling-curve (SFC) ordering:
// - MFEM source: `mfem_v4.7/mesh/ncmesh.cpp` `HilbertSfc3D`.
// We copy the algorithm verbatim in spirit (same integer recursion and splitting rules),
// but write into a `std::vector<int>` instead of MFEM’s `Array<int>`.
void HilbertSfc3D(Index_type x,
                  Index_type y,
                  Index_type z,
                  Index_type ax,
                  Index_type ay,
                  Index_type az,
                  Index_type bx,
                  Index_type by,
                  Index_type bz,
                  Index_type cx,
                  Index_type cy,
                  Index_type cz,
                  std::vector<Index_type> &coords)
{
   const Index_type w = std::abs(ax + ay + az);
   const Index_type h = std::abs(bx + by + bz);
   const Index_type d = std::abs(cx + cy + cz);

   const Index_type dax = sgn(ax), day = sgn(ay), daz = sgn(az);
   const Index_type dbx = sgn(bx), dby = sgn(by), dbz = sgn(bz);
   const Index_type dcx = sgn(cx), dcy = sgn(cy), dcz = sgn(cz);

   if (h == 1 && d == 1)
   {
      for (Index_type i = 0; i < w; i++, x += dax, y += day, z += daz)
      {
         coords.push_back(x);
         coords.push_back(y);
         coords.push_back(z);
      }
      return;
   }
   if (w == 1 && d == 1)
   {
      for (Index_type i = 0; i < h; i++, x += dbx, y += dby, z += dbz)
      {
         coords.push_back(x);
         coords.push_back(y);
         coords.push_back(z);
      }
      return;
   }
   if (w == 1 && h == 1)
   {
      for (Index_type i = 0; i < d; i++, x += dcx, y += dcy, z += dcz)
      {
         coords.push_back(x);
         coords.push_back(y);
         coords.push_back(z);
      }
      return;
   }

   Index_type ax2 = ax / 2, ay2 = ay / 2, az2 = az / 2;
   Index_type bx2 = bx / 2, by2 = by / 2, bz2 = bz / 2;
   Index_type cx2 = cx / 2, cy2 = cy / 2, cz2 = cz / 2;

   Index_type w2 = std::abs(ax2 + ay2 + az2);
   Index_type h2 = std::abs(bx2 + by2 + bz2);
   Index_type d2 = std::abs(cx2 + cy2 + cz2);

   if ((w2 & 0x1) && (w > 2)) { ax2 += dax, ay2 += day, az2 += daz; }
   if ((h2 & 0x1) && (h > 2)) { bx2 += dbx, by2 += dby, bz2 += dbz; }
   if ((d2 & 0x1) && (d > 2)) { cx2 += dcx, cy2 += dcy, cz2 += dcz; }

   if ((2 * w > 3 * h) && (2 * w > 3 * d))
   {
      HilbertSfc3D(x, y, z, ax2, ay2, az2, bx, by, bz, cx, cy, cz, coords);
      HilbertSfc3D(x + ax2, y + ay2, z + az2, ax - ax2, ay - ay2, az - az2, bx, by, bz, cx, cy, cz, coords);
   }
   else if (3 * h > 4 * d)
   {
      HilbertSfc3D(x, y, z, bx2, by2, bz2, cx, cy, cz, ax2, ay2, az2, coords);
      HilbertSfc3D(x + bx2,
                   y + by2,
                   z + bz2,
                   ax,
                   ay,
                   az,
                   bx - bx2,
                   by - by2,
                   bz - bz2,
                   cx,
                   cy,
                   cz,
                   coords);
      HilbertSfc3D(x + (ax - dax) + (bx2 - dbx),
                   y + (ay - day) + (by2 - dby),
                   z + (az - daz) + (bz2 - dbz),
                   -bx2,
                   -by2,
                   -bz2,
                   cx,
                   cy,
                   cz,
                   -(ax - ax2),
                   -(ay - ay2),
                   -(az - az2),
                   coords);
   }
   else if (3 * d > 4 * h)
   {
      HilbertSfc3D(x, y, z, cx2, cy2, cz2, ax2, ay2, az2, bx, by, bz, coords);
      HilbertSfc3D(x + cx2,
                   y + cy2,
                   z + cz2,
                   ax,
                   ay,
                   az,
                   bx,
                   by,
                   bz,
                   cx - cx2,
                   cy - cy2,
                   cz - cz2,
                   coords);
      HilbertSfc3D(x + (ax - dax) + (cx2 - dcx),
                   y + (ay - day) + (cy2 - dcy),
                   z + (az - daz) + (cz2 - dcz),
                   -cx2,
                   -cy2,
                   -cz2,
                   -(ax - ax2),
                   -(ay - ay2),
                   -(az - az2),
                   bx,
                   by,
                   bz,
                   coords);
   }
   else
   {
      HilbertSfc3D(x, y, z, bx2, by2, bz2, cx2, cy2, cz2, ax2, ay2, az2, coords);
      HilbertSfc3D(x + bx2,
                   y + by2,
                   z + bz2,
                   cx,
                   cy,
                   cz,
                   ax2,
                   ay2,
                   az2,
                   bx - bx2,
                   by - by2,
                   bz - bz2,
                   coords);
      HilbertSfc3D(x + (bx2 - dbx) + (cx - dcx),
                   y + (by2 - dby) + (cy - dcy),
                   z + (bz2 - dbz) + (cz - dcz),
                   ax,
                   ay,
                   az,
                   -bx2,
                   -by2,
                   -bz2,
                   -(cx - cx2),
                   -(cy - cy2),
                   -(cz - cz2),
                   coords);
      HilbertSfc3D(x + (ax - dax) + bx2 + (cx - dcx),
                   y + (ay - day) + by2 + (cy - dcy),
                   z + (az - daz) + bz2 + (cz - dcz),
                   -cx,
                   -cy,
                   -cz,
                   -(ax - ax2),
                   -(ay - ay2),
                   -(az - az2),
                   bx - bx2,
                   by - by2,
                   bz - bz2,
                   coords);
      HilbertSfc3D(x + (ax - dax) + (bx2 - dbx),
                   y + (ay - day) + (by2 - dby),
                   z + (az - daz) + (bz2 - dbz),
                   -bx2,
                   -by2,
                   -bz2,
                   cx2,
                   cy2,
                   cz2,
                   -(ax - ax2),
                   -(ay - ay2),
                   -(az - az2),
                   coords);
   }
}

// Derived from MFEM’s grid-to-Hilbert ordering driver:
// - MFEM source: `mfem_v4.7/mesh/ncmesh.cpp` `NCMesh::GridSfcOrdering3D`.
// Returns a flat list [x0,y0,z0, x1,y1,z1, ...] for all cells in the SFC order.
std::vector<Index_type> GridSfcOrdering3D(Index_type width, Index_type height, Index_type depth)
{
   std::vector<Index_type> coords;
   coords.reserve(static_cast<Size_type>(3 * width * height * depth));

   if (width >= height && width >= depth)
   {
      HilbertSfc3D(0, 0, 0, width, 0, 0, 0, height, 0, 0, 0, depth, coords);
   }
   else if (height >= width && height >= depth)
   {
      HilbertSfc3D(0, 0, 0, 0, height, 0, width, 0, 0, 0, 0, depth, coords);
   }
   else
   {
      HilbertSfc3D(0, 0, 0, 0, 0, depth, width, 0, 0, 0, height, 0, coords);
   }
   return coords;
}

// Derived from the minimal set of members created by the original driver:
// - `m_num_angles` is taken from the quadrature.
// - `nelem` is the number of hexahedra in an nx*ny*nz Cartesian mesh.
MeshGenerator::MeshGenerator(AngularQuadratureLite &quad, Index_type nx, Index_type ny, Index_type nz, Index_type num_groups)
   : m_num_groups(num_groups),
     m_num_angles(quad.GetNumAngles()),
     nelem(nx * ny * nz),
     md_ordered_elements_all_angles(nelem, m_num_angles),
     md_angle_elem_to_face_types(6, nelem, m_num_angles),
     md_elem_to_faces(6, nelem),
     quad_(quad),
     nx_(nx),
     ny_(ny),
     nz_(nz)
{
}

// Derived from the original MFEM-backed setup sequence:
// - Original code calls `sweeper->Setup()` which, for the trimmed GPU sweeper, runs:
//   `SetupFaceConnectivity()`, `SetupFaceAngleElemTypes()`, `SetupHyperplanesOrdering()`.
// We mirror the same logical phases in this MFEM-free reimplementation.
void MeshGenerator::Setup()
{
   BuildTopology_();
   SetupFaceConnectivity_();
   SetupFaceAngleElemTypes_();
   SetupHyperplanesOrdering_();
}

// Derived from MFEM mesh topology generation for Cartesian HEX meshes:
// - MFEM source: `mfem_v4.7/mesh/mesh.cpp` `Mesh::MakeCartesian3D` (default `sfc_ordering=true`),
//   plus `Mesh::GetElementToFaceTable` which uses `STable3D::Push4` (3-smallest-of-4 keying).
// This function reproduces:
// - MFEM’s element ordering (Hilbert SFC over (x,y,z) cells),
// - MFEM’s vertex numbering per element (standard 8-vertex HEX layout),
// - MFEM’s global face numbering and element->face connectivity.
void MeshGenerator::BuildTopology_()
{
   elem_verts_.assign(static_cast<Size_type>(nelem), {});

   auto VTX = [&](Index_type x, Index_type y, Index_type z) { return x + (y + z * (ny_ + 1)) * (nx_ + 1); };

   const auto sfc = GridSfcOrdering3D(nx_, ny_, nz_);
   for (Index_type e = 0; e < nelem; ++e)
   {
      const Index_type x = sfc[static_cast<Size_type>(3 * e + 0)];
      const Index_type y = sfc[static_cast<Size_type>(3 * e + 1)];
      const Index_type z = sfc[static_cast<Size_type>(3 * e + 2)];
      elem_verts_[static_cast<Size_type>(e)] = {
         VTX(x, y, z),
         VTX(x + 1, y, z),
         VTX(x + 1, y + 1, z),
         VTX(x, y + 1, z),
         VTX(x, y, z + 1),
         VTX(x + 1, y, z + 1),
         VTX(x + 1, y + 1, z + 1),
         VTX(x, y + 1, z + 1),
      };
   }

   std::unordered_map<FaceKey, Index_type, FaceKeyHash> face_map;
   face_map.reserve(static_cast<Size_type>(nelem) * 3);

   faces_.clear();

   for (Index_type elem = 0; elem < nelem; ++elem)
   {
      const auto &v = elem_verts_[static_cast<Size_type>(elem)];
      for (Index_type lf = 0; lf < 6; ++lf)
      {
         const Index_type v0 = v[static_cast<Size_type>(CUBE_FACE_VERT[lf][0])];
         const Index_type v1 = v[static_cast<Size_type>(CUBE_FACE_VERT[lf][1])];
         const Index_type v2 = v[static_cast<Size_type>(CUBE_FACE_VERT[lf][2])];
         const Index_type v3 = v[static_cast<Size_type>(CUBE_FACE_VERT[lf][3])];

         FaceKey key = KeyFrom4(v0, v1, v2, v3);
         auto it = face_map.find(key);
         Index_type gf = -1;
         if (it == face_map.end())
         {
            gf = static_cast<Index_type>(faces_.size());
            face_map.emplace(key, gf);
            FaceInfo fi;
            fi.elem1 = elem;
            fi.lf1 = lf;
            fi.base_verts = {v0, v1, v2, v3};
            faces_.push_back(fi);
         }
         else
         {
            gf = it->second;
            FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
            if (fi.elem2 != -1) { continue; }
            fi.elem2 = elem;
            fi.lf2 = lf;
            fi.orient = GetQuadOrientation(fi.base_verts, {v0, v1, v2, v3});
         }

         md_elem_to_faces(lf, elem) = gf;
      }
   }

   global_to_local_face.SetSize(static_cast<Index_type>(faces_.size()));
}

// Derived from the original GPU sweeper’s connectivity setup:
// - Original file: `transport_code/src/sweep/gpu/setup/Connectivity.cc`.
// That code builds:
// - `global_to_local_face`: MFEM interior faces mapped to [0..nf_int-1], boundary faces mapped to negative ids.
// - `d_indices1` / `d_indices2`: L2FaceRestriction scatter indices for interior faces in lexicographic ordering.
//
// In MFEM, `scatter_indices*` come from `L2FaceRestriction(..., LEXICOGRAPHIC, Interior, DoubleValued)`.
// We match MFEM’s output by constructing indices for Q1 hex faces in the same per-face ordering.
void MeshGenerator::SetupFaceConnectivity_()
{
   Index_type f_int = 0;
   Index_type f_bdr = 0;
   for (Index_type gf = 0; gf < global_to_local_face.Size(); ++gf)
   {
      const FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
      const bool interior = (fi.elem2 >= 0);
      if (interior)
      {
         global_to_local_face[gf] = f_int++;
      }
      else
      {
         global_to_local_face[gf] = -1 - f_bdr++;
      }
   }

   d_indices1.SetSize(f_int * 4);
   d_indices2.SetSize(f_int * 4);

   Index_type int_face_index = 0;
   for (Index_type gf = 0; gf < global_to_local_face.Size(); ++gf)
   {
      const FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
      if (fi.elem2 < 0) { continue; }

      std::array<Index_type, 4> ldof1 = {
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf1][0]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf1][1]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf1][2]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf1][3]),
      };
      std::array<Index_type, 4> ldof2 = {
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf2][0]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf2][1]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf2][2]),
         DofOfCubeVertex(CUBE_FACE_VERT[fi.lf2][3]),
      };
      std::sort(ldof1.begin(), ldof1.end());
      std::sort(ldof2.begin(), ldof2.end());

      for (Index_type fd = 0; fd < 4; ++fd)
      {
         d_indices1[int_face_index * 4 + fd] = fi.elem1 * 8 + ldof1[static_cast<Size_type>(fd)];
         d_indices2[int_face_index * 4 + fd] = fi.elem2 * 8 + ldof2[static_cast<Size_type>(fd)];
      }
      int_face_index++;
   }
}

// Derived from the original GPU sweeper’s face-type setup:
// - Original file: `transport_code/src/sweep/gpu/setup/FaceTypes.cc`.
// That implementation computes incoming/outgoing face flags based on `omega·n` using face geometric factors.
//
// For a uniform Cartesian mesh with Q1 hexes, face normals are axis-aligned and constant.
// We compute the face normal direction from the orientation of the face’s base vertex ordering
// (consistent with MFEM’s face construction) and then classify by the sign of `omega·n`.
void MeshGenerator::SetupFaceAngleElemTypes_()
{
   constexpr Real_type eps = 1e-10;

   auto VXYZ = [&](Index_type v)
   {
      const Index_type nxv = nx_ + 1;
      const Index_type nyv = ny_ + 1;
      const Index_type x = v % nxv;
      const Index_type y = (v / nxv) % nyv;
      const Index_type z = v / (nxv * nyv);
      return std::array<Index_type, 3>{x, y, z};
   };

   std::vector<std::array<Index_type, 3>> face_normals(faces_.size());
   for (Size_type gf = 0; gf < faces_.size(); ++gf)
   {
      const auto &bv = faces_[gf].base_verts;
      const auto p0 = VXYZ(bv[0]);
      const auto p1 = VXYZ(bv[1]);
      const auto p3 = VXYZ(bv[3]);
      const std::array<Index_type, 3> e1 = {p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]};
      const std::array<Index_type, 3> e2 = {p3[0] - p0[0], p3[1] - p0[1], p3[2] - p0[2]};
      const std::array<Index_type, 3> c = {
         e1[1] * e2[2] - e1[2] * e2[1],
         e1[2] * e2[0] - e1[0] * e2[2],
         e1[0] * e2[1] - e1[1] * e2[0],
      };
      face_normals[gf] = {sgn(c[0]), sgn(c[1]), sgn(c[2])};
   }

   for (Index_type elem = 0; elem < nelem; ++elem)
   {
      for (Index_type lf = 0; lf < 6; ++lf)
      {
         const Index_type gf = md_elem_to_faces(lf, elem);
         const FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
         const bool interior = (fi.elem2 >= 0);

         for (Index_type a = 0; a < m_num_angles; ++a)
         {
            Index_type face_type = 0;
            if (interior)
            {
               const auto n = face_normals[static_cast<Size_type>(gf)];
               const auto &o = quad_.GetAngleVector(a);
               const Real_type dot = o[0] * static_cast<Real_type>(n[0]) + o[1] * static_cast<Real_type>(n[1])
                                  + o[2] * static_cast<Real_type>(n[2]);
               const bool D1_nonzero = (dot > eps);

               if (fi.elem1 == elem)
               {
                  face_type = D1_nonzero ? 0 : 1;
               }
               else
               {
                  face_type = D1_nonzero ? 2 : 0;
               }
            }

            md_angle_elem_to_face_types(lf, elem, a) = face_type;
         }
      }
   }
}

// Derived from the hyperplane ordering used by the original GPU sweeper:
// - Original file: `transport_code/src/sweep/gpu/setup/Hyperplanes.cc`.
// That algorithm:
// - counts incoming interior faces per element for each angle,
// - repeatedly emits “ready” elements as a hyperplane, updating downstream dependencies,
// - produces:
//   - `md_ordered_elements_all_angles` (element sweep order per angle),
//   - `m_nhyperplanes_all_angles` (hyperplane count per angle),
//   - `m_ohyperplanes_all_angles` (offsets per angle),
//   - `m_phyperplanes_all_angles` (elements per hyperplane, appended across angles).
void MeshGenerator::SetupHyperplanesOrdering_()
{
   m_nhyperplanes_all_angles.SetSize(m_num_angles);
   m_phyperplanes_all_angles = IntArray();

   std::vector<Index_type> can_compute(static_cast<Size_type>(nelem));
   std::vector<Index_type> can_compute_next(static_cast<Size_type>(nelem));
   std::vector<Index_type> num_dependencies(static_cast<Size_type>(nelem));

   for (Index_type angle = 0; angle < m_num_angles; ++angle)
   {
      for (Index_type e = 0; e < nelem; ++e)
      {
         Index_type deps = 0;
         for (Index_type lf = 0; lf < 6; ++lf)
         {
            const Index_type gf = md_elem_to_faces(lf, e);
            const FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
            const bool interior = (fi.elem2 >= 0);
            const Index_type t = md_angle_elem_to_face_types(lf, e, angle);
            if (interior && (t == 1 || t == 2)) { deps++; }
         }
         num_dependencies[static_cast<Size_type>(e)] = deps;
      }

      Index_type nelem_in_hyperplane = 0;
      for (Index_type e = 0; e < nelem; ++e)
      {
         if (num_dependencies[static_cast<Size_type>(e)] == 0)
         {
            can_compute[static_cast<Size_type>(nelem_in_hyperplane)] = e;
            nelem_in_hyperplane++;
         }
      }

      Index_type count = 0;
      Index_type steps_taken = 0;
      Index_type nhyperplanes = 0;

      while (nelem_in_hyperplane > 0 && steps_taken < nelem)
      {
         m_phyperplanes_all_angles.Append(nelem_in_hyperplane);
         for (Index_type k = 0; k < nelem_in_hyperplane; ++k)
         {
            const Index_type e = can_compute[static_cast<Size_type>(k)];
            md_ordered_elements_all_angles(count, angle) = e;
            count++;
         }

         nhyperplanes++;
         steps_taken++;

         Index_type nelem_in_next = 0;
         for (Index_type k = 0; k < nelem_in_hyperplane; ++k)
         {
            const Index_type e = can_compute[static_cast<Size_type>(k)];
            for (Index_type lf = 0; lf < 6; ++lf)
            {
               const Index_type gf = md_elem_to_faces(lf, e);
               const FaceInfo &fi = faces_.at(static_cast<Size_type>(gf));
               const bool interior = (fi.elem2 >= 0);
               const Index_type t = md_angle_elem_to_face_types(lf, e, angle);
               if (!interior || (t == 1 || t == 2)) { continue; }

               const Index_type adj = (fi.elem1 == e) ? fi.elem2 : fi.elem1;
               if (adj < 0) { continue; }
               const Index_type nd = --num_dependencies[static_cast<Size_type>(adj)];
               if (nd == 0)
               {
                  can_compute_next[static_cast<Size_type>(nelem_in_next)] = adj;
                  nelem_in_next++;
               }
            }
         }

         nelem_in_hyperplane = nelem_in_next;
         for (Index_type k = 0; k < nelem_in_hyperplane; ++k)
         {
            can_compute[static_cast<Size_type>(k)] = can_compute_next[static_cast<Size_type>(k)];
         }
      }

      m_nhyperplanes_all_angles[angle] = nhyperplanes;
   }

   m_ohyperplanes_all_angles.SetSize(m_num_angles);
   Index_type sum = 0;
   for (Index_type a = 0; a < m_num_angles; ++a)
   {
      m_ohyperplanes_all_angles[a] = sum;
      sum += m_nhyperplanes_all_angles[a];
   }
}

} // end namespace apps
} // end namespace rajaperf
