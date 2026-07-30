//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// TRANSPORT3DMC placeholder reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   out[i] = in[i];
/// }
///

#ifndef RAJAPerf_Apps_TRANSPORT3DMC_HPP
#define RAJAPerf_Apps_TRANSPORT3DMC_HPP


#define TRANSPORT3DMC_DATA_SETUP \
  TRANSPORT3DMC::XS cross; \
  double scatterDx; \
  double absDX;\
  double fissionDX; \
  double boundaryDX; \
  double censusDX; \
  double minDX;\
  Index_type p; \
  Index_type nxt; \
  Material m;


#define TRANSPORT3DMC_BODY \
  for (p = 0; p < particles.count; p++) { \
    while(particles.lastEvent[p] != CENSUS || particles.lastEvent[p] != ABSORB ) { \
      if (particles.cell[p] == -1) \
        break; \
        //TODO: Implement search for particle to find cell 
      m = mesh[particles.cell[p]].matID; \
      cross = calcMacroXS(m, particles.group[p]); \
      scatterDX = calcDistScatter(cross.scatter); \
      absDX = calcDistAbs(cross.abs); \
      fissionDX = calcDistFission(cross.fission); \
      nufissionDX = calcDistNuFission(cross.nu_fission); \
      boundaryDX = particles.cell[p].getBoundary(particles.pos[p], particles.dir[p], nxt); \
      particles.calcDX(p); \
      minDX = min(scatterDX, min(absDX, min(fissionDX, min(boundaryDX, particles.dx[p])))); \
      particles.dx[p] -= minDX; \
      if (particles.E[p] < cutoff || minDX == absDX) \
        particles.lastEvent[p] = ABSORB; \
      else if (minDX == scatterDX) { \
        particles.lastEvent[p] = SCATTER; \
        //TODO: Sample direction
      } \
      else if (minDX == fissionDX) { \
        particles.lastEvent[p] = FISSION; \
        //TODO: Handle fission (kill particle? do nothing?)
      } \
      else if (minDX == nufissionDX) { \
        particles.lastEvent[p] = FISSION; \
        //TODO: Handle nu-fission (extend lifetime?)
      } \
      else if (minDX == boundaryDX) {\
        //TODO: Handle BCs
      }
      else {
        particles.lastEvent[p] = CENSUS;
      }
    } \
  } \


#include "common/KernelBase.hpp"
#include <random>

namespace rajaperf
{
class RunParams;

namespace apps
{

class TRANSPORT3DMC : public KernelBase
{
public:

  TRANSPORT3DMC(const RunParams& params);

  ~TRANSPORT3DMC();

  void setSize(Index_type target_size, Index_type target_reps);
  void setUp(VariantID vid, size_t tune_idx);
  void updateChecksum(VariantID vid, size_t tune_idx);
  void tearDown(VariantID vid, size_t tune_idx);

  void defineSeqVariantTunings();

  void runSeqVariant(VariantID vid);

  #define dt 0.001
  #define n_isotope 5
  #define groups 1
  #define cutoff 1e-5

  enum Event {
    CENSUS,
    SCATTER,
    ABSORB,
    BORN,
    FISSION,
    BOUNDARY
  };

  enum BC {
    ELEMENT,
    REFLECT,
    VACUUM,
    SOURCE,
  };

  struct XS {
    double scatter;
    double abs;
    double fission;
    double nu_fission;
    double total;

    XS();
    operator*(int c);
    operator+(XS sigma);
  };

  struct Cell {
    public:
      static int GLBL = 0;
      int ID;
      std::array<double, 6> planes; //-x, +x, -y, +y, -z, +z
      std::array<double, 6> BC;
      std::array<int, 6> next;
      int matID;

      Cell(); 
      double getBoundary(const std::array<double,3> &pos, const std::array<double,3> &angle, uint32_t &surface_cross);
  };

  struct Material {
    int isotopeCt;
    std:vector<double> conc;
    std:vector<int> nucIDs;

    Material() {
      isotopeCt = 1;
      conc = std::vector(1, 500.0);
      nucIDs = std::vector(1, 0);
    }
  }

  struct Particles {
    public:
      size_t count;
      std::vector<int> cell;
      std::vector<int> group;       //lower is higher energy
      std::vector<Event> lastEvent; //doubles as tracker for particle alive
      std::vector<std::array<double,3> > pos;
      std::vector<std::array<double,3> > dir;
      std::vector<double> E;
      std::vector<double> dx;     //remaining distance to census
      std::vector<uint64_t> seed; 
      std::vector<XS> prevXS;
      std::vector<bool> newState; //quick lookup to see if particle state changed and if prevXS valid

      //using fixed parameters for debug for now
      Particles();
      // convert energy to speed to distance
      calcDX(Index_type i);
  };

  // simple MFP
  XS calcMacroXS (size_t mat);
  double calcDistScatter(double SigmaS);
  double calcDistAbs(double SigmaA);
  double calcDistFission(double SigmaF);
  double calcDistNuFission(double SigmaNF);
  std::array<double,3> > sampleScatter(u_int64_t seed);


private:
  std::vector<XS> XSTable; //(n_isotope*groups, );
  std::vector<Material> materials; //(5, Material());
  std::vector<Cell> mesh; // (1, Cell());
  std::vector<double> bin;
  Particles* particles;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
