//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_Apps_FEMSWEEPMeshGen_HPP
#define RAJAPerf_Apps_FEMSWEEPMeshGen_HPP

#include "common/KernelBase.hpp"

#include <array>
#include <cstddef>
#include <vector>

namespace rajaperf
{

namespace apps
{

// A minimal replacement for the subset of MFEM-based angular quadrature used by the mesh generator.
//
// Derived from the project’s original `transport_code/src/transport/AngularQuadrature.*`:
// - It builds a tensor product of:
//   - Gauss–Legendre polar nodes for cos(theta) on [-1,1], and
//   - uniform azimuthal angles gamma on [0,2π).
// - It stores only the direction vectors ω (no weights or spherical harmonics),
//   because the MFEM-free sweeper/driver only needs ω to classify faces and build ordering.
class AngularQuadratureLite
{
  public:
   // Derived from `transport_code/src/transport/AngularQuadrature.cc` constructor.
   // For this MFEM-free skeleton we assume 3D, so the azimuthal count is:
   //   azimuthalAngles = 4 * azimuthal_order  (since pow(2, dim-1) == 4 in 3D).
   AngularQuadratureLite(Index_type polar_order, Index_type azimuthal_order);

   // Returns the total number of angular directions (polarAngles * azimuthalAngles).
   Index_type GetNumAngles() const { return static_cast<Index_type>(omega_.size()); }

   // Returns ω = (ωx, ωy, ωz) for angle index n.
   const std::array<Real_type, 3> &GetAngleVector(Index_type n) const { return omega_.at(static_cast<Size_type>(n)); }

  private:
   // Stores direction vectors ω in the same loop ordering as the original implementation:
   // outer loop over polar, inner loop over azimuthal.
   std::vector<std::array<Real_type, 3>> omega_;
};


// Minimal MFEM-free sweep setup that reproduces the mesh connectivity arrays from the original driver.
//
// The mesh data in `transport_code/src/drivers/transport_code.cpp` comes from the (trimmed) GPU sweeper setup:
// - face connectivity (global-to-local face mapping + face restriction scatter indices),
// - per-angle face type classification (incoming/outgoing),
// - per-angle hyperplane ordering of elements.
//
// This header defines small array containers and a `MeshGenerator` that reimplements those pieces
// using logic derived from MFEM and the previous MFEM-backed transport code:
// - MFEM mesh topology / face numbering:
//   `mfem_git_v4.7/mesh/mesh.cpp` (`MakeCartesian3D`, `GetElementToFaceTable`, `GetQuadOrientation`)
//   and `mfem_git_v4.7/general/stable3d.cpp` (`STable3D::Push4` keying rule).
// - MFEM Hilbert SFC ordering:
//   `mfem_git_v4.7/mesh/ncmesh.cpp` (`NCMesh::GridSfcOrdering3D`, `HilbertSfc3D`).
// - Transport code GPU setup structure:
//   `transport_code/src/sweep/gpu/setup/{Connectivity,FaceTypes,Hyperplanes}.cc`.
class IntArray
{
  public:
   IntArray() = default;
   explicit IntArray(Index_type n) : data_(static_cast<Size_type>(n)) {}
   Index_type Size() const { return static_cast<Index_type>(data_.size()); }
   Index_type * Data() { return data_.data(); }
   const Index_type * Data() const { return data_.data(); }
   Index_type &operator[](Index_type i) { return data_.at(static_cast<Size_type>(i)); }
   Index_type operator[](Index_type i) const { return data_.at(static_cast<Size_type>(i)); }
   void SetSize(Index_type n) { data_.assign(static_cast<Size_type>(n), 0); }
   void Append(Index_type v) { data_.push_back(v); }

