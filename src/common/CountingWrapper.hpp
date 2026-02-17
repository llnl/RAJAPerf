//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_CountingWrapper_HPP
#define RAJAPerf_CountingWrapper_HPP

#include "common/RAJAPerfSuite.hpp"
#include "common/RPTypes.hpp"
#include "common/CountingData.hpp"

#include <sstream>
#include <type_traits>
#include <iterator>
#include <utility>
#include <cstddef>

namespace rajaperf
{

namespace counting
{

// Wrapper types that count operations
template < typename T >
struct Wrapper;


template < typename T >
struct is_wrapper
{
  static inline constexpr bool value = false;
};

template < typename T >
struct is_wrapper<Wrapper<T>>
{
  static inline constexpr bool value = true;
};

template < typename T >
inline constexpr bool is_wrapper_v = is_wrapper<T>::value;



template < typename T >
concept Wrapped = is_wrapper_v<std::remove_cvref_t<T>>;

template < typename T >
concept NonWrapped = !Wrapped<T>;

template < typename T >
concept WrappedVal = Wrapped<T> && T::is_val;

template < typename T >
concept WrappedArray = Wrapped<T> && T::is_array;

template < typename T >
concept WrappedPtr = Wrapped<T> && T::is_ptr;

template < typename T >
concept WrappedNonPtr = Wrapped<T> && !T::is_ptr;

template < typename T >
concept WrappedRef = Wrapped<T> && T::is_ref;


template < typename T >
struct PointedToType
{
  using type = std::remove_reference_t<decltype(*std::declval<T>())>;
};

template < WrappedPtr T >
struct PointedToType<T>
{
  using type = typename std::remove_cvref_t<T>::pointed_to_type;
};

template < typename T >
using pointed_to_type_t = typename PointedToType<T>::type;


template < typename T >
struct WrappedType
{
  using type = T;
};

template < Wrapped T >
struct WrappedType<T>
{
  using direct_type = typename std::remove_cvref_t<T>::wrapped_type;
  using const_type = std::conditional_t<std::is_const_v<T>, std::add_const_t<direct_type>, direct_type>;
  using lref_type = std::conditional_t<std::is_lvalue_reference_v<T>, std::add_lvalue_reference_t<const_type>, const_type>;
  using rref_type = std::conditional_t<std::is_rvalue_reference_v<T>, std::add_rvalue_reference_t<lref_type>, lref_type>;
  using type = rref_type;
};

template < typename T >
using wrapped_type_t = typename WrappedType<T>::type;


template < typename T >
concept raw_pointer = std::is_pointer_v<T>;

template < typename T >
concept pointer = raw_pointer<T> || WrappedPtr<T>;

template < typename T >
concept convertible_to_pointer = std::convertible_to<wrapped_type_t<T>, pointed_to_type_t<T>*>;

template < typename T, typename U >
concept convertible_to = std::convertible_to<wrapped_type_t<T>, wrapped_type_t<U>>;

template < typename T >
concept integral = std::integral<T> ||
    (Wrapped<T> && std::integral<typename T::value_type>);


template < typename T >
constexpr decltype(auto) get_value(T&& val, Size_type num_ops=0)
{
  if constexpr (Wrapped<T>) {
    return std::forward<T>(val).get_native(num_ops);
  } else {
    return std::forward<T>(val);
  }
}

template<typename T, typename V>
struct add_all_extents_of_to
{
  using type = V;
};

template<typename T, typename V>
struct add_all_extents_of_to<T[], V>
{
  using type = typename add_all_extents_of_to<T, V>::type[];
};

template<typename T, std::size_t N, typename V>
struct add_all_extents_of_to<T[N], V>
{
  using type = typename add_all_extents_of_to<T, V>::type[N];
};

template<typename T, typename V>
using add_all_extents_of_to_t = typename add_all_extents_of_to<T,V>::type;

template < typename T >
struct Wrapper
{
  static inline constexpr bool is_ref = std::is_reference_v<T>;
  static inline constexpr bool is_val = !is_ref;
  static inline constexpr bool is_array = std::is_array_v<std::remove_reference_t<T>>;
  static inline constexpr bool is_ptr = std::is_pointer_v<T>;

