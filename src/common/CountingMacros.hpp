//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_CountingMacros_HPP
#define RAJAPerf_CountingMacros_HPP

// Note that using this should change the signature of functions but
// can cause ODR violations if it does not

// Use this wrapper type in variable declarations in a kernel
// ex.
//   RAJAPERF_WRAPPER(Real_type) val = ptr[i];
// Note do not use it if declaring variables with constant values
#ifdef RAJAPERF_WRAPPER
#undef RAJAPERF_WRAPPER
#endif
#define RAJAPERF_WRAPPER(type) counting::Wrapper<type>
#define RAJAPERF_ARRAY1_WRAPPER(type_name) typename counting::Array1WrapperHelper<type_name>::template type
#define RAJAPERF_ARRAY2_WRAPPER(type_name) typename counting::Array2WrapperHelper<type_name>::template type
#define RAJAPERF_ARRAY3_WRAPPER(type_name) typename counting::Array3WrapperHelper<type_name>::template type
#define RAJAPERF_ARRAY4_WRAPPER(type_name) typename counting::Array4WrapperHelper<type_name>::template type

#define RAJAPERF_ATOMIC_ADD_COUNTING(lhs, rhs) \
      (lhs).atomic_add(rhs);


#define RAJAPERF_COUNTERS_INITIALIZE() \
  auto _exterior_context = this->initializeCounters({ \
      RAJAPERF_STRINGIFY(RAJAPERF_WRAPPER({0})), \
      RAJAPERF_STRINGIFY(RAJAPERF_ARRAY1_WRAPPER({0})), \
      RAJAPERF_STRINGIFY(RAJAPERF_ARRAY2_WRAPPER({0})), \
      RAJAPERF_STRINGIFY(RAJAPERF_ARRAY3_WRAPPER({0}))});

#define RAJAPERF_COUNTERS_CODE_WRAPPER(...) \
  auto RAJAPERF_NAME_PER_LINE(_code_context_) = \
      counting::CountingData::current_data-> \
          push_outer_context(RAJAPERF_STRINGIFY(__VA_ARGS__)); \
  __VA_ARGS__; \
  RAJAPERF_NAME_PER_LINE(_code_context_).pop_context()

#define RAJAPERF_COUNTERS_REP_SCOPE() \
  if constexpr (auto _rep_context = \
      counting::CountingData::current_data->push_rep_context( \
          "for (RepIndex_type irep = 0; irep < run_reps; irep = irep + 1)"); \
      false) {} else

#define RAJAPERF_COUNTERS_IF(...) \
  if constexpr (auto RAJAPERF_NAME_PER_LINE(_cond_context_) = \
      counting::CountingData::current_data->push_cond_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); false) {} else \
  __VA_ARGS__

#define RAJAPERF_COUNTERS_ELSE_IF(...) \
  else if constexpr (auto RAJAPERF_NAME_PER_LINE(_cond_context_) = \
      counting::CountingData::current_data->push_cond_context( \
          "else " RAJAPERF_STRINGIFY(__VA_ARGS__)); false) {} else \
  __VA_ARGS__

#define RAJAPERF_COUNTERS_ELSE() \
  else if constexpr (auto RAJAPERF_NAME_PER_LINE(_cond_context_) = \
      counting::CountingData::current_data->push_cond_context( \
          "else"); false) {} else

// Note the main practical difference between this and SEQ_LOOP
// is that only SEQ_LOOP counts iterations
#define RAJAPERF_COUNTERS_OUTER_LOOP(...) \
  if constexpr (auto RAJAPERF_NAME_PER_LINE(_loop_context_) = \
      counting::CountingData::current_data->push_outer_loop_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); false) {} else \
  __VA_ARGS__

#define RAJAPERF_COUNTERS_SEQ_LOOP(...) \
  if constexpr (auto RAJAPERF_NAME_PER_LINE(_loop_context_) = \
      counting::CountingData::current_data->push_seq_loop_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); false) {} else \
  __VA_ARGS__

