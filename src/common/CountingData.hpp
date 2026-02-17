//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-25, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJAPerf_CountingData_HPP
#define RAJAPerf_CountingData_HPP

#include "common/RAJAPerfSuite.hpp"
#include "common/RPTypes.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <tuple>
#include <algorithm>
#include <typeinfo>
#include <type_traits>
#include <concepts>
#include <format>
#include <regex>
#include <source_location>


namespace rajaperf
{

namespace counting
{

enum struct OpType : int
{
  fp64,
  int32,
  int64,
  ptr,
  other,
  NumOpTypes // must be at the end of the valid values
};

template < typename T >
constexpr OpType getOpType()
{
  using decayed_T = std::decay_t<T>;
  if constexpr (std::is_floating_point_v<decayed_T> && sizeof(decayed_T) == sizeof(double)) {
    return OpType::fp64;
  } else if constexpr (std::is_integral_v<decayed_T> && sizeof(decayed_T) == sizeof(std::int32_t)) {
    return OpType::int32;
  } else if constexpr (std::is_integral_v<decayed_T> && sizeof(decayed_T) == sizeof(std::int64_t)) {
    return OpType::int64;
  } else if constexpr (std::is_pointer_v<decayed_T>) {
    return OpType::ptr;
  } else {
    return OpType::other;
  }
}

constexpr const char* getOpTypeName(OpType ot)
{
  switch (ot) {
    case OpType::int32: return "int32";
    case OpType::int64: return "int64";
    case OpType::ptr:   return "ptr";
    case OpType::fp64:  return "fp64";
    case OpType::other: return "other";
    default: throw std::invalid_argument("ot is not in OpType");
  }
}

template < typename T >
const char* get_type_name()
{
  OpType ot = getOpType<T>();
  if (ot != OpType::other) {
    return getOpTypeName(ot);
  } else {
    return typeid(T).name();
  }
}

enum struct Operation : int
{
  copy,
  assign,
  load,
  store,
  uplus,
  uminus,
  abs,
  add,
  sub,
  mult,
  div,
  rem,
  preinc,
  predec,
  postinc,
  postdec,
  atomic_add,
  sqrt,
  exp,
  bit_not,
  bit_and,
  bit_or,
  bit_xor,
  bit_lsh,
  bit_rsh,
  eq,
  ne,
  lt,
  le,
  gt,
  ge,
  NumOperations, // must be at the end of the valid values
  FLOP_begin = add, // used when counting what counts as a flop
  FLOP_end = eq     // used when counting what counts as a flop
};

constexpr const char* getOperationName(Operation op)
{
  switch (op) {
    case Operation::copy:       return "copy";
    case Operation::assign:     return "assign";
    case Operation::load:       return "load";
    case Operation::store:      return "store";
    case Operation::uplus:      return "uplus";
    case Operation::uminus:     return "uminus";
    case Operation::abs:        return "abs";
    case Operation::add:        return "add";
    case Operation::sub:        return "sub";
    case Operation::mult:       return "mult";
    case Operation::div:        return "div";
    case Operation::rem:        return "rem";
    case Operation::preinc:     return "preinc";
    case Operation::predec:     return "predec";
    case Operation::postinc:    return "postinc";
    case Operation::postdec:    return "postdec";
    case Operation::atomic_add: return "atomic_add";
    case Operation::sqrt:       return "sqrt";
    case Operation::exp:        return "exp";
    case Operation::bit_not:    return "bit_not";
    case Operation::bit_and:    return "bit_and";
    case Operation::bit_or:     return "bit_or";
    case Operation::bit_xor:    return "bit_xor";
    case Operation::bit_lsh:    return "bit_lsh";
    case Operation::bit_rsh:    return "bit_rsh";
    case Operation::eq:         return "eq";
    case Operation::ne:         return "ne";
    case Operation::lt:         return "lt";
    case Operation::le:         return "le";
    case Operation::gt:         return "gt";
    case Operation::ge:         return "ge";
    default: throw std::invalid_argument("op is not in Operation");
  }
}

enum struct ContextType : int
{
  exterior,
  outer,
  repetition,
  cond,
  outer_loop,
  seq_loop,
  par_loop,
  team,
  body,
  par_sync,
  team_sync,
  NumContextTypes // must be at the end of the valid values
};

constexpr const char* getContextTypeName(ContextType ct)
{
  switch (ct) {
    case ContextType::exterior:   return "exterior";
    case ContextType::outer:      return "outer";
    case ContextType::repetition: return "repetition";
    case ContextType::cond:       return "cond";
    case ContextType::outer_loop: return "outer_loop";
    case ContextType::seq_loop:   return "seq_loop";
    case ContextType::par_loop:   return "par_loop";
    case ContextType::team:       return "team";
    case ContextType::body:       return "body";
    case ContextType::par_sync:   return "par_sync";
    case ContextType::team_sync:  return "team_sync";
    default: throw std::invalid_argument("Unknown ContextType");
  }
}

enum struct MemoryAccess : int
{
  read,
  write,
  atomicModifyWrite,
  NumMemoryAccesses // must be at the end of the valid values
};

constexpr const char* getMemoryAccessName(MemoryAccess ma)
{
  switch (ma) {
    case MemoryAccess::read:              return "read";
    case MemoryAccess::write:             return "write";
    case MemoryAccess::atomicModifyWrite: return "atomicModifyWrite";
    default: throw std::invalid_argument("Unknown MemoryAccess");
  }
}

constexpr const char* getMemoryAccessNamePastTense(MemoryAccess ma)
{
  switch (ma) {
    case MemoryAccess::read:              return "read";
    case MemoryAccess::write:             return "written";
    case MemoryAccess::atomicModifyWrite: return "atomicModifyWritten";
    default: throw std::invalid_argument("Unknown MemoryAccess");
  }
}

constexpr const char* getMemoryAccessNamePastTenseTitle(MemoryAccess ma)
{
  switch (ma) {
    case MemoryAccess::read:              return "Read";
    case MemoryAccess::write:             return "Written";
    case MemoryAccess::atomicModifyWrite: return "AtomicModifyWritten";
    default: throw std::invalid_argument("Unknown MemoryAccess");
  }
}

enum struct AllocationGroup : int
{
  global,
  team,
  NumAllocationGroups // must be at the end of the valid values
};

constexpr const char* getAllocationGroupName(AllocationGroup ma)
{
  switch (ma) {
    case AllocationGroup::global: return "global";
    case AllocationGroup::team:   return "team";
    default: throw std::invalid_argument("Unknown AllocationGroup");
  }
}

// Must be in order innermost to outermost, so loop must be before rep, etc.
enum struct CountingPoint : int
{
  team,
  loop,
  rep,
  NumCountingPoints // must be at the end of the valid values
};

constexpr const char* getCountingPointName(CountingPoint ma)
{
  switch (ma) {
    case CountingPoint::team: return "team";
    case CountingPoint::loop: return "loop";
    case CountingPoint::rep:  return "rep";
    default: throw std::invalid_argument("Unknown CountingPoint");
  }
}


constexpr std::string get_spacing(Size_type depth)
{
  return std::string(depth*2, ' ');
}

struct MemoryCounts
{
  Size_type touched = 0;
  Size_type accessed[Size_type(MemoryAccess::NumMemoryAccesses)] = {0};

