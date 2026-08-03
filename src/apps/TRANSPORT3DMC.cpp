//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "TRANSPORT3DMC.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace apps
{


TRANSPORT3DMC::TRANSPORT3DMC(const RunParams& params)
  : KernelBase(rajaperf::Apps_TRANSPORT3DMC, params)
{
  setDefaultProblemSize(1);
  setDefaultReps(1000);

  setSize(params.getTargetSize(getDefaultProblemSize()),
          params.getReps(getDefaultReps()));

  setChecksumConsistency(ChecksumConsistency::ConsistentPerVariantTuning);
  setChecksumTolerance(ChecksumTolerance::tight);

  setComplexity(Complexity::N);

  setMaxPerfectLoopDimensions(1);
  setProblemDimensionality(1);

  setUsesFeature(Forall);

  addVariantTunings();
}

void TRANSPORT3DMC::setSize(Index_type target_size, Index_type target_reps)
{
  setActualProblemSize( target_size );
  setRunReps( target_reps );

  setItsPerRep( getActualProblemSize() );
  setKernelsPerRep(1);

  setBytesAllocatedPerRep( 0 ); // in, out
  setBytesReadPerRep( 0 ); // in
  setBytesWrittenPerRep( 0 ); // out
  setBytesModifyWrittenPerRep( 0 );
  setBytesAtomicModifyWrittenPerRep( 0 );
  setFLOPsPerRep( 0 );
}

TRANSPORT3DMC::~TRANSPORT3DMC()
{
}

void TRANSPORT3DMC::setUp(VariantID vid, size_t partCt)
{
  dT = deltaT;
  GROUPS = groups;
  CALI_MARK_BEGIN("Table setup");
  XSTable = std::vector<XS>(N_ISOTOPE*groups, XS(1442695040888963407)); //arbitrary seed for cross section table
  CALI_MARK_END("Table setup");
  
  //Materials copy-pasted from XSBench small problem
  CALI_MARK_BEGIN("Init XSBench Materials");
  initMaterials(materials);
  CALI_MARK_END("Init XSBench Materials");

  //TODO: Implement function to set up actual mesh
  const size_t meshSide = 3;

  CALI_MARK_BEGIN("Init Mesh");
  buildMeshCube(meshSide, mesh);
  CALI_MARK_END("Init Mesh");

  CALI_MARK_BEGIN("Init Particles");
  particles = Particles(partCt);
  CALI_MARK_BEGIN("Init Particles");

  // size_t centerCell = (meshSide / 2) * meshSide * meshSide + (meshSide / 2) * meshSide + (meshSide / 2);
  // std::fill(particles.cell.begin(), particles.cell.end(), static_cast<int>(centerCell));
  CALI_MARK_BEGIN("Bin Init");
  bins = std::vector<double>(groups);

  double binWidth = 14.1e6/(double)groups, binUpperBound = 0;
  for (size_t i = 0; i < groups; i++) {
    binUpperBound += binWidth;
    bins[i] = binUpperBound;
  }
  CALI_MARK_END("Bin Init");
}

void initMaterials(std::vector<Material> &mats) {
  size_t matCt = 12;
  mats.resize(matCt);

  mats[0].isotopeCt = 34;
  mats[1].isotopeCt  = 5;
	mats[2].isotopeCt  = 4;
	mats[3].isotopeCt  = 4;
	mats[4].isotopeCt  = 27;
	mats[5].isotopeCt  = 21;
	mats[6].isotopeCt  = 21;
	mats[7].isotopeCt  = 21;
	mats[8].isotopeCt  = 21;
	mats[9].isotopeCt  = 21;
	mats[10].isotopeCt = 9;
	mats[11].isotopeCt = 9;
  
  mats[0].nucIDs = { 58, 59, 60, 61, 40, 42, 43, 44, 45, 46, 1, 2, 3, 7,
	                 8, 9, 10, 29, 57, 47, 48, 0, 62, 15, 33, 34, 52, 53, 
	                 54, 55, 56, 18, 23, 41 };
  mats[1].nucIDs = { 63, 64, 65, 66, 67 };
  mats[2].nucIDs = { 24, 41, 4, 5 };
  mats[3].nucIDs = { 24, 41, 4, 5 };
  mats[4].nucIDs = { 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 27, 28, 29,
	                 30, 31, 32, 26, 49, 50, 51, 11, 12, 13, 14, 6, 16,
	                 17 }; 
  mats[5].nucIDs = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25,
	                 49, 50, 51, 11, 12, 13, 14 };
  mats[6].nucIDs = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25,
	                 49, 50, 51, 11, 12, 13, 14 };
  mats[7].nucIDs = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25,
	                 49, 50, 51, 11, 12, 13, 14 };
  mats[8].nucIDs = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25,
	                 49, 50, 51, 11, 12, 13, 14 };
  mats[9].nucIDs = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25,
	                 49, 50, 51, 11, 12, 13, 14 };
  mats[10].nucIDs = { 24, 41, 4, 5, 63, 64, 65, 66, 67 };
  mats[11].nucIDs = { 24, 41, 4, 5, 63, 64, 65, 66, 67 }; 


  for (int i = 0; i < matCt; i++) {
    materials[i].conc.resize(materials[i].isotopeCt);
    for (int m = 0; m < materials[i].isotopeCt; m++) {
      materials[i].conc[m] = 20 * posDist(RNGr);
    }
  }
  
}