  private:
   std::vector<Index_type> data_;
};

template <Index_type Rank> class MDIntArray;

// A multi-dimensional integer array with 2D indexing and a contiguous "Data()" view,
// matching what the original code used from MFEM's `mfem::MDArray<Index_type,2>`.
template <> class MDIntArray<2>
{
  public:
   MDIntArray() = default;
   MDIntArray(Index_type d0, Index_type d1) : d0_(d0), d1_(d1), data_(static_cast<Size_type>(d0 * d1)) {}
   Size_type Size() const { return static_cast<Size_type>(data_.size()); }
   const Index_type * Data() const { return data_.data(); }
   Index_type &operator()(Index_type i0, Index_type i1) { return data_.at(static_cast<Size_type>(i0 + d0_ * i1)); }
   Index_type operator()(Index_type i0, Index_type i1) const { return data_.at(static_cast<Size_type>(i0 + d0_ * i1)); }

  private:
   Index_type d0_ = 0, d1_ = 0;
   std::vector<Index_type> data_;
};

// A multi-dimensional integer array with 3D indexing and a contiguous "Data()" view,
// matching what the original code used from MFEM's `mfem::MDArray<Index_type,3>`.
template <> class MDIntArray<3>
{
  public:
   MDIntArray() = default;
   MDIntArray(Index_type d0, Index_type d1, Index_type d2)
      : d0_(d0), d1_(d1), d2_(d2), data_(static_cast<Size_type>(d0 * d1 * d2))
   {
   }
   Size_type Size() const { return static_cast<Size_type>(data_.size()); }
   const Index_type * Data() const { return data_.data(); }
   Index_type &operator()(Index_type i0, Index_type i1, Index_type i2)
   {
      return data_.at(static_cast<Size_type>(i0 + d0_ * (i1 + d1_ * i2)));
   }
   Index_type operator()(Index_type i0, Index_type i1, Index_type i2) const
   {
      return data_.at(static_cast<Size_type>(i0 + d0_ * (i1 + d1_ * i2)));
   }

  private:
   Index_type d0_ = 0, d1_ = 0, d2_ = 0;
   std::vector<Index_type> data_;
};

// Minimal mesh generator that constructs exactly the mesh connectivity arrays:
// - `m_nhyperplanes_all_angles`, `m_ohyperplanes_all_angles`, `m_phyperplanes_all_angles`
// - `md_ordered_elements_all_angles`
// - `md_angle_elem_to_face_types`
// - `md_elem_to_faces`
// - `global_to_local_face`
// - `d_indices1`, `d_indices2`
//
// This intentionally does NOT implement a transport solve; only the setup needed for mesh connectivity.
class MeshGenerator
{
  public:
   // Derived from the original sweeper constructor parameters used by the mesh generator.
   // We assume a 3D Cartesian HEX mesh of size nx*ny*nz and Q1 (8 dofs per element).
   MeshGenerator(AngularQuadratureLite &quad, Index_type nx, Index_type ny, Index_type nz, Index_type num_groups);

   // Derived from `transport_code/src/sweep/gpu/setup/Setup.cc` (the trimmed setup path):
   // runs topology build + connectivity + face types + hyperplane ordering.
   void Setup();

   // Used by the sweep.
   const Index_type m_num_groups;
   const Index_type m_num_angles;
   const Index_type nelem;

   // Used by the sweep: hyperplane counts/offsets/sizes across all angles.
   IntArray m_nhyperplanes_all_angles;
   IntArray m_ohyperplanes_all_angles;
   IntArray m_phyperplanes_all_angles;

   // Used by the sweep.
   MDIntArray<2> md_ordered_elements_all_angles;    // nelem, na
   MDIntArray<3> md_angle_elem_to_face_types;       // 6, nelem, na
   MDIntArray<2> md_elem_to_faces;                  // 6, nelem
   IntArray global_to_local_face;                   // nf_total
   IntArray d_indices1;                             // nf_int*4
   IntArray d_indices2;                             // nf_int*4

  private:
   // Internal face record mirroring what MFEM stores in `faces_info`:
   // - which element is "Elem1" vs "Elem2",
   // - which local face ids are used on each element,
   // - the quad orientation (0..7) of elem2 relative to elem1,
   // - and the base face vertex ordering used to compute orientation and normals.
   struct FaceInfo
   {
      Index_type elem1 = -1;
      Index_type lf1 = -1;
      Index_type elem2 = -1;
      Index_type lf2 = -1;
      Index_type orient = 0;
      std::array<Index_type, 4> base_verts{};
   };

   const AngularQuadratureLite &quad_;
   const Index_type nx_, ny_, nz_;

   // For each element, the global vertex ids of the 8 HEX vertices in MFEM's local vertex order.
   std::vector<std::array<Index_type, 8>> elem_verts_;

   // Global face list in MFEM numbering (as produced by `GetElementToFaceTable`).
   std::vector<FaceInfo> faces_;

   // Derived from MFEM's `MakeCartesian3D` + `GetElementToFaceTable`:
   // builds element vertex ids, global faces, and element->face connectivity.
   void BuildTopology_();

   // Derived from transport code's `transport_code/src/sweep/gpu/setup/Connectivity.cc`:
   // builds `global_to_local_face` and `d_indices1/2` (interior face restriction indices).
   void SetupFaceConnectivity_();

   // Derived from transport code's `transport_code/src/sweep/gpu/setup/FaceTypes.cc`:
   // classifies each (face,element,angle) into the integer face type values used by the hyperplane DAG.
   void SetupFaceAngleElemTypes_();

   // Derived from transport code's `transport_code/src/sweep/gpu/setup/Hyperplanes.cc`:
   // performs per-angle dependency DAG traversal to build ordered elements and hyperplane metadata.
   void SetupHyperplanesOrdering_();
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