  void add(MemoryCounts const& other_counts, Size_type multiplier = 1)
  {
    touched += other_counts.touched * multiplier;
    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
      accessed[a] += other_counts.accessed[a] * multiplier;
    }
  }
};

struct AddressTouches
{
  std::vector<bool> address_accessed[Size_type(MemoryAccess::NumMemoryAccesses)];

  AddressTouches() = default;

  explicit AddressTouches(Size_type size, bool value = false)
  {
    resize(size, value);
  }

  void resize(Size_type size, bool value = false)
  {
    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
      address_accessed[a].resize(size, value);
    }
  }

  Size_type size() const
  {
    return address_accessed[0].size();
  }

  void set_all(Size_type size, bool value)
  {
    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
      for (Size_type i = 0; i < size; ++i) {
        address_accessed[a][i] = value;
      }
    }
  }

  void count(Size_type size,
             MemoryCounts& address_counts) const
  {
    for (Size_type i = 0; i < size; ++i) {
      bool addr_touched = false;
      for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
        bool addr_accessed = address_accessed[a][i];
        addr_touched = addr_touched || addr_accessed;
        address_counts.accessed[a] += addr_accessed ? 1 : 0;
      }
      address_counts.touched += addr_touched ? 1 : 0;
    }
  }

  void combine(Size_type size,
               AddressTouches const& other_touches)
  {
    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
      for (Size_type i = 0; i < size; ++i) {
        address_accessed[a][i] = other_touches.address_accessed[a][i] || address_accessed[a][i];
      }
    }
  }

  void clear()
  {
    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {
      address_accessed[a].clear();
      address_accessed[a].shrink_to_fit();
    }
  }
};