void buildMeshCube(size_t side, std::vector<Cell> &mesh) {
  if (side == 0) {
    throw std::invalid_argument("buildMeshCube side must be nonzero");
  }

  constexpr double cellWidth = 0.01;
  size_t layerSize = side * side;
  size_t cubeSz = side * side * side;

  Cell::GLBL = 0;
  mesh.clear();
  mesh.reserve(cubeSz);
  for (size_t i = 0; i < cubeSz; i++) {
    mesh.emplace_back();
  }

  auto index = [side, layerSize](size_t row, size_t col, size_t layer) {
    return static_cast<int>(layer * layerSize + col * side + row);
  };

  const double halfWidth = 0.5 * side * cellWidth;

  std::uniform_int_distribution<int> matDist(0, 11);
  std::mt19937_64 genr(11111111111111111111);
  
  for (size_t layer = 0; layer < side; layer++) {
    for (size_t col = 0; col < side; col++) {
      for (size_t row = 0; row < side; row++) {
        int i = index(row, col, layer);
        Cell &cell = mesh[i];

        cell.next = {
          row > 0 ? index(row - 1, col, layer) : -1,
          row + 1 < side ? index(row + 1, col, layer) : -1,
          col + 1 < side ? index(row, col + 1, layer) : -1,
          col > 0 ? index(row, col - 1, layer) : -1,
          layer > 0 ? index(row, col, layer - 1) : -1,
          layer + 1 < side ? index(row, col, layer + 1) : -1
        };

        if (row == 0) cell.bc[0] = REFLECT;
        if (row + 1 == side) cell.bc[1] = REFLECT;
        if (col + 1 == side) cell.bc[2] = REFLECT;
        if (col == 0) cell.bc[3] = REFLECT;
        if (layer == 0) cell.bc[4] = VACUUM;
        if (layer + 1 == side) cell.bc[5] = VACUUM;

        double xMin = -halfWidth + row * cellWidth;
        double yMax = halfWidth - col * cellWidth;
        double zMin = -halfWidth + layer * cellWidth;
        cell.planes = {
          xMin, xMin + cellWidth,
          yMax - cellWidth, yMax,
          zMin, zMin + cellWidth
        };

        cell.matID = matDist(genr);      
      }
    }
  }

  for (size_t i = 0; i < cubeSz; i++) {
    std::cout << "cell " << i << " info:\n" 
              << " bounds:\n"
              << "  x: " << mesh[i].planes[0] << " " << mesh[i].planes[1] << "\n"
              << "  y: " << mesh[i].planes[2] << " " << mesh[i].planes[3] << "\n"
              << "  z: " << mesh[i].planes[4] << " " << mesh[i].planes[5] << "\n"
              << " neighbors:\n" 
              << "  x: " << mesh[i].next[0] << " " << mesh[i].next[1] << "\n"
              << "  y: " << mesh[i].next[2] << " " << mesh[i].next[3] << "\n"
              << "  z: " << mesh[i].next[4] << " " << mesh[i].next[5] << "\n"
              << " BCs:\n" 
              << "  x: " << mesh[i].bc[0] << " " << mesh[i].bc[1] << "\n"
              << "  y: " << mesh[i].bc[2] << " " << mesh[i].bc[3] << "\n"
              << "  z: " << mesh[i].bc[4] << " " << mesh[i].bc[5] << "\n";
  }
}

void TRANSPORT3DMC::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_out, getActualProblemSize(), vid);
}

void TRANSPORT3DMC::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  XSTable.clear();
  materials.clear();
  mesh.clear();
  bins.clear();

}

//Cross Section Helper functions
TRANSPORT3DMC::XS::XS() {
  scatter = 0;
  abs = 0;
  fission = 0;
  nu_fission = 0;
  total = 0;
}

XS::XS(uint64_t seed) {
  //std::mt19937_64 rng(seed);
  scatter = posDist(RNGr);
  abs = posDist(RNGr);
  fission = posDist(RNGr);
  nu_fission = posDist(RNGr);
  total = scatter + abs + fission + nu_fission;
}

TRANSPORT3DMC::XS::operator*(int c) {
  scatter *= c;
  abs *= c;
  fission *= c;
  nu_fission *= c;
  total *= c;
}

