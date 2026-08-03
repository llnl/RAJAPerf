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


#define TRANSPORT3DMC_DATA_SETUP(vid, sz) \
  setUp((VariantID)vid, (size_t) partCt); \    
  TRANSPORT3DMC::XS cross;                \
  double scatterDX;                       \
  double absDX;                           \
  double fissionDX;                       \
  double nufissionDX;                     \
  double boundaryDX;                      \
  double minDX;                           \
  uint32_t nxt;                           \
  int m;                                  \
  size_t p;                               \
  std::std::mt19937_64 RNG;               \


#define TRANSPORT3DMC_BODY                                                                                                \
  std::cout << "Starting particle " << p << "...\n";                                                                      \
      RNG(particles.seed[p]);                                                                                             \
      while(particles.lastEvent[p] != CENSUS && particles.lastEvent[p] != ABSORB && particles.lastEvent[p] != ESCAPE ) {  \                                                    \
        if (particles.cell[p] == -1)                                                                                      \
          break;                                                                                                          \
          //TODO: Implement search for particle to find cell 
        m = mesh[particles.cell[p]].matID;                                                                                \
        std:: cout << "inside cell "                                                                                      \
                   << particles.cell[p]                                                                                   \
                   << " with material " << m << "\n";                                                                     \ 
        cross = calcMacroXS(m, particles.group[p]);                                                                       \
        CALI_MARK_BEGIN("Calc Event distances");                                                                          \
        scatterDX = calcEventDist(cross.scatter, RNG);                                                                    \   
        absDX = calcEventDist(cross.abs, RNG);                                                                            \
        fissionDX = calcEventDist(cross.fission, RNG);                                                                    \
        nufissionDX = calcEventDist(cross.nu_fission, RNG);                                                               \
        boundaryDX = mesh[particles.cell[p]].getBoundary(particles.pos[p], particles.dir[p], nxt);                        \
        particles.calcDX(p);                                                                                              \
        minDX = std::min({scatterDX, absDX, fissionDX, nufissionDX, boundaryDX, particles.dx[p]});                        \
        CALI_MARK_END("Calc Event Distances");                                                                            \
        CALI_MARK_BEGIN("Logging");                                                                                       \
        std::cout << "\nTraveled dist: " << minDX << "\n";                                                                \
        std::cout << "Event distances: \n";                                                                               \
        std::cout << "Scatter:       " << scatterDX << "\n";                                                              \
        std::cout << "Abs:           " << absDX << "\n";                                                                  \
        std::cout << "fission:       " << fissionDX << "\n";                                                              \
        std::cout << "nufission:     " << nufissionDX << "\n";                                                            \
        std::cout << "boundary:      " << boundaryDX << "\n";                                                             \
        std::cout << "census:        " << particles.dx[p] << "\n";                                                        \
        CALI_MARK_END("Logging");                                                                                         \
        particles.pos[p] = {                                                                                              \
          particles.pos[p][0] + particles.dir[p][0] * minDX,                                                              \
          particles.pos[p][1] + particles.dir[p][1] * minDX,                                                              \
          particles.pos[p][2] + particles.dir[p][2] * minDX                                                               \
        };                                                                                                                \
        particles.dx[p] -= minDX;                                                                                         \
        if (particles.E[p] < cutoff || minDX == absDX) {                                                                  \
          particles.lastEvent[p] = ABSORB;                                                                                \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Absorbed\n";                                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == scatterDX) {                                                                                    \
          particles.lastEvent[p] = SCATTER;                                                                               \
          particles.dir[p] = sampleScatter(RNG, particles.E[p]);                                                          \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Scattered\n";                                                                                     \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == fissionDX) {                                                                                    \
          particles.lastEvent[p] = FISSION;                                                                               \
          //TODO: Handle fission (assume do nothing)                                                                      
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Fission Occured\n";                                                                               \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == nufissionDX) {                                                                                  \
          particles.lastEvent[p] = FISSION;                                                                               \
          //TODO: Handle nu-fission (assume extended lifetime)
          particles.dx[p] *= sampleNuFission(RNG);                                                                        \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "NuFission, life extended\n";                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == boundaryDX) {                                                                                   \
          particles.lastEvent[p] = handleBC(mesh[particles.cell[p]], nxt);                                                \
          if (particles.lastEvent[p] == BOUNDARY) {                                                                       \
            particles.cell[p] = mesh[particles.cell[p]].next[nxt];                                                        \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, pass\n";                                                                              \
            CALI_MARK_END("Logging");                                                                                     \
          }                                                                                                               \
          else if (particles.lastEvent[p] == FISSION || particles.lastEvent[p] == SCATTER) {                              \
            particles.dir[p][0] *= -1.0; particles.dir[p][1] *= -1.0; particles.dir[p][2] *= -1.0;                        \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, Reflected\n";                                                                         \
            CALI_MARK_END("Logging");                                                                                     \
          }                                                                                                               \
          else if (particles.lastEvent[p] == ESCAPE) {                                                                    \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, escape\n";                                                                            \
            CALI_MARK_END("Logging");                                                                                     \
            break;                                                                                                        \
          }                                                                                                               \
        }                                                                                                                 \  
        else {                                                                                                            \
          particles.lastEvent[p] = CENSUS;                                                                                \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Census, end of lifetime.\n";                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
          break;                                                                                                          \
        }                                                                                                                 \
      }                                                                                                                   \
      CALI_MARK_BEGIN("Logging");                                                                                         \
      std::cout << "Particle " << p << " done, moving to next\n" << std::endl;                                            \
      CALI_MARK_END("Logging");                                                                                           \

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
  #define N_ISOTOPE 68
  #define GROUPS 10
  #define cutoff 1e-5

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
    XS(uint64_t seed);
    XS operator*(double c) const;
    XS operator+(const XS &sigma) const;
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
    std::vector<double> conc;
    std::vector<int> nucIDs;

    Material();
  };

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

      Particles();
      Particles(const size_t numParticles);
      // convert energy to speed to distance
      void calcDX(int i);
  };

  // simple MFP
  XS calcMacroXS(size_t mat, double E);
  double calcDistEvent(double Sigma, std::mt19937_64 &rng);
  std::array<double, 3> sampleScatter( std::mt19937_64 &rng, double &E);
  double sampleNuFission(std::mt19937_64 &rng);
  Event handleBC(Cell &cell, int face);
  void initMaterials(std::vector<Material> &mats);
  void buildMeshCube(size_t side, std::vector<Cell> &mesh);
  void setup(size_t particles, size_t groups, double deltaT);
  void Transport(double time);


private:
  std::vector<XS> XSTable;
  std::vector<Material> materials;
  std::vector<Cell> mesh;
  Particles particles;
  std::vector<double> bins;
  std::uniform_real_distribution<double> dist(-1,1);
  std::uniform_real_distribution<double> posDist(0,1);
  int GROUPS;
  int Cell::GLBL = 0;
  std::mt19937_64 RNGr(18446744073709551557UL);
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