struct TouchCounts
{
  Size_type m_size = 0;
  MemoryCounts total_counts;
  MemoryCounts address_counts[Size_type(CountingPoint::NumCountingPoints)];
  AddressTouches address_touches[Size_type(CountingPoint::NumCountingPoints)];

  TouchCounts() = default;

  TouchCounts(CountingPoint point, Size_type size)
  {
    resize(point, size);
  }

  void resize(CountingPoint point, Size_type size)
  {
    for (Size_type p = Size_type(point);
         p < Size_type(CountingPoint::NumCountingPoints); ++p) {
      address_touches[p].resize(size);
    }
    m_size = size;
  }

  Size_type size() const
  {
    return m_size;
  }

  void set_all_accesses(CountingPoint point, bool value)
  {
    address_touches[Size_type(point)].set_all(m_size, value);
  }

  void touch(CountingPoint point, MemoryAccess access, Size_type offset,
             Size_type num_ops)
  {
    if (point < CountingPoint::NumCountingPoints) {
      total_counts.touched += num_ops;
      total_counts.accessed[Size_type(access)] += num_ops;
      address_touches[Size_type(point)].address_accessed[Size_type(access)].at(offset) = true;
    }
  }

  void count(CountingPoint point)
  {
    address_touches[Size_type(point)].count(m_size, address_counts[Size_type(point)]);
  }

  void combine_accesses(CountingPoint point,
                        TouchCounts const& other_touches,
                        CountingPoint other_point)
  {
    address_touches[Size_type(point)].combine(
        m_size, other_touches.address_touches[Size_type(other_point)]);
  }

  void clear_accesses(CountingPoint point)
  {
    address_touches[Size_type(point)].clear();
  }
};


struct AllocationMetadata
{
  Index_type idx = std::numeric_limits<Index_type>::min();
  const void* ptr_ptr = nullptr;
  std::source_location allocate_location;
  AllocationGroup group;

  void* ptr = nullptr;

  std::string pointed_to_type_name;
  Size_type element_size = 0;
  Size_type size = 0;

  TouchCounts counts;

  AllocationMetadata(Index_type idx_, const void* ptr_ptr_,
                     std::source_location location, AllocationGroup group_,
                     std::string pointed_to_type_name_, void* ptr_,
                     Size_type size_, Size_type element_size_)
    : idx(idx_)
    , ptr_ptr(ptr_ptr_)
    , allocate_location(location)
    , group(group_)
    , ptr(ptr_)
    , pointed_to_type_name(std::move(pointed_to_type_name_))
    , element_size(element_size_)
    , size(size_)
    , counts(CountingPoint(0), size_)
  {
  }

  void allocate(void* ptr_)
  {
    ptr = ptr_;
  }

  void deallocate()
  {
    ptr = nullptr;
  }

  void print_allocation(std::ostream& str, Size_type depth) const
  {
    std::string spacing = get_spacing(depth);
    spacing += "// ";
    str << spacing << pointed_to_type_name << "* allocation_" << idx
                   << " = " << getAllocationGroupName(group) << "_malloc("
                   << size << " * " << element_size << ");\n";
  }

  void print_deallocation(std::ostream& str, Size_type depth) const
  {
    std::string spacing = get_spacing(depth);
    spacing += "// ";
    str << spacing << getAllocationGroupName(group) << "_free("
                   << "allocation_" << idx << ");\n";
  }
};

struct Context
{
  Index_type idx = -1;
  Size_type hit_count = 0;
  ContextType type = ContextType::NumContextTypes;
  const char* text = nullptr;
  CountingPoint point = CountingPoint::NumCountingPoints;
  Index_type point_depth = 0;

  Context* parent = nullptr;
  // children are stored in order of increasing idx
  std::vector<std::unique_ptr<Context>> children;
  std::vector<Index_type> child_idcs;

  Size_type operation_counters[Size_type(OpType::NumOpTypes)][Size_type(Operation::NumOperations)] = {{0}};

  std::vector<TouchCounts> aloc_counts;

  MemoryCounts aloc_total_bytes;
  MemoryCounts aloc_totals_bytes[Size_type(CountingPoint::NumCountingPoints)];

  std::vector<Index_type> allocation_indices;
  std::vector<Index_type> deallocation_indices;