  template < typename U >
  friend struct Wrapper;

  using wrapped_type = T;

  using value_type = std::conditional_t<is_ref, std::remove_reference_t<T>, T>;
  using const_value_type = std::conditional_t<is_array,
      add_all_extents_of_to_t<value_type, const std::remove_all_extents_t<value_type>>,
      const value_type>;

  using member_type = std::conditional_t<is_ref,
      std::add_const_t<std::add_pointer_t<value_type>>, value_type>;

  using pointed_to_type =
      std::conditional_t<is_array, std::remove_extent_t<value_type>,
      std::conditional_t<is_ptr,   std::remove_pointer_t<value_type>,
                                   value_type>>;
  using const_pointed_to_type =
      std::conditional_t<is_array, std::remove_extent_t<const_value_type>,
      std::conditional_t<is_ptr,   std::remove_pointer_t<const_value_type>,
                                   const_value_type>>;

  template < size_t... Is >
  static constexpr size_t get_array_size(std::index_sequence<Is...>)
  {
    return (... * std::extent_v<wrapped_type, Is>);
  }
  ///
  static constexpr size_t get_array_size()
  {
    if constexpr (is_array) {
      using dims = std::make_index_sequence<std::rank_v<value_type>>;
      return get_array_size(dims{});
    }
    return size_t(0);
  }

  explicit Wrapper(AllocationMetadata* allocation, member_type value)
    : m_value(value)
    , m_allocation(allocation)
  {
  }

  // allow default construction of non-ref values
  Wrapper()
  requires(is_val && !is_array)
    : m_value()
  {
  }
  ///
  Wrapper(std::source_location location = std::source_location::current())
  requires(is_val && is_array)
    : m_value()
  {
    registerArray(location);
    m_allocation = CountingData::current_data->get_allocation(
          static_cast<void*>(&m_value));
  }

  // allow implicit construction from non-wrapped values
  template < convertible_to<wrapped_type> rhs_T >
  Wrapper(rhs_T&& rhs)
  requires(is_val && !is_array && !is_ptr)
    : m_value(get_value(std::forward<rhs_T>(rhs), 1))
  {
    this->count(Operation::copy, 1);
  }
  ///
  Wrapper(std::nullptr_t)
  requires(is_val && !is_array && is_ptr)
    : Wrapper()
  {
  }
  ///
  template < convertible_to<wrapped_type> rhs_T >
  Wrapper(rhs_T&& rhs)
  requires(is_val && !is_array && is_ptr)
    : m_value(get_value(std::forward<rhs_T>(rhs), 1))
  {
    if constexpr (WrappedPtr<rhs_T>) {
      m_allocation = rhs.m_allocation;
    } else {
      m_allocation = CountingData::current_data->get_allocation(
            static_cast<void*>(m_value));
    }
    if (!m_allocation) {
      std::ostringstream str;
      str << "Couldn't find allocation ";
      str << static_cast<void*>(get_value(std::forward<rhs_T>(rhs)));
      throw std::runtime_error(str.str());
    }
    this->count(Operation::copy, 1);
  }
  ///
  template < NonWrapped rhs_T >
  Wrapper(rhs_T& rhs)
  requires(is_ref)
    : m_value(&rhs)
  {
    m_allocation = CountingData::current_data->get_allocation(
          static_cast<void*>(m_value));
    if (!m_allocation) {
      throw std::runtime_error("Couldn't find allocation");
    }
  }

  // copy and move constructors
  Wrapper(Wrapper const& rhs)
  requires(!(is_val && is_array))
    : m_value(rhs.get_native())
    , m_allocation(rhs.m_allocation)
  {
    if constexpr (is_val) {
      this->count(Operation::copy, 1);
    }
  }
  ///
  Wrapper(Wrapper && rhs)
  requires(!(is_val && is_array))
    : m_value(std::move(rhs).get_native())
    , m_allocation(rhs.m_allocation)
  {
    if constexpr (is_val) {
      this->count(Operation::copy, 1);
    }
  }

