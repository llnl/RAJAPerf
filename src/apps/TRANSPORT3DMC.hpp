//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///TODO: REPLACE
/// TRANSPORT3DMC placeholder reference implementation:
///
/// for (Index_type i = ibegin; i < iend; ++i ) {
///   out[i] = in[i];
/// }
///

#include "camp/helpers.hpp"

#ifndef RAJAPerf_Apps_TRANSPORT3DMC_HPP
#define RAJAPerf_Apps_TRANSPORT3DMC_HPP

#define dT 0.001
#define N_ISOTOPE 68
//#define GROUPS 10
#define cutoff 1e-5


#define TRANSPORT3DMC_DATA_SETUP(vid, partCt) \
  setUp((VariantID)vid, (size_t)partCt);       \
  XS cross = XS();                \
  double scatterDX = 0.0;                       \
  double absDX = 0.0;                           \
  double fissionDX = 0.0;                       \
  double nufissionDX = 0.0;                     \
  double boundaryDX = 0.0;                      \
  double minDX = 0.0;                           \
  u_int32_t nxt = -1;                           \
  int m = -1;                                  \
  bool logging = true; \


#define TRANSPORT3DMC_RESET \
    std::copy_n(particles.cell,      particles.count, t_particles.cell); \
  std::copy_n(particles.group,     particles.count, t_particles.group); \
  std::copy_n(particles.lastEvent, particles.count, t_particles.lastEvent); \
  std::copy_n(particles.pos,       particles.count, t_particles.pos); \
  std::copy_n(particles.dir,       particles.count, t_particles.dir); \
  std::copy_n(particles.E,         particles.count, t_particles.E); \
  std::copy_n(particles.dx,        particles.count, t_particles.dx); \
  std::copy_n(particles.seed,      particles.count, t_particles.seed); \
  std::copy_n(particles.prevXS,    particles.count, t_particles.prevXS); \
  std::copy_n(particles.newState,  particles.count, t_particles.newState); \ 