  static constexpr CountingPoint get_point(Context* parent, ContextType type)
  {
    CountingPoint point = CountingPoint::NumCountingPoints;
    if (type == ContextType::repetition) {
      point = CountingPoint::rep;
    } else if (type == ContextType::par_loop) {
      point = CountingPoint::loop;
    } else if (type == ContextType::team) {
      point = CountingPoint::team;
    }
    if (parent) {
      point = std::min(parent->point, point);
    }
    return point;
  }

  // depth of 0 indicates this does not have a valid point
  // depth of 1 indicates this is the first context with this point
  // depths greater than 1 are children of of a context of this point
  static constexpr Index_type get_depth(Context* parent, CountingPoint point)
  {
    Index_type depth = 0;
    if (parent) {
      if (point != parent->point) {
        depth = 1;
      } else if (parent->point_depth > 0) {
        depth = parent->point_depth + 1;
      }
    }
    return depth;
  }

  Context(Index_type idx_, Context* parent_, ContextType type_, const char* text_,
          std::vector<std::unique_ptr<AllocationMetadata>> const& allocations)
    : idx(idx_)
    , type(type_)
    , text(text_)
    , point(get_point(parent_, type_))
    , point_depth(get_depth(parent_, get_point(parent_, type_)))
    , parent(parent_)
    , aloc_counts(allocations.size())
  {
    if (type == ContextType::par_sync) {
      if (point != CountingPoint::rep) {
        throw std::runtime_error("par_sync must be in a repetition context");
      }
    } else if (type == ContextType::team_sync) {
      if (point != CountingPoint::team) {
        throw std::runtime_error("team_sync must be in a team context");
      }
    }
    for (Size_type i = 0; i < allocations.size(); ++i) {
      auto const& item = allocations[i];
      aloc_counts[i].resize(point, item->size);
    }
  }

  void update_allocations(std::vector<std::unique_ptr<AllocationMetadata>> const& allocations)
  {
    for (Size_type i = 0; i < allocations.size(); ++i) {
      auto const& item = allocations[i];
      if (i < aloc_counts.size()) {
        if (item->size != aloc_counts[i].size()) {
          throw std::runtime_error("Allocation record changed since last update");
        }
      } else {
        aloc_counts.resize(i+1);
        aloc_counts[i].resize(point, item->size);
      }
    }

    for (auto& child_ptr : children) {
      child_ptr->update_allocations(allocations);
    }
  }

  void add_allocation(AllocationMetadata const& item)
  {
    auto iter = std::ranges::find(allocation_indices, item.idx);
    if (iter == allocation_indices.end()) {
      allocation_indices.emplace_back(item.idx);
    }
  }

  void remove_allocation(AllocationMetadata const& item)
  {
    auto iter = std::ranges::find(deallocation_indices, item.idx);
    if (iter == deallocation_indices.end()) {
      deallocation_indices.emplace_back(item.idx);
    }
  }

  template < typename... Args >
  Context* get_or_emplace_child(Index_type idx, Args&&... args)
  {
    using std::distance;
    auto idx_iter = std::ranges::lower_bound(child_idcs, idx, std::ranges::less{});
    Size_type offset = distance(child_idcs.begin(), idx_iter);
    auto iter = children.begin() + offset;
    if (idx_iter == child_idcs.end() || *idx_iter != idx) {
      idx_iter = child_idcs.emplace(idx_iter, idx);
      iter = children.emplace(iter, std::make_unique<Context>(idx, this, std::forward<Args>(args)...));
    }
    return iter->get();
  }

  void count_totals(AllocationMetadata& item)
  {
    aloc_total_bytes.add(aloc_counts[item.idx].total_counts, item.element_size);
    item.counts.total_counts.add(aloc_counts[item.idx].total_counts, item.element_size);

    for (Size_type p = 0; p < Size_type(CountingPoint::NumCountingPoints); ++p) {
      aloc_totals_bytes[p].add(aloc_counts[item.idx].address_counts[p], item.element_size);
    }
  }

  void clear()
  {
    for (Size_type i = 0; i < aloc_counts.size(); ++i) {

      for (Size_type p = 0; p < Size_type(CountingPoint::NumCountingPoints); ++p) {
        aloc_counts[i].clear_accesses(CountingPoint(p));
      }

    }
  }


  void print_header(std::ostream& str, Size_type depth) const
  {
    std::string spacing = get_spacing(depth);
    spacing += "// ";

    str << spacing << "Line " << idx << " hit " << hit_count << " times\n";
  }