  // count assignments from non-wrapped values
  template < NonWrapped rhs_T >
  Wrapper& operator=(rhs_T&& rhs)
  requires(!is_array)
  {
    this->set(std::forward<rhs_T>(rhs));
    if constexpr (is_ptr) {
      this->m_allocation = CountingData::current_data->get_allocation(
          (void*)(m_value));
      if (!m_allocation) {
        throw std::runtime_error("Couldn't find allocation");
      }
    }
    this->count(Operation::assign, 1);
    return *this;
  }
  ///
  Wrapper& operator=(std::nullptr_t)
  requires(is_val && is_ptr)
  {
    return (*this) = Wrapper();
  }

  // count assignments from wrappers
  Wrapper& operator=(Wrapper const& rhs)
  requires(!is_array)
  {
    this->set(rhs.get_native());
    if constexpr (is_ptr) {
      this->m_allocation = rhs.m_allocation;
    }
    this->count(Operation::assign, 1);
    return *this;
  }
  ///
  Wrapper& operator=(Wrapper&& rhs)
  requires(!is_array)
  {
    this->set(std::move(rhs).get_native());
    if constexpr (is_ptr) {
      this->m_allocation = rhs.m_allocation;
    }
    this->count(Operation::assign, 1);
    return *this;
  }
  ///
  template < Wrapped rhs_T >
  Wrapper& operator=(rhs_T&& rhs)
  requires(!is_array)
  {
    this->set(std::forward<rhs_T>(rhs).get_native());
    if constexpr (is_ptr) {
      this->m_allocation = rhs.m_allocation;
    }
    this->count(Operation::assign, 1);
    return *this;
  }

  ~Wrapper()
  {
    if constexpr (is_val && is_array) {
      deregisterArray();
    }
  }


#define RAJAPERF_DEFINE_WRAPPER_PRE_OPERATOR(op_name, op, op_enum) \
    auto& op_name() \
    requires(!is_array) \
    { \
      this->set(this->get_native() op 1); \
      this->count(op_enum, 1); \
      return *this; \
    }

  RAJAPERF_DEFINE_WRAPPER_PRE_OPERATOR(operator++, +, Operation::preinc)
  RAJAPERF_DEFINE_WRAPPER_PRE_OPERATOR(operator--, -, Operation::predec)


#define RAJAPERF_DEFINE_WRAPPER_POST_OPERATOR(op_name, op, op_enum) \
    auto op_name(int) \
    requires(!is_array) \
    { \
      auto value = this->get_value_wrapper(); \
      this->set(value.get_native() op 1); \
      this->count(op_enum, 1); \
      return value; \
    }

  RAJAPERF_DEFINE_WRAPPER_POST_OPERATOR(operator++, +, Operation::postinc)
  RAJAPERF_DEFINE_WRAPPER_POST_OPERATOR(operator--, -, Operation::postdec)


#define RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(op_name, op, op_enum) \
    template < WrappedNonPtr rhs_T > \
    auto& op_name(rhs_T const& rhs) \
    requires(!is_array && !is_ptr) \
    { \
      this->set(this->get_native() op rhs.get_native()); \
      this->count(op_enum, 1); \
      return *this; \
    } \
    template < NonWrapped rhs_T > \
    auto& op_name(rhs_T const& rhs) \
    requires(!is_array && !is_ptr) \
    { \
      this->set(this->get_native() op rhs); \
      this->count(op_enum, 1); \
      return *this; \
    }

  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator+=,  +,  Operation::add)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator-=,  -,  Operation::sub)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator*=,  *,  Operation::mult)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator/=,  /,  Operation::div)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator%=,  %,  Operation::rem)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator&=,  &,  Operation::bit_and)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator|=,  |,  Operation::bit_or)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator^=,  ^,  Operation::bit_xor)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator<<=, <<, Operation::bit_lsh)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator>>=, >>, Operation::bit_rsh)