#define TRANSPORT3DMC_BODY                                                                                                \
  CALI_MARK_BEGIN("Transport");                                                                                                  \
  std::cout << "Starting particle " << i << "...\n";                                                                      \
  std::cout << "info:\n cell " << particles.cell[i] << std::endl << "t_cell " << t_particles.cell[i] << std::endl; \
  RNG.seed(t_particles.seed[i]);                                                                                             \
  while(t_particles.lastEvent[i] != CENSUS && t_particles.lastEvent[i] != ABSORB && t_particles.lastEvent[i] != ESCAPE ) {  \
    if (t_particles.cell[i] == -1) {                                                                                  \
      std::cout << "LOST\n"; break;                                                                                   \
    }                                                                                                                 \
    m = mesh[t_particles.cell[i]].matID; \
    if (logging) {                                                                                \
    std:: cout << "inside cell "                                                                                      \
                << t_particles.cell[i]                                                                                   \
                << " with material " << m << "\n";         }                                                            \
    cross = calcMacroXS(m, t_particles.group[i]);                                                                       \
    CALI_MARK_BEGIN("Calc Event Distances");                                                                          \
    scatterDX = calcEventDist(cross.scatter, RNG);                                                                    \
    absDX = calcEventDist(cross.abs, RNG);                                                                            \
    fissionDX = calcEventDist(cross.fission, RNG);                                                                    \
    nufissionDX = calcEventDist(cross.nu_fission, RNG);                                                               \
    boundaryDX = mesh[t_particles.cell[i]].getBoundary(t_particles.pos[i], t_particles.dir[i], nxt);                        \
    t_particles.calcDX(i);                                                                                              \
    minDX = std::min({scatterDX, absDX, fissionDX, nufissionDX, boundaryDX, t_particles.dx[i]});                        \
    CALI_MARK_END("Calc Event Distances");                                                                            \
    if(logging) {                                                                                                     \
    CALI_MARK_BEGIN("Logging");                                                                                       \
    std::cout << "\nTraveled dist: " << minDX << "\n";                                                                \
    std::cout << "Event distances: \n";                                                                               \
    std::cout << "Scatter:       " << scatterDX << "\n";                                                              \
    std::cout << "Abs:           " << absDX << "\n";                                                                  \
    std::cout << "fission:       " << fissionDX << "\n";                                                              \
    std::cout << "nufission:     " << nufissionDX << "\n";                                                            \
    std::cout << "boundary:      " << boundaryDX << "\n";                                                             \
    std::cout << "census:        " << t_particles.dx[i] << "\n";                                                        \
    CALI_MARK_END("Logging");   }                                                                                      \
    t_particles.pos[i] = {                                                                                              \
      t_particles.pos[i][0] + t_particles.dir[i][0] * minDX,                                                              \
      t_particles.pos[i][1] + t_particles.dir[i][1] * minDX,                                                              \
      t_particles.pos[i][2] + t_particles.dir[i][2] * minDX                                                               \
    };                                                                                                                \
    t_particles.dx[i] -= minDX;                                                                                         \
    if (t_particles.E[i] < cutoff || minDX == absDX) {                                                                  \
      t_particles.lastEvent[i] = ABSORB;                                                                              \
      if (logging) {                                                                                                  \
      CALI_MARK_BEGIN("Logging");                                                                                     \
      std::cout << "Absorbed\n";                                                                                      \
      CALI_MARK_END("Logging"); }                                                                                     \
    }                                                                                                                 \
    else if (minDX == scatterDX) {                                                                                    \
      t_particles.lastEvent[i] = SCATTER;                                                                             \
      t_particles.dir[i] = sampleScatter(RNG, dist, posDist, t_particles.E[i]);                                       \
      if (logging) {                                                                                                  \
      CALI_MARK_BEGIN("Logging");                                                                                     \
      std::cout << "Scattered\n";                                                                                     \
      CALI_MARK_END("Logging"); }                                                                                     \
    }                                                                                                                 \
    else if (minDX == fissionDX) {                                                                                    \
      t_particles.lastEvent[i] = FISSION;                                                                             \
      if (logging) {                                                                                                  \
      CALI_MARK_BEGIN("Logging");                                                                                     \
      std::cout << "Fission Occured\n";                                                                               \
      CALI_MARK_END("Logging"); }                                                                                     \
    }                                                                                                                 \
    else if (minDX == nufissionDX) {                                                                                  \
      t_particles.lastEvent[i] = FISSION;                                                                             \
      t_particles.dx[i] *= sampleNuFission(RNG);                                                                      \
      if (logging) {                                                                                                  \
      CALI_MARK_BEGIN("Logging");                                                                                     \
      std::cout << "NuFission, life extended\n";                                                                      \
      CALI_MARK_END("Logging"); }                                                                                     \
    }                                                                                                                 \
    else if (minDX == boundaryDX) {                                                                                   \
      CALI_MARK_BEGIN("Handle BCs");                                                                                  \
      t_particles.lastEvent[i] = handleBC(mesh[t_particles.cell[i]], nxt);                                            \
      if (t_particles.lastEvent[i] == BOUNDARY) {                                                                     \
        t_particles.cell[i] = mesh[t_particles.cell[i]].next[nxt];                                                    \
        if (logging) {                                                                                                \
        CALI_MARK_BEGIN("Logging");                                                                                   \
        std::cout << "Boundary, pass\n";                                                                              \
        CALI_MARK_END("Logging"); }                                                                                   \
      }                                                                                                               \
      else if (t_particles.lastEvent[i] == FISSION || t_particles.lastEvent[i] == SCATTER) {                          \
        t_particles.dir[i][0] *= -1.0; t_particles.dir[i][1] *= -1.0; t_particles.dir[i][2] *= -1.0;                  \
        if (logging) {                                                                                                \
        CALI_MARK_BEGIN("Logging");                                                                                   \
        std::cout << "Boundary, Reflected\n";                                                                         \
        CALI_MARK_END("Logging"); }                                                                                   \
      }                                                                                                               \
      else if (t_particles.lastEvent[i] == ESCAPE) {                                                                  \
        if (logging) {                                                                                                \
        CALI_MARK_BEGIN("Logging");                                                                                   \
        std::cout << "Boundary, escape\n";                                                                            \
        CALI_MARK_END("Logging"); }                                                                                   \
      }                                                                                                               \
      CALI_MARK_END("Handle BCs");                                                                                  \
    }                                                                                                                 \
    else {                                                                                                            \
      t_particles.lastEvent[i] = CENSUS;                                                                              \
      if (logging) {                                                                                                  \
      CALI_MARK_BEGIN("Logging");                                                                                     \
      std::cout << "Census, end of lifetime.\n";                                                                      \
      CALI_MARK_END("Logging"); }                                                                                     \
      break;                                                                                                          \
    }                                                                                                                 \
  }                                                                                                                   \
  if (logging) {                                                                                                      \
  CALI_MARK_BEGIN("Logging");                                                                                         \
  std::cout << "Particle " << i << " done, moving to next\n" << std::endl;                                            \
  CALI_MARK_END("Logging"); }                                                                                      \
  CALI_MARK_END("Transport");                                                                                       \

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

  enum Event {
    CENSUS,
    SCATTER,
    ABSORB,
    BORN,
    FISSION,
    BOUNDARY,
    ESCAPE
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
    XS(std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &posDist);
    inline XS operator*(const double c) const {
      XS result = *this;
      result.scatter *= c;
      result.abs *= c;
      result.fission *= c;
      result.nu_fission *= c;
      result.total *= c;
      return result;
    }
    inline XS operator+(const XS &sigma) const{
      XS result = *this;
      result.scatter += sigma.scatter;
      result.abs += sigma.abs;
      result.fission += sigma.fission;
      result.nu_fission += sigma.nu_fission;
      result.total += sigma.total;
      return result;
    }
  };

  struct Cell {
    public:
      static int GLBL;
      int ID;
      std::array<double, 6> planes; //-x, +x, -y, +y, -z, +z
      std::array<BC, 6> bc;
      std::array<int, 6> next;
      int matID;

      Cell(); 
      double getBoundary(const std::array<double,3> &pos, const std::array<double,3> &angle, uint32_t &surface_cross);
  };

  struct Material {
    int isotopeCt;
    Real_ptr conc;
    Int_ptr nucIDs;

    Material();
  };

  struct Particles {
    public:
      size_t count;
      Int_ptr cell;
      Int_ptr group;       //lower is higher energy
      Event * lastEvent; //doubles as tracker for particle alive
      std::array<double,3> * pos;
      std::array<double,3> * dir;
      Real_ptr E;
      Real_ptr dx;     //remaining distance to census
      uint64_t * seed; 
      XS * prevXS;
      bool * newState; //quick lookup to see if particle state changed and if prevXS valid

      void distribute(const size_t numParticles, std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &dist, std::uniform_real_distribution<double> &posDist, Cell* &mesh, uint64_t dim);
      void calcDX(Index_type i);
  };


  XS calcMacroXS(size_t mat, double E);
  double calcEventDist(double Sigma, std::mt19937_64 &rng);
  double sampleNuFission(std::mt19937_64 &rng);
  Event handleBC(Cell &cell, int face);
  void initMaterials(Material* &mats, VariantID vid);
  void buildMeshCube(size_t side, TRANSPORT3DMC::Cell* &mesh, std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &posDist, VariantID vid);
  static std::array<double, 3> sampleScatter(std::mt19937_64 &rng, std::uniform_real_distribution<double> &dist, std::uniform_real_distribution<double> &posDist, double &E);
  //void setup(size_t particles, size_t groups, double deltaT);
  //void Transport(double time);


private:
  XS * XSTable;
  Material * materials;
  Cell * mesh;
  Particles particles;
  Particles t_particles;
  Real_ptr bins;
  std::uniform_real_distribution<double> dist;
  std::uniform_real_distribution<double> posDist;
  // int GROUPS;
  std::mt19937_64 RNGr;

  bool logging;
  std::mt19937_64 RNG;

  size_t dim;
  size_t GROUPS;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