  void print_allocations(std::ostream& str, Size_type depth,
      std::vector<std::unique_ptr<AllocationMetadata>> const& allocations) const
  {
    for (Index_type const& allocation_idx : allocation_indices) {
      allocations[allocation_idx]->print_allocation(str, depth);
    }
    for (Index_type const& allocation_idx : deallocation_indices) {
      allocations[allocation_idx]->print_deallocation(str, depth);
    }
  }

  void print_allocation_counts(std::ostream& str, Size_type depth,
      std::string_view name,
      MemoryCounts const& mem_counts) const
  {
    std::string spacing = get_spacing(depth);
    spacing += "// ";

    if (mem_counts.touched) {
      str << spacing
          << name
          << " touched "
          << mem_counts.touched << "\n";
    }

    for (Size_type a = 0; a < Size_type(MemoryAccess::NumMemoryAccesses); ++a) {

      if (mem_counts.accessed[a]) {
        str << spacing
            << name
            << " " << getMemoryAccessNamePastTense(MemoryAccess(a)) << " "
            << mem_counts.accessed[a] << "\n";
      }

    }
  }

  void print_counters(std::ostream& str, Size_type depth) const
  {
    std::string spacing = get_spacing(depth);
    spacing += "// ";

    for (Size_type ot = 0; ot < Size_type(OpType::NumOpTypes); ++ot) {

      std::string opTypeName = getOpTypeName(OpType(ot));

      for (Size_type op = 0; op < Size_type(Operation::NumOperations); ++op) {

        std::string opName = getOperationName(Operation(op));

        Size_type num_ops = operation_counters[ot][op];

        if (num_ops > 0) {
          str << spacing << opTypeName << " " << opName << " " << num_ops << "\n";
        }
      }
    }

    print_allocation_counts(str, depth, "bytes", aloc_total_bytes);

    for (Size_type p = 0; p < Size_type(CountingPoint::NumCountingPoints); ++p) {

      std::string name = std::format("by {} bytes",
          getCountingPointName(CountingPoint(p)));

      print_allocation_counts(str, depth, name, aloc_totals_bytes[p]);

    }

    for (Size_type i = 0; i < aloc_counts.size(); ++i) {

      for (Size_type p = 0; p < Size_type(CountingPoint::NumCountingPoints); ++p) {

        std::string name = std::format("by {} allocation_{} elements",
            getCountingPointName(CountingPoint(p)), i);

        print_allocation_counts(str, depth,
            name, aloc_counts[i].address_counts[p]);

      }

    }

  }

  std::string replace_values(std::string str,
      std::vector<const char*> const& wrapper_formats) const
  {
    for (const char* wrapper_format : wrapper_formats) {
      std::regex re(std::vformat(wrapper_format, std::make_format_args("(.*?)")));
      str = std::regex_replace(str, re, "$1");
    }

    return str;
  }

  void print_text(std::ostream& str, Size_type depth,
      std::vector<const char*> const& wrapper_formats) const
  {
    if (text == nullptr) return;

    std::string spacing = get_spacing(depth);

    std::string new_text = replace_values(text, wrapper_formats);

    std::string_view tv = new_text;

    if (!tv.empty()) {

      Size_type pos = 0;
      while (pos < tv.size()) {

        // skip spacing between lines and extra semicolons
        if (std::isspace(tv[pos]) ||
            tv[pos] == ';') {
          ++pos;
          continue;
        }

        Size_type end = tv.find(';', pos);
        if (end < tv.size()) {
          end += 1;
        } else {
          end = tv.size();
        }

        str << spacing << tv.substr(pos, end-pos) << "\n";

        pos = end;
      }
    }
  }

  void print(std::ostream& str, Size_type depth, std::string_view tv) const
  {
    std::string spacing = get_spacing(depth);

    str << spacing << tv << "\n";
  }
};

struct CountingData;

struct ScopedContext
{
  Context* context;
  CountingData* countingData;

  ScopedContext(CountingData* countingData_, Context* context_)
    : context(context_)
    , countingData(countingData_)
  {
  }

  ScopedContext() = delete;
  ScopedContext(ScopedContext const&) = delete;
  ScopedContext(ScopedContext &&) = delete;
  ScopedContext& operator=(ScopedContext const&) = delete;
  ScopedContext& operator=(ScopedContext &&) = delete;

  ~ScopedContext()
  {
    pop_context();
  }

  void release()
  {
    countingData = nullptr;
    context = nullptr;
  }

  inline void pop_context();
};

struct CountingData
{
  static inline Context* current_context = nullptr;
  static inline CountingData* current_data = nullptr;

