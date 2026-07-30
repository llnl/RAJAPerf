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

void TRANSPORT3DMC::setUp(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  // allocAndInitData(m_in, getActualProblemSize(), vid);
  // allocAndInitDataConst(m_out, getActualProblemSize(), 0.0, vid);

  //TODO: Add actual different XS's along with different isotopes
  XSTable = std::vector<XS>(n_isotope * groups, XS(0.5));
  //TODO: Implement actual materials
  materials = std::vector<Material>(5, Material());
  //TODO: Implement function to set up actual mesh
  mesh = std::vector<Cell>(1, Cell());
  particles = new Particles();
  bin = vector<double>(groups);

  double binWidth = 14.1e7/(double)groups, binUpperBound = 0;
  for (int i = 0; i < groups; i++) {
    binUpperBound += binWidth;
    bin[i] = binWidth;
  }
}

void TRANSPORT3DMC::updateChecksum(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  addToChecksum(m_out, getActualProblemSize(), vid);
}

void TRANSPORT3DMC::tearDown(VariantID vid, size_t RAJAPERF_UNUSED_ARG(tune_idx))
{
  deallocData(m_in, vid);
  deallocData(m_out, vid);
}

//Cross Section Helper functions
TRANSPORT3DMC::XS::XS() {
  scatter = 0;
  abs = 0;
  fission = 0;
  nu_fission = 0;
  total = 0;
}

TRANSPORT3DMC::XS::XS(double sigma) {
  scatter = sigma;
  abs = sigma;
  fission = sigma;
  nu_fission = sigma;
  total = 4 * sigma;
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

//Cell/mesh constructor helper functions
TRANSPORT3DMC::Cell::Cell() {
  for (int i = 0; i < 6; i++){
    next[i] = -1;
    BC[i] = REFLECT;
    planes[i] = 1;
    if (i%2 == 1)
      planes[i] = - planes[i];
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

//Material constructor function
TRANSPORT3DMC::Material::Material() {
  isotopeCt = 1;
  conc = std::vector(1, 500.0);
  nucIDs = std::vector(1, 0);
}

//particle constructor and helpers

TRANSPORT3DMC::Particles::Particles() {
  cell  = std::vector(1, 0);
  group = std::vector(1, 0);
  lastEvent = std::vector(1, BORN);
  pos = std::vector(1, [0, 0, 0]);
  dir = std::vector(1, [1, 0, 0]);
  E   = std::vector(1, 100.0);
  seed = std::vector(1, 1);
  prevXS = std::vector(1, XS());
  newState = std::vector(1, true);
  count = 1;
  calcDX(0); 
}

TRANSPORT3DMC::Particles::calcDX(Index_type i) {
  dx[i] = sqrt(2/1.7e-27 * E[i]/(double)1.6e-13 ) * dt;
}

//Other helpers for history loop
XS TRANSPORT3DMC::calcMacroXS(size_t mat, double E) {
  XS Sigma;
  //TODO: implement search to get bin - 1 hardcoded for now
  //take summation of MacroscopicXS's based on 
  for (int i = 0; i < materials[mat].isotopeCt; i++) {
    Sigma += XSTable[groups * n_isotope + materials[mat].nucIDs[i]] * materials[mat].conc[i];
  }
}

double TRANSPORT3DMC::calcDistScatter(double SigmaS) {
  return 1/SigmaS;
}

double TRANSPORT3DMC::calcDistAbs(double SigmaA) {
  return 1/SigmaA;
}

double TRANSPORT3DMC::calcDistFission(double SigmaF) {
  return 1/SigmaF;
}

double TRANSPORT3DMC::calcDistNuFission(double SigmaNF) {
  return 1/SigmaNF;
}

} // end namespace apps
} // end namespace rajaperf