#define RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_POINTER_OPERATOR(op_name, op, op_enum) \
  template < WrappedNonPtr rhs_T > \
  auto& op_name(rhs_T const& rhs) \
  requires(is_ptr) \
  { \
    this->m_value op##= rhs.get_native(); \
    this->count(op_enum, 1); \
    return *this; \
  } \
  template < NonWrapped rhs_T > \
  auto& op_name(rhs_T const& rhs) \
  requires(is_ptr) \
  { \
    this->m_value op##= rhs; \
    this->count(op_enum, 1); \
    return *this; \
  }

  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_POINTER_OPERATOR(operator+=, +, Operation::add)
  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_POINTER_OPERATOR(operator-=, -, Operation::sub)


#define RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_ATOMIC_OPERATOR(op_name, op, op_enum) \
    template < WrappedNonPtr rhs_T > \
    auto& op_name(rhs_T const& rhs) \
    requires(is_ref) \
    { \
      this->set(this->get_native(0) op rhs.get_native(), 0); \
      this->count(op_enum, 1); \
      return *this; \
    } \
    template < NonWrapped rhs_T > \
    auto& op_name(rhs_T const& rhs) \
    requires(is_ref) \
    { \
      this->set(this->get_native(0) op rhs, 0); \
      this->count(op_enum, 1); \
      return *this; \
    }

  RAJAPERF_DEFINE_WRAPPER_COMPOUND_ASSIGN_ATOMIC_OPERATOR(atomic_add,  +,  Operation::atomic_add)


  auto operator&()
  requires(is_ref)
  {
    return Wrapper<pointed_to_type*>(m_allocation, m_value);
  }
  ///
  auto operator&() const
  requires(is_ref)
  {
    return Wrapper<pointed_to_type const*>(m_allocation, m_value);
  }


  auto operator*()
  requires(is_array || is_ptr)
  {
    if constexpr (is_val) {
      if constexpr (is_array) {
        return Wrapper<pointed_to_type&>(m_allocation, &*m_value);
      } else {
        return Wrapper<pointed_to_type&>(m_allocation, m_value);
      }
    } else {
      if constexpr (is_array) {
        return Wrapper<pointed_to_type&>(m_allocation, &**m_value);
      } else {
        return Wrapper<pointed_to_type&>(nullptr, *m_value);
      }
    }
  }
  ///
  auto operator*() const
  requires(is_array || is_ptr)
  {
    if constexpr (is_val) {
      if constexpr (is_array) {
        return Wrapper<const_pointed_to_type&>(m_allocation, &*m_value);
      } else {
        return Wrapper<pointed_to_type&>(m_allocation, m_value);
      }
    } else {
      if constexpr (is_array) {
        return Wrapper<const_pointed_to_type&>(m_allocation, &*(*m_value));
      } else {
        return Wrapper<pointed_to_type&>(nullptr, (*m_value));
      }
    }
  }

  auto operator->() const
  requires(is_ptr)
  {
    return m_value;
  }


  template < convertible_to<Index_type> I >
  auto operator[](I&& i)
  requires(is_array || is_ptr)
  {
    this->count(Operation::add, 1);
    if constexpr (is_val) {
      if constexpr (is_array) {
        return Wrapper<pointed_to_type&>(m_allocation, &m_value[get_value(std::forward<I>(i), 1)]);
      } else {
        return Wrapper<pointed_to_type&>(m_allocation, m_value+get_value(std::forward<I>(i), 1));
      }
    } else {
      if constexpr (is_array) {
        return Wrapper<pointed_to_type&>(m_allocation, &(*m_value)[get_value(std::forward<I>(i), 1)]);
      } else {
        return Wrapper<pointed_to_type&>(nullptr, (*m_value)+get_value(std::forward<I>(i), 1));
      }
    }
  }
  ///
  template < convertible_to<Index_type> I >
  auto operator[](I&& i) const
  requires(is_array || is_ptr)
  {
    this->count(Operation::add, 1);
    if constexpr (is_val) {
      if constexpr (is_array) {
        return Wrapper<const_pointed_to_type&>(m_allocation, &m_value[get_value(std::forward<I>(i), 1)]);
      } else {
        return Wrapper<pointed_to_type&>(m_allocation, m_value+get_value(std::forward<I>(i), 1));
      }
    } else {
      if constexpr (is_array) {
        return Wrapper<const_pointed_to_type&>(m_allocation, &(*m_value)[get_value(std::forward<I>(i), 1)]);
      } else {
        return Wrapper<pointed_to_type&>(nullptr, (*m_value)+get_value(std::forward<I>(i), 1));
      }
    }
  }

  operator auto() const
  {
    if constexpr (!is_array) {
      this->count(Operation::copy, 1);
      return this->get_native();
    }
  }
  ///
  explicit operator Wrapper<wrapped_type&>()
  requires(is_val)
  {
    if constexpr (is_array) {
      return Wrapper<wrapped_type&>(m_allocation, &m_value);
    } else {
      return Wrapper<wrapped_type&>(nullptr, &m_value);
    }
  }
  ///
  explicit operator Wrapper<wrapped_type const&>() const
  requires(is_val)
  {
    if constexpr (is_array) {
      return Wrapper<wrapped_type const&>(m_allocation, &m_value);
    } else {
      return Wrapper<wrapped_type const&>(nullptr, &m_value);
    }
  }

  void swap(Wrapper& rhs)
  requires(!is_array) // consider implementing array version later
  {
    using std::swap;
    value_type rhs_tmp(std::move(rhs).get_native());
    rhs.set(std::move(*this).get_native());
    this->set(std::move(rhs_tmp));
    swap(this->m_allocation, rhs.m_allocation);
  }

  void swap(Wrapper&& rhs) &&
  requires(is_ref && ! is_array)
  {
    using std::swap;
    value_type rhs_tmp(std::move(rhs).get_native());
    rhs.set(std::move(*this).get_native());
    this->set(std::move(rhs_tmp));
    swap(this->m_allocation, rhs.m_allocation);
  }


  // internal interface methods, should only be used in this file
  template < typename rhs_T >
  void set(rhs_T&& rhs, Size_type num_ops = 1)
  requires(!is_array)
  {
    if constexpr (is_val) {
      m_value = std::forward<rhs_T>(rhs);
    } else {
      this->count(Operation::store, num_ops);
      *m_value = std::forward<rhs_T>(rhs);
    }
  }

  // gets a copy of the value represented by this object
  auto get_value_wrapper(Size_type num_ops = 1) const
  requires(!is_array)
  {
    if constexpr (is_val) {
      return Wrapper<value_type>(m_allocation, m_value);
    } else {
      this->count(Operation::load, num_ops);
      return Wrapper<value_type>(nullptr, *m_value);
    }
  }

  // gets a reference to the underlying value
  auto&& get_native(Size_type num_ops = 1) &
  {
    if constexpr (is_val) {
      return m_value;
    } else {
      this->count(Operation::load, num_ops);
      return *m_value;
    }
  }
  ///
  auto&& get_native(Size_type num_ops = 1) &&
  {
    if constexpr (is_val) {
      return std::move(m_value);
    } else {
      this->count(Operation::load, num_ops);
      return *m_value;
    }
  }
  ///
  auto&& get_native(Size_type num_ops = 1) const&
  {
    if constexpr (is_val) {
      return m_value;
    } else {
      this->count(Operation::load, num_ops);
      return *m_value;
    }
  }
  ///
  auto&& get_native(Size_type num_ops = 1) const&&
  {
    if constexpr (is_val) {
      return std::move(m_value);
    } else {
      this->count(Operation::load, num_ops);
      return *m_value;
    }
  }


  template < typename U = T >
  void count(Operation op, Size_type num_ops) const
  {
    using V = std::decay_t<U>; // decay arrays to pointers

    if (!CountingData::current_context) {
      throw std::runtime_error("Can't count if there is no current context");
    }

    CountingData::current_context->operation_counters[
        Size_type(getOpType<V>())][Size_type(op)] += num_ops;

    if constexpr (std::is_pointer_v<V> && sizeof(std::remove_pointer_t<V>) > 1) {

      if (op == Operation::add || op == Operation::sub) {
        // Note that this fails to differentiate between
        // adding/subtracting a pointer and an integer which entails a mult or bit_lsh
        // and subtracting two pointers which entails a div or bit_rsh
        auto is_pow_2 = [](size_t n) { return (n & (n-1)) == size_t(0); };
        Operation extra_op = is_pow_2(sizeof(std::remove_pointer_t<V>))
            ? Operation::bit_lsh : Operation::mult ;
        CountingData::current_context->operation_counters[
            Size_type(getOpType<V>())][Size_type(extra_op)] += num_ops;
      }
    }

    if constexpr (std::is_reference_v<U>) {
      if (op == Operation::load || op == Operation::store ||
          op == Operation::atomic_add) {

        if (!m_allocation) {
          throw std::runtime_error("Memory access to unknown allocation");
        }

        auto base_ptr = static_cast<member_type>(m_allocation->ptr);
        check_bounds(base_ptr);

        if (num_ops > Size_type(0)) {
          CountingPoint point = CountingData::current_context->point;
          MemoryAccess access = MemoryAccess::NumMemoryAccesses;
          if (op == Operation::load) {
            access = MemoryAccess::read;
          } else if (op == Operation::store) {
            access = MemoryAccess::write;
          } else if (op == Operation::atomic_add) {
            access = MemoryAccess::atomicModifyWrite;
          }
          Size_type offset = m_value - base_ptr;
          CountingData::current_context->aloc_counts[m_allocation->idx].
              touch(point, access, offset, num_ops);
        }
      }
    }
  }

  void check_bounds(member_type base_ptr) const
  requires(is_ref)
  {
    if (!base_ptr) {
      throw std::runtime_error("Memory access to deallocated pointer");
    }
    if (m_value < base_ptr) {
      throw std::runtime_error("Memory access is out of bounds low");
    }
    if (m_value >= (base_ptr + m_allocation->size)) {
      throw std::runtime_error("Memory access is out of bounds high");
    }
  }

  void registerArray(std::source_location location = std::source_location::current())
  requires(is_val && is_array)
  {
    CountingData::current_data->add_team_allocation(
        get_type_name<std::remove_all_extents_t<wrapped_type>>(),
        static_cast<void*>(&m_value),
        get_array_size(), sizeof(std::remove_all_extents_t<wrapped_type>),
        static_cast<const void*>(&m_value), location);
  }

  void deregisterArray(std::source_location location = std::source_location::current())
  requires(is_val && is_array)
  {
    CountingData::current_data->remove_allocation(
        static_cast<void*>(&m_value),
        static_cast<const void*>(&m_value), location);
  }