  Size_type par_it_per_rep_counter = 0;
  Size_type all_it_per_rep_counter = 0;

  Size_type max_par_loop_depth = 0;
  Size_type max_all_loop_depth = 0;

  Size_type kernel_per_rep_counter = 0;
  Size_type par_sync_per_rep_counter = 0;
  Size_type team_sync_per_rep_counter = 0;


  Size_type memory_allocations[Size_type(AllocationGroup::NumAllocationGroups)] = {0};
  Size_type memory_bytes[Size_type(AllocationGroup::NumAllocationGroups)] = {0};

  MemoryCounts memory_total_bytes[Size_type(AllocationGroup::NumAllocationGroups)];
  MemoryCounts memory_totals_bytes[Size_type(CountingPoint::NumCountingPoints)][Size_type(AllocationGroup::NumAllocationGroups)];

  std::vector<std::unique_ptr<AllocationMetadata>> allocations;


  Size_type operation_counters[Size_type(OpType::NumOpTypes)][Size_type(Operation::NumOperations)] = {{0}};


  std::unique_ptr<Context> counter_context;


  std::vector<const char*> wrapper_formats;


  void set_formats(std::initializer_list<const char*> wrapper_formats)
  {
    for (const char* wrapper_format : wrapper_formats) {
      this->wrapper_formats.emplace_back(wrapper_format);
    }
  }


  AllocationMetadata* get_allocation(const void* ptr)
  {
    if (!ptr) {
      return nullptr;
    }
    auto iter = std::ranges::find_if(allocations,
        [&](std::unique_ptr<AllocationMetadata> const& item) {
          if (!item->ptr) { return false; }
          const char* allocation_begin = static_cast<const char*>(item->ptr);
          const char* allocation_end = allocation_begin + item->size*item->element_size;
          return (allocation_begin <= static_cast<const char*>(ptr) &&
                  allocation_end   >  static_cast<const char*>(ptr));
        });
    if (iter == allocations.end()) {
      return nullptr;
    }
    return iter->get();
  }
  ///
  AllocationMetadata* get_allocation(const void* ptr_ptr, std::source_location location)
  {
    auto iter = std::ranges::find(allocations,
        std::make_tuple(ptr_ptr, location.line(), location.column()),
        [](std::unique_ptr<AllocationMetadata> const& item) {
          return std::make_tuple(item->ptr_ptr,
                                 item->allocate_location.line(),
                                 item->allocate_location.column());
        });
    if (iter == allocations.end()) {
      return nullptr;
    }
    return iter->get();
  }

  void add_allocation_impl(std::string pointed_to_type_name, AllocationGroup group, void* ptr,
                           Size_type size, Size_type element_size,
                           const void* ptr_ptr, std::source_location location)
  {
    auto item = get_allocation(ptr);
    if (item) {
      throw std::runtime_error("Allocation with this pointer already registered");
    }
    item = get_allocation(ptr_ptr, location);
    if (item) {
      if (pointed_to_type_name != item->pointed_to_type_name ||
          size != item->size ||
          element_size != item->element_size) {
        throw std::runtime_error("Allocation at this location changed type, size, or element_size");
      }
      item->allocate(ptr);
    } else {
      item = allocations.emplace_back(
          std::make_unique<AllocationMetadata>(
              allocations.size(), ptr_ptr, location, group,
              std::move(pointed_to_type_name), ptr, size, element_size)).get();
      counter_context->update_allocations(allocations);
      current_context->add_allocation(*item);
    }
  }

  void add_allocation(std::string pointed_to_type_name, void* ptr,
                      Size_type size, Size_type element_size,
                      const void* ptr_ptr, std::source_location location)
  {
    add_allocation_impl(std::move(pointed_to_type_name), AllocationGroup::global,
                   ptr, size, element_size,
                   ptr_ptr, location);
  }

  void add_team_allocation(std::string pointed_to_type_name, void* ptr,
                      Size_type size, Size_type element_size,
                      const void* ptr_ptr, std::source_location location)
  {
    add_allocation_impl(std::move(pointed_to_type_name), AllocationGroup::team,
                   ptr, size, element_size,
                   ptr_ptr, location);
  }

  void remove_allocation(void* ptr,
      [[maybe_unused]] const void* ptr_ptr,
      [[maybe_unused]] std::source_location location = std::source_location::current())
  {
    auto item = get_allocation(ptr);
    if (!item) {
      throw std::runtime_error("Allocation with this pointer not registered");
    }
    item->deallocate();
    current_context->remove_allocation(*item);
  }


