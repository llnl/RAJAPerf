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


#define TRANSPORT3DMC_DATA_SETUP(vid, partCt) \
  setUp((VariantID)vid, (size_t)partCt); \
  cross = XS();                \
  scatterDX = 0.0;                       \
  absDX = 0.0;                           \
  fissionDX = 0.0;                       \
  nufissionDX = 0.0;                     \
  boundaryDX = 0.0;                      \
  minDX = 0.0;                           \
  nxt = -1;                           \
  m = -1;                                  \


#define TRANSPORT3DMC_RESET \
  t_particles = particles;  \

#define TRANSPORT3DMC_BODY                                                                                                \
  CALI_MARK_BEGIN("Transport");                                                                                                  \
  std::cout << "Starting particle " << i << "...\n";                                                                      \
  std::cout << "info:\n cell " << particles.cell[i] << std::endl << "t_cell " << t_particles.cell[i] << std::endl; \ 
      RNG.seed(t_particles.seed[i]);                                                                                             \
      while(t_particles.lastEvent[i] != CENSUS && t_particles.lastEvent[i] != ABSORB && t_particles.lastEvent[i] != ESCAPE ) {  \
        if (t_particles.cell[i] == -1) {                                                                                  \
          std::cout << "LOST\n"; break;                                                                                   \
        }                                                                                                                 \
        m = mesh[t_particles.cell[i]].matID;                                                                                \
        std:: cout << "inside cell "                                                                                      \
                   << t_particles.cell[i]                                                                                   \
                   << " with material " << m << "\n";                                                                     \
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
        CALI_MARK_BEGIN("Logging");                                                                                       \
        std::cout << "\nTraveled dist: " << minDX << "\n";                                                                \
        std::cout << "Event distances: \n";                                                                               \
        std::cout << "Scatter:       " << scatterDX << "\n";                                                              \
        std::cout << "Abs:           " << absDX << "\n";                                                                  \
        std::cout << "fission:       " << fissionDX << "\n";                                                              \
        std::cout << "nufission:     " << nufissionDX << "\n";                                                            \
        std::cout << "boundary:      " << boundaryDX << "\n";                                                             \
        std::cout << "census:        " << t_particles.dx[i] << "\n";                                                        \
        CALI_MARK_END("Logging");                                                                                         \
        t_particles.pos[i] = {                                                                                              \
          t_particles.pos[i][0] + t_particles.dir[i][0] * minDX,                                                              \
          t_particles.pos[i][1] + t_particles.dir[i][1] * minDX,                                                              \
          t_particles.pos[i][2] + t_particles.dir[i][2] * minDX                                                               \
        };                                                                                                                \
        t_particles.dx[i] -= minDX;                                                                                         \
        if (t_particles.E[i] < cutoff || minDX == absDX) {                                                                  \
          t_particles.lastEvent[i] = ABSORB;                                                                                \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Absorbed\n";                                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == scatterDX) {                                                                                    \
          t_particles.lastEvent[i] = SCATTER;                                                                               \
          t_particles.dir[i] = sampleScatter(RNG, dist, posDist, t_particles.E[i]);                                                          \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Scattered\n";                                                                                     \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == fissionDX) {                                                                                    \
          t_particles.lastEvent[i] = FISSION;                                                                               \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Fission Occured\n";                                                                               \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == nufissionDX) {                                                                                  \
          t_particles.lastEvent[i] = FISSION;                                                                               \
          t_particles.dx[i] *= sampleNuFission(RNG);                                                                        \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "NuFission, life extended\n";                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
        }                                                                                                                 \
        else if (minDX == boundaryDX) {                                                                                   \
          t_particles.lastEvent[i] = handleBC(mesh[t_particles.cell[i]], nxt);                                                \
          if (t_particles.lastEvent[i] == BOUNDARY) {                                                                       \
            t_particles.cell[i] = mesh[t_particles.cell[i]].next[nxt];                                                        \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, pass\n";                                                                              \
            CALI_MARK_END("Logging");                                                                                     \
          }                                                                                                               \
          else if (t_particles.lastEvent[i] == FISSION || t_particles.lastEvent[i] == SCATTER) {                              \
            t_particles.dir[i][0] *= -1.0; t_particles.dir[i][1] *= -1.0; t_particles.dir[i][2] *= -1.0;                        \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, Reflected\n";                                                                         \
            CALI_MARK_END("Logging");                                                                                     \
          }                                                                                                               \
          else if (t_particles.lastEvent[i] == ESCAPE) {                                                                    \
            CALI_MARK_BEGIN("Logging");                                                                                   \
            std::cout << "Boundary, escape\n";                                                                            \
            CALI_MARK_END("Logging");                                                                                     \
            break;                                                                                                        \
          }                                                                                                               \
        }                                                                                                                 \
        else {                                                                                                            \
          t_particles.lastEvent[i] = CENSUS;                                                                                \
          CALI_MARK_BEGIN("Logging");                                                                                     \
          std::cout << "Census, end of lifetime.\n";                                                                      \
          CALI_MARK_END("Logging");                                                                                       \
          break;                                                                                                          \
        }                                                                                                                 \
      }                                                                                                                   \
      CALI_MARK_BEGIN("Logging");                                                                                         \
      std::cout << "Particle " << i << " done, moving to next\n" << std::endl;                                            \
      CALI_MARK_END("Logging");                                                                                           \
      CALI_MARK_END("Transport");                                                                                             \

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

  #define dT 0.001
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
    XS(std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &posDist);
    XS operator*(const double c) const;
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
      Particles(const size_t numParticles, std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &dist, std::uniform_real_distribution<double> &posDist, std::vector<Cell> &mesh);
      void calcDX(Index_type i);
  };


  XS calcMacroXS(size_t mat, double E);
  double calcEventDist(double Sigma, std::mt19937_64 &rng);
  double sampleNuFission(std::mt19937_64 &rng);
  Event handleBC(Cell &cell, int face);
  void initMaterials(std::vector<Material> &mats);
  void buildMeshCube(size_t side, std::vector<TRANSPORT3DMC::Cell> &mesh, std::mt19937_64 &RNGr, std::uniform_real_distribution<double> &posDist);
  static std::array<double, 3> sampleScatter(std::mt19937_64 &rng, std::uniform_real_distribution<double> &dist, std::uniform_real_distribution<double> &posDist, double E);
  //void setup(size_t particles, size_t groups, double deltaT);
  //void Transport(double time);


private:
  std::vector<XS> XSTable;
  std::vector<Material> materials;
  std::vector<Cell> mesh;
  Particles particles;
  Particles t_particles;
  std::vector<double> bins;
  std::uniform_real_distribution<double> dist;
  std::uniform_real_distribution<double> posDist;
  // int GROUPS;
  std::mt19937_64 RNGr;

  TRANSPORT3DMC::XS cross;                
  double scatterDX;                       
  double absDX;                           
  double fissionDX;                       
  double nufissionDX;                     
  double boundaryDX;                      
  double minDX;                           
  uint32_t nxt;                           
  int m;                                  
  std::mt19937_64 RNG;                    
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