private:
  member_type m_value;
  AllocationMetadata* m_allocation = nullptr;
};

template < typename U >
auto make_ValueWrapper(U&& value)
{
  return Wrapper<std::remove_cvref_t<U>>(value);
}

// Operations with Wrapper types
// Some of these will be found before functions of the same name in the
// global namespace

#define RAJAPERF_DEFINE_WRAPPER_UNARY_FUNCTION(op_name, op, op_enum) \
  template < typename T > \
  auto op_name(Wrapper<T> const& obj) \
  { \
    using ::op; \
    auto value = make_ValueWrapper(op(obj.get_native())); \
    value.count(op_enum, 1); \
    return value; \
  }

RAJAPERF_DEFINE_WRAPPER_UNARY_FUNCTION(exp,  exp,  Operation::exp)
RAJAPERF_DEFINE_WRAPPER_UNARY_FUNCTION(sqrt, sqrt, Operation::sqrt)
RAJAPERF_DEFINE_WRAPPER_UNARY_FUNCTION(fabs, fabs, Operation::abs)


#define RAJAPERF_DEFINE_WRAPPER_UNARY_OPERATOR(op_name, op, op_enum) \
  template < WrappedNonPtr T > \
  auto op_name(T const& obj) \
  { \
    auto value = make_ValueWrapper(op(obj.get_native())); \
    value.count(op_enum, 1); \
    return value; \
  }

