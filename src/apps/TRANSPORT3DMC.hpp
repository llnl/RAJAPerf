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
  Real_ptr in = m_in; \
  Real_ptr out = m_out;

// Placeholder body. Replace this statement with the target kernel operation.
#define TRANSPORT3DMC_BODY \
  out[i] = in[i];


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
  };

  struct Cell {
    public:
      int ID;
      std::array<double, 6> planes; //-x, +x, -y, +y, -z, +z
      std::array<double, 6> BC;
      std::array<int, 6> next;
      int matID; 
  };

  struct Material {
    int isotopeCt;
    std:vector<double> conc;
    std:vector<int> nucIDs;
  }

  struct Particles {
    public:
      std::vector<int> cell;
      std::vector<int> group;
      std::vector<Event> lastEvent; //doubles as tracker for particle alive
      std::vector<double> pos;
      std::vector<double> dir;
      std::vector<double> E;
      std::vector<double> dx;     //remaining distance to census
      std::vector<uint64_t> seed; 
      std::vector<XS> prevXS;
      std::vector<bool> newState; //quick lookup to see if particle state changed and if prevXS valid
  };

private:
  Real_ptr m_in;
  Real_ptr m_out;
};

} // end namespace apps
} // end namespace rajaperf

#endif // closing endif for header file include guard