  ScopedContext create_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    if (counter_context) {
      throw std::runtime_error("Already created exterior context");
    }

    counter_context = std::make_unique<Context>(
        location.line(), nullptr, ContextType::exterior, text, allocations);

    current_data = this;
    current_context = counter_context.get();

    current_context->hit_count += 1;

    return {this, current_context};
  }

  void push_context(ContextType type, const char* text,
      std::source_location location = std::source_location::current())
  {
    if (!current_data) {
      throw std::runtime_error("Current data not set");
    }
    if (!current_context) {
      throw std::runtime_error("Current context not set");
    }
    current_context = current_context->get_or_emplace_child(
        location.line(), type, text, allocations);
    current_context->hit_count += 1;
  }

  ScopedContext push_outer_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::outer, text, location);
    return {this, current_context};
  }

  ScopedContext push_rep_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::repetition, text, location);
    return {this, current_context};
  }

  ScopedContext push_cond_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::cond, text, location);
    return {this, current_context};
  }

  ScopedContext push_outer_loop_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::outer_loop, text, location);
    return {this, current_context};
  }

  ScopedContext push_seq_loop_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::seq_loop, text, location);
    return {this, current_context};
  }

  ScopedContext push_par_loop_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::par_loop, text, location);
    return {this, current_context};
  }

  ScopedContext push_body_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::body, text, location);
    return {this, current_context};
  }

  ScopedContext push_team_context(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::team, text, location);
    return {this, current_context};
  }

  void add_par_sync(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::par_sync, text, location);
    pop_context();
  }

  void add_team_sync(const char* text,
      std::source_location location = std::source_location::current())
  {
    push_context(ContextType::team_sync, text, location);
    pop_context();
  }

  void pop_context()
  {
    if (!current_context) {
      throw std::runtime_error("No context to pop");
    }
    if (current_context->point_depth == 1) {
      CountingPoint src_point = current_context->point;
      CountingPoint dst_point = current_context->parent
                              ? current_context->parent->point
                              : src_point;
      count_touches(current_context, src_point, dst_point, 0);
    }

    current_context = current_context->parent;
  }



  void finalize_context([[maybe_unused]] std::source_location location)
  {
    if (!counter_context) throw std::runtime_error("Exterior context not created");
    if (!current_context) throw std::runtime_error("No current context");
    if (current_context != counter_context.get()) throw std::runtime_error("Not at outer context");
    current_context = nullptr;
    current_data = nullptr;

    count_totals(counter_context.get(), 0);

    // count stats for allocations
    for (auto& item : allocations) {

      Size_type g = Size_type(item->group);

      memory_allocations[g] += 1;
      memory_bytes[g] += item->size * item->element_size;

      memory_total_bytes[g].add(item->counts.total_counts, item->element_size);

      for (Size_type p = 0; p < Size_type(CountingPoint::NumCountingPoints); ++p) {

        memory_totals_bytes[p][g].add(item->counts.address_counts[p], item->element_size);

        item->counts.clear_accesses(CountingPoint(p));
      }
    }

    count_kernels_and_iterations(counter_context.get());

    count_operations(counter_context.get());
  }

  void count_totals(Context* context, Size_type depth)
  {
    for (auto& child_ptr : context->children) {
      count_totals(child_ptr.get(), depth+1);
    }

    for (auto& item : allocations) {
      context->count_totals(*item);
    }
    context->clear();
  }

  void count_touches(Context* context, CountingPoint src_point,
      CountingPoint dst_point, Size_type depth)
  {
    for (auto& child_ptr : context->children) {
      count_touches(child_ptr.get(), src_point, dst_point, depth+1);
    }

    for (auto& item : allocations) {

      auto& src_counts = context->aloc_counts[item->idx];

      item->counts.combine_accesses(src_point, src_counts, src_point);

      if (dst_point < CountingPoint::NumCountingPoints &&
          dst_point != src_point) {

        context->aloc_counts[item->idx].combine_accesses(
            dst_point, src_counts, src_point);

        item->counts.combine_accesses(dst_point, src_counts, src_point);

      }

      src_counts.count(src_point);

      src_counts.set_all_accesses(src_point, false);

      if (depth == 0) {

        item->counts.count(src_point);

        item->counts.set_all_accesses(src_point, false);
      }
    }

  }

  std::array<Size_type, 2> count_kernels_and_iterations(
      Context* context,
      Size_type par_loop_stack_depth = 0,
      Size_type all_loop_stack_depth = 0)
  {
    if (!context->parent) {
      par_it_per_rep_counter = 0;
      all_it_per_rep_counter = 0;
      max_par_loop_depth = 0;
      max_all_loop_depth = 0;
      kernel_per_rep_counter = 0;
      par_sync_per_rep_counter = 0;
      team_sync_per_rep_counter = 0;
    }

    if (context->type == ContextType::par_loop) {
      par_loop_stack_depth += 1;
      all_loop_stack_depth += 1;
      max_par_loop_depth = std::max(par_loop_stack_depth, max_par_loop_depth);
      max_all_loop_depth = std::max(all_loop_stack_depth, max_all_loop_depth);
    } else if (context->type == ContextType::seq_loop) {
      all_loop_stack_depth += 1;
      max_all_loop_depth = std::max(all_loop_stack_depth, max_all_loop_depth);
    }

    Size_type max_child_par_iterations = 0;
    Size_type all_child_par_iterations = 0;
    Size_type max_child_iterations = 0;
    Size_type all_loop_iterations = 0;

    for (auto& child_ptr : context->children) {

      auto [par_iter, all_iter] =
          count_kernels_and_iterations(child_ptr.get(),
                                       par_loop_stack_depth,
                                       all_loop_stack_depth);

      max_child_par_iterations = std::max(par_iter, max_child_par_iterations);
      all_child_par_iterations += par_iter;
      max_child_iterations = std::max(child_ptr->hit_count, max_child_iterations);
      all_loop_iterations += all_iter;

    }

    Size_type child_par_iterations = all_child_par_iterations;
    Size_type child_all_iterations = all_loop_iterations;
    if (context->type == ContextType::seq_loop) {
      child_all_iterations = std::max(all_loop_iterations, max_child_iterations);
    }

    if (context->type == ContextType::team_sync) {
      team_sync_per_rep_counter += context->hit_count;
    } else if (context->type == ContextType::par_sync) {
      par_sync_per_rep_counter += context->hit_count;
    }

    if (Size_type(context->point) <= Size_type(CountingPoint::loop)) {

      if (context->point == CountingPoint::loop && context->point_depth == 1) {
        kernel_per_rep_counter += context->hit_count;
      }

      child_par_iterations = max_child_par_iterations;
      if (context->type == ContextType::par_loop) {
        child_par_iterations = std::max(max_child_par_iterations, max_child_iterations);
        child_all_iterations = std::max(all_loop_iterations, max_child_iterations);
      }

    }

    if (context->point == CountingPoint::rep && context->point_depth == 1) {
      par_it_per_rep_counter = all_child_par_iterations;
      all_it_per_rep_counter = all_loop_iterations;
    }

    return {{child_par_iterations, child_all_iterations}};

  }

  void count_operations(Context* context)
  {
    for (auto& child_ptr : context->children) {
      count_operations(child_ptr.get());
    }

    if (Size_type(context->point) > Size_type(CountingPoint::rep)) {
      return; // don't count operations outside of the repetition
    }

    for (Size_type ot = 0; ot < Size_type(OpType::NumOpTypes); ++ot) {
      for (Size_type op = 0; op < Size_type(Operation::NumOperations); ++op) {
        operation_counters[ot][op] += context->operation_counters[ot][op];
      }
    }
  }

  void print_context(std::ostream& str, Context const& context, Size_type depth) const
  {
    context.print_header(str, depth+1);

    context.print_allocations(str, depth+1, allocations);

    context.print_counters(str, depth+1);

    context.print_text(str, depth+1, wrapper_formats);

    if (!context.children.empty()) {

      context.print(str, depth+1, "{");

      for (auto const& child_ptr : context.children) {
        print_context(str, *child_ptr.get(), depth+1);
      }

      context.print(str, depth+1, "}");
    }
  }

  void print(std::ostream& str) const
  {
    Context const& context = *counter_context.get();
    Size_type depth = 0;
    context.print(str, depth, "{");
    print_context(str, context, depth);
    context.print(str, depth, "}");
  }
};

inline void ScopedContext::pop_context()
{
  if (context) {
    if (CountingData::current_context != context) {
      throw std::runtime_error("ScopedContext popped in wrong context");
    }
    if (CountingData::current_data != countingData) {
      throw std::runtime_error("ScopedContext popped in wrong context");
    }
    CountingData::current_data->pop_context();
    release();
  }
}

}  // closing brace for counting namespace

}  // closing brace for rajaperf namespace

#endif  // closing endif for header file include guard
