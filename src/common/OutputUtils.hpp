//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) Lawrence Livermore National Security, LLC and other 
// RAJA Project Developers. See top-level LICENSE and COPYRIGHT
// files for dates and other details. No copyright assignment is required
// to contribute to RAJA Performance Suite.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Utility methods for generating output reports.
///

#ifndef RAJAPerf_OutputUtils_HPP
#define RAJAPerf_OutputUtils_HPP

#include <string>

namespace rajaperf
{

/*!
 * \brief Recursively construct directories based on a relative or 
 * absolute path name.  
 * 
 * Return string name of directory if created successfully, else empty string.
 */
std::string recursiveMkdir(const std::string& in_path);

}  // closing brace for rajaperf namespace

#endif  // closing endif for header file include guard