RAJAPERF_DEFINE_WRAPPER_UNARY_OPERATOR(operator+, +, Operation::uplus)
RAJAPERF_DEFINE_WRAPPER_UNARY_OPERATOR(operator-, -, Operation::uminus)

template < WrappedPtr T >
auto operator+(T const& obj)
{
  Wrapper<std::remove_cvref_t<decltype(+(obj.get_native()))>> value(
      obj.m_allocation, +(obj.get_native()));
  value.count(Operation::uplus, 1);
  return value;
}


#define RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(op_name, op, op_enum) \
  template < NonWrapped lhs_T, Wrapped rhs_T > \
  auto op_name(lhs_T & lhs, \
               rhs_T const& rhs) \
  { \
    rhs.template count<lhs_T>(op_enum, 1); \
    return lhs op rhs.get_native(); \
  }

RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator+=,  +=,  Operation::add)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator-=,  -=,  Operation::sub)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator*=,  *=,  Operation::mult)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator/=,  /=,  Operation::div)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator%=,  %=,  Operation::rem)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator&=,  &=,  Operation::bit_and)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator|=,  |=,  Operation::bit_or)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator^=,  ^=,  Operation::bit_xor)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator<<=, <<=, Operation::bit_lsh)
RAJAPERF_DEFINE_NON_WRAPPER_COMPOUND_ASSIGN_OPERATOR(operator>>=, >>=, Operation::bit_rsh)