#define RAJAPERF_COUNTERS_PAR_LOOP(...) \
  if constexpr (auto RAJAPERF_NAME_PER_LINE(_loop_context_) = \
      counting::CountingData::current_data->push_par_loop_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); false) {} else \
  __VA_ARGS__

#define RAJAPERF_COUNTERS_LOOP_BODY(...) \
  auto RAJAPERF_NAME_PER_LINE(_body_context_) = \
      counting::CountingData::current_data->push_body_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); \
  __VA_ARGS__; \
  RAJAPERF_NAME_PER_LINE(_body_context_).pop_context()

#define RAJAPERF_COUNTERS_TEAM_CONTEXT() \
  auto RAJAPERF_NAME_PER_LINE(_team_context_) = \
      counting::CountingData::current_data->push_team_context("");

#define RAJAPERF_COUNTERS_PAR_ALG(...) \
  auto RAJAPERF_NAME_PER_LINE(_alg_context_) = \
      counting::CountingData::current_data->push_par_loop_context( \
          RAJAPERF_STRINGIFY(__VA_ARGS__)); \
  __VA_ARGS__; \
  RAJAPERF_NAME_PER_LINE(_alg_context_).pop_context()


#define RAJAPERF_COUNTERS_PAR_SYNC() \
  counting::CountingData::current_data->add_par_sync("synchronize();")

#define RAJAPERF_COUNTERS_TEAM_SYNC() \
  counting::CountingData::current_data->add_team_sync("synchronize();")

#define RAJAPERF_COUNTERS_FINALIZE() \
  this->finalizeCounters(_exterior_context)


// Wrap rajaperf data types after implementing everything
#define Index_type RAJAPERF_WRAPPER(Index_type)
#define Index_ptr RAJAPERF_WRAPPER(Index_ptr)
#define Index_ptr_ptr RAJAPERF_WRAPPER(Index_ptr_ptr)
#define Size_type RAJAPERF_WRAPPER(Size_type)
#define Int_type RAJAPERF_WRAPPER(Int_type)
#define Int_ptr RAJAPERF_WRAPPER(Int_ptr)
#define Int_ptr_ptr RAJAPERF_WRAPPER(Int_ptr_ptr)
#define Real_type RAJAPERF_WRAPPER(Real_type)
#define Real_array RAJAPERF_ARRAY1_WRAPPER(Real_array)
#define Real_array2 RAJAPERF_ARRAY2_WRAPPER(Real_array2)
#define Real_array3 RAJAPERF_ARRAY3_WRAPPER(Real_array3)
#define Real_array4 RAJAPERF_ARRAY4_WRAPPER(Real_array4)
#define Real_array_ref RAJAPERF_ARRAY1_WRAPPER(Real_array_ref)
#define Real_array2_ref RAJAPERF_ARRAY2_WRAPPER(Real_array2_ref)
#define Real_array3_ref RAJAPERF_ARRAY3_WRAPPER(Real_array3_ref)
#define Real_array4_ref RAJAPERF_ARRAY4_WRAPPER(Real_array4_ref)
#define Real_ptr RAJAPERF_WRAPPER(Real_ptr)
#define Real_ptr_ptr RAJAPERF_WRAPPER(Real_ptr_ptr)
#define Real_const_ptr RAJAPERF_WRAPPER(Real_const_ptr)
#define Real_const_ptr_ptr RAJAPERF_WRAPPER(Real_const_ptr_ptr)
#define Complex_type RAJAPERF_WRAPPER(Complex_type)
#define Complex_ptr RAJAPERF_WRAPPER(Complex_ptr)
#define Data_type RAJAPERF_WRAPPER(Data_type)
#define Data_ptr RAJAPERF_WRAPPER(Data_ptr)

#endif  // closing endif for header file include guard