TRANSPORT3DMC::XS::operator+(XS sigma) {
  scatter += sigma.scatter;
  abs += sigma.abs;
  fission += sigma.fission;
  nu_fission += sigma.fission;
  total += sigma.total;
}

XS calcMacroXS(size_t mat, double E) {
  CALI_CXX_MARK_FUNCTION;
  XS Sigma;
  size_t bin = std::upper_bound(bins.begin(), bins.end(), E) - bins.begin();
  for (int i = 0; i < materials[mat].isotopeCt; i++) {
    Sigma = Sigma + XSTable[GROUPS * materials[mat].nucIDs[i] + bin] * materials[mat].conc[i];
  }
  return Sigma;
}

double calcEventDist(double Sigma, std::mt19937_64 &rng) {
  return (dist(rng)/2 + 0.5) /Sigma;
}

std::array<double, 3> sampleScatter(std::mt19937_64 &rng, double &E) {
  CALI_CXX_MARK_FUNCTION;
  double x = dist(rng), y = dist(rng), z = dist(rng);
  double norm = std::sqrt(x*x + y*y + z*z);
  x /= norm; y/= norm; z/= norm;

  E *= posDist(rng);

  return {x,y,z};
}

double sampleNuFission(std::mt19937_64 &rng) {
  CALI_CXX_MARK_FUNCTION;
  return 5 * (dist(rng) + 1);
}

//Cell/mesh constructor helper functions
Cell::Cell() {
    for (int i = 0; i < 6; i++){
      next[i] = -1;
      bc[i] = ELEMENT;
      planes[i] = (i % 2 == 0) ? -1.0 : 1.0;
    }
    ID = GLBL;
    GLBL++;
    matID = 0;
  }

double TRANSPORT3DMC::Cell::getBoundary(const std::array<double,3> &pos, const std::array<double,3> &angle, uint32_t &surface_cross) {
  double min_dist = 1.0e16;
  double dist = 0.0;
  uint32_t index;
  // only check the positive or negative surface
  for (uint32_t i = 0; i < 3; i++) {
    index = 2 * i + ( (angle[i] > 0) ? 1 : 0 );
    dist = (nodes[index] - pos[i]) / angle[i];
    if (dist < min_dist) {
      min_dist = dist;
      surface_cross = index;
    }
  }
  return min_dist;
}

Event handleBC(Cell &cell, int face) {
  CALI_CXX_MARK_FUNCTION;
  Event pState;
  switch (cell.bc[face]) {
  case ELEMENT:
    pState = BOUNDARY;
    break;
  case REFLECT:
    pState = SCATTER;
    break;
  case VACUUM:
    pState = ESCAPE;
    break;
  case SOURCE:
    pState = FISSION;
    break;
  }
  
  return pState;
}

//Material constructor function
TRANSPORT3DMC::Material::Material() {
  isotopeCt = 1;
  conc = std::vector<double>();
  nucIDs = std::vector<int>;
}

//particle constructor and helpers

Particles::Particles() {

  cell  = std::vector(1, 0);
  group = std::vector(1, 0);
  lastEvent = std::vector(1, BORN);
  pos = std::vector<std::array<double, 3> >(1, {0.0, 0.0, 0.0});
  dir = std::vector<std::array<double, 3> >(1, {1, 0, 0});
  E   = std::vector(1, 100.0);
  dx  = std::vector<double>(1, 0.0);
  seed = std::vector<uint64_t>(1, 0);
  prevXS = std::vector(1, XS());
  newState = std::vector(1, true);
  count = 1;

  for (int i = 0; i < 1; i++) {
    calcDX(i);
    seed[i] = i*3;
  }
}

Particles::Particles(const size_t numParticles) {

  cell  = std::vector<int>(numParticles);
  group = std::vector(numParticles, 0);
  lastEvent = std::vector(numParticles, BORN);
  pos = std::vector<std::array<double, 3> >(numParticles, {0.0, 0.0, 0.0});
  dir = std::vector<std::array<double, 3> >(numParticles);
  E   = std::vector<double>(numParticles, 14.4e6);
  dx  = std::vector<double>(numParticles);
  seed = std::vector<uint64_t>(numParticles, 0);
  prevXS = std::vector(numParticles, XS());
  newState = std::vector(numParticles, true);
  count = numParticles;

  for (size_t i = 0; i < numParticles; i++) {
    seed[i] = i*3;
    dir[i] = sampleScatter(RNGr, E[i]);
    cell[i] = 26 * posDist(RNGr);
    calcDX(i);
  }
}

TRANSPORT3DMC::Particles::calcDX(Index_type i) {
  dx[i] = sqrt(2/1.7e-27 * E[i]/(double)1.6e-13 ) * dt;
}

} // end namespace apps
} // end namespace rajaperf