#define RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(op_name, op, op_enum) \
  template < WrappedNonPtr lhs_T, WrappedNonPtr rhs_T > \
  auto op_name(lhs_T const& lhs, \
               rhs_T const& rhs) \
  { \
    auto value = make_ValueWrapper(lhs.get_native() op rhs.get_native()); \
    value.count(op_enum, 1); \
    return value; \
  } \
  template < WrappedNonPtr lhs_T, NonWrapped rhs_T > \
  auto op_name(lhs_T const& lhs, \
               rhs_T const& rhs) \
  { \
    auto value = make_ValueWrapper(lhs.get_native() op rhs); \
    value.count(op_enum, 1); \
    return value; \
  } \
  template < NonWrapped lhs_T, WrappedNonPtr rhs_T > \
  auto op_name(lhs_T const& lhs, \
               rhs_T const& rhs) \
  { \
    auto value = make_ValueWrapper(lhs op rhs.get_native()); \
    value.count(op_enum, 1); \
    return value; \
  }

RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator+,  +,  Operation::add)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator-,  -,  Operation::sub)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator*,  *,  Operation::mult)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator/,  /,  Operation::div)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator%,  %,  Operation::rem)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator&,  &,  Operation::bit_and)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator|,  |,  Operation::bit_or)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator^,  ^,  Operation::bit_xor)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator<<, <<, Operation::bit_lsh)
RAJAPERF_DEFINE_WRAPPER_BINARY_OPERATOR(operator>>, >>, Operation::bit_rsh)


template < typename lhs_T, typename rhs_T >
auto operator+(Wrapper<lhs_T> const& lhs,
               Wrapper<rhs_T> const& rhs)
requires((Wrapper<lhs_T>::is_ptr || Wrapper<rhs_T>::is_ptr) &&
        !(Wrapper<lhs_T>::is_ptr && Wrapper<rhs_T>::is_ptr))
{
  if constexpr (Wrapper<lhs_T>::is_ptr) {
    auto value = lhs.get_value_wrapper();
    value += rhs;
    return value;
  } else {
    auto value = rhs.get_value_wrapper();
    value += lhs;
    return value;
  }
}
template < WrappedPtr lhs_T, NonWrapped rhs_T >
auto operator+(lhs_T const& lhs,
               rhs_T const& rhs)
{
  auto value = lhs.get_value_wrapper();
  value += rhs;
  return value;
}
template < NonWrapped lhs_T, WrappedPtr rhs_T >
auto operator+(lhs_T const& lhs,
               rhs_T const& rhs)
{
  auto value = rhs.get_value_wrapper();
  value += lhs;
  return value;
}

template < WrappedPtr lhs_T, WrappedNonPtr rhs_T >
auto operator-(lhs_T const& lhs,
               rhs_T const& rhs)
{
  auto value = lhs.get_value_wrapper();
  value -= rhs;
  return value;
}
template < WrappedPtr lhs_T, NonWrapped rhs_T >
auto operator-(lhs_T const& lhs,
               rhs_T const& rhs)
{
  auto value = lhs.get_value_wrapper();
  value -= rhs;
  return value;
}


#define RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(op_name, op, op_enum) \
  template < typename lhs_T, typename rhs_T > \
  auto op_name(Wrapper<lhs_T> const& lhs, \
               Wrapper<rhs_T> const& rhs) \
  { \
    lhs.template count<std::common_type_t<lhs_T, rhs_T>>(op_enum, 1); \
    return lhs.get_native() op rhs.get_native(); \
  } \
  template < typename lhs_T, NonWrapped rhs_T > \
  auto op_name(Wrapper<lhs_T> const& lhs, \
               rhs_T const& rhs) \
  { \
    lhs.template count<std::common_type_t<lhs_T, rhs_T>>(op_enum, 1); \
    return lhs.get_native() op rhs; \
  } \
  template < NonWrapped lhs_T, typename rhs_T > \
  auto op_name(lhs_T const& lhs, \
               Wrapper<rhs_T> const& rhs) \
  { \
    rhs.template count<std::common_type_t<lhs_T, rhs_T>>(op_enum, 1); \
    return lhs op rhs.get_native(); \
  }

RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator==, ==, Operation::eq)
RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator!=, !=, Operation::ne)
RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator< , < , Operation::lt)
RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator<=, <=, Operation::le)
RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator> , > , Operation::gt)
RAJAPERF_DEFINE_WRAPPER_COMPARISON_OPERATOR(operator>=, >=, Operation::ge)

template < Wrapped T >
void swap(T& lhs, T& rhs)
{
  lhs.swap(rhs);
}

template < WrappedRef T >
void swap(T&& lhs, T&& rhs)
{
  std::move(lhs).swap(std::move(rhs));
}

// helper for getting right type
template < template<size_t> typename T >
struct Array1WrapperHelper
{
  template < size_t N >
  using type = Wrapper<T<N>>;
};
///
template < template<size_t, size_t> typename T >
struct Array2WrapperHelper
{
  template < size_t N0, size_t N1 >
  using type = Wrapper<T<N0, N1>>;
};
///
template < template<size_t, size_t, size_t> typename T >
struct Array3WrapperHelper
{
  template < size_t N0, size_t N1, size_t N2 >
  using type = Wrapper<T<N0, N1, N2>>;
};
///
template < template<size_t, size_t, size_t, size_t> typename T >
struct Array4WrapperHelper
{
  template < size_t N0, size_t N1, size_t N2, size_t N3 >
  using type = Wrapper<T<N0, N1, N2, N3>>;
};

}  // closing brace for counting namespace

}  // closing brace for rajaperf namespace

namespace std
{

template < typename T >
struct iterator_traits<::rajaperf::counting::Wrapper<T*>>
{
  using difference_type = ::rajaperf::counting::Wrapper<std::ptrdiff_t>;
  using value_type = ::rajaperf::counting::Wrapper<std::remove_cv_t<T>>;
  using pointer = ::rajaperf::counting::Wrapper<T*>;
  using reference = ::rajaperf::counting::Wrapper<T&>;
  using iterator_category = std::random_access_iterator_tag;
};

}  // closing brace for std namespace


// Use this wrapper type in variable declarations in a kernel
// ex.
//   RAJAPERF_WRAPPER(my_struct*) val;
// Note wrapping is done for most types in CountingMacros.hpp, but some types
// like structs specific to a kernel need to be wrapped manually
// Note do not use it if declaring variables with constant values
#define RAJAPERF_WRAPPER(type) type

#endif  // closing endif for header file include guard
