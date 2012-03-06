/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#include "Elg/Performance/ScopeProfiler.h"

#include <cstdio>

#include "Elg/Utilities/Macros.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/Utilities/Types.h"
#include "Elg/Allocators/GlobalAllocator.h"

namespace Elg {
namespace Performance {
namespace Scope {

// Consts
const uint32 ELG_SCOPE_PROFILER_HASH_TABLE_SIZE = 2048;  // Should be the power of two for fast divide/modulo
const uint32 ELG_SCOPE_PROFILER_EMPTY_PARENT_MARKER = ELG_SCOPE_PROFILER_HASH_TABLE_SIZE;
const uint32 ELG_SCOPE_PROFILER_FNV_PRIME = 16777619;
const uint32 ELG_SCOPE_PROFILER_FNV_OFFSET = 2166136261;

// Structs
struct ScopeState {
  ScopeState()
    : CurrentIndex(ELG_SCOPE_PROFILER_EMPTY_PARENT_MARKER) {
  }

  uint32 CurrentIndex;
};

struct HashTableEntry {
  HashTableEntry()
    : Hash(ELG_SCOPE_PROFILER_FNV_PRIME), Empty(true) {
  }

  uint32 Hash;
  bool Empty;
};

struct ScopeStatistics {
  ScopeStatistics()
    : Max(0), Accumulator(0), SquaresAccumulator(0), HitCount(0), ParentIndex(0), ScopeId(0) {
  }

  uint64 Max;
  uint64 Accumulator;
  uint64 SquaresAccumulator;
  uint32 HitCount;
  uint32 ParentIndex;
  const char* ScopeId;
};

// Globals
uint32 g_hash_table_entry_count_ = 0;
HashTableEntry g_hash_table_[ELG_SCOPE_PROFILER_HASH_TABLE_SIZE+1];  // The final one is used for going up over roots
ScopeStatistics g_scope_statistics_[ELG_SCOPE_PROFILER_HASH_TABLE_SIZE];  // Indexed via hash table
ScopeState g_scope_state_;

// Hash table
FORCEINLINE uint32 HashCombine(uint32 seed, uint32 value) {
  uint32 combined = seed;

  combined *= ELG_SCOPE_PROFILER_FNV_PRIME;
  combined ^= ((value >> 24) & 0xFF);
  combined *= ELG_SCOPE_PROFILER_FNV_PRIME;
  combined ^= ((value >> 16) & 0xFF);
  combined *= ELG_SCOPE_PROFILER_FNV_PRIME;
  combined ^= ((value >> 8) & 0xFF);
  combined *= ELG_SCOPE_PROFILER_FNV_PRIME;
  combined ^= ((value) & 0xFF);

  return combined;
}

void EnterScope(uint32 scope_id_hash, const char* scope_id) {
  // Get the global state
  uint32 parent_index = g_scope_state_.CurrentIndex;
  uint32 parent_hash = g_hash_table_[parent_index].Hash;

  // Hash a pair of the
  uint32 comined_hash = HashCombine(parent_hash, scope_id_hash);

  // Initial guess
  uint32 index = comined_hash & (ELG_SCOPE_PROFILER_HASH_TABLE_SIZE-1);  // We have power of two as the size

  // Search until we either find the key, or find an empty slot (linear probing)
  while (!g_hash_table_[index].Empty && (g_hash_table_[index].Hash != comined_hash)) {
    index = (index+1) & (ELG_SCOPE_PROFILER_HASH_TABLE_SIZE-1);  // We have power of two as the size
  }

  // Create a new entry if we need it
  if (g_hash_table_[index].Empty) {
    g_hash_table_[index].Empty = false;
    g_hash_table_[index].Hash = comined_hash;

    g_scope_statistics_[index].ParentIndex = parent_index;
    g_scope_statistics_[index].ScopeId = scope_id;

    ++g_hash_table_entry_count_;

    ASSERT(g_hash_table_entry_count_ < ELG_SCOPE_PROFILER_HASH_TABLE_SIZE, "Scope profiler table full");
  }

  // Update the global state
  g_scope_state_.CurrentIndex = index;
}

void LeaveScope(uint64 time) {
  // Get the index
  uint32 index = g_scope_state_.CurrentIndex;
  uint32 parent_index = g_scope_statistics_[index].ParentIndex;

  g_scope_statistics_[index].Max = (time < g_scope_statistics_[index].Max ? g_scope_statistics_[index].Max : time);
  g_scope_statistics_[index].Accumulator += time;
  g_scope_statistics_[index].SquaresAccumulator += (time * time);
  g_scope_statistics_[index].HitCount += 1;

  g_scope_state_.CurrentIndex = parent_index;
}

// The tree data structure for dumping
class CallGraph {
 public:
  // Consts
  static const uint32 EMPTY_MARKER = 0xFFFFFFFF;

  // Constructor
  CallGraph() : m_root_(EMPTY_MARKER) {
    for (uint32 node_index = 0;node_index < ELG_SCOPE_PROFILER_HASH_TABLE_SIZE;++node_index) {
      m_left_child_[node_index] = EMPTY_MARKER;
    }
    for (uint32 node_index = 0;node_index < ELG_SCOPE_PROFILER_HASH_TABLE_SIZE;++node_index) {
      m_right_sibling_[node_index] = EMPTY_MARKER;
    }
  };

  // Functions
  void AddNewRoot(uint32 root_index) {
    m_right_sibling_[root_index] = m_root_;
    m_root_ = root_index;
  }

  void AddNewChild(uint32 parent_index, uint32 child_index) {
    m_right_sibling_[child_index] = m_left_child_[parent_index];
    m_left_child_[parent_index] = child_index;
  }

  void DumpStatistics(ScopeStatistics* data, FILE* output) {
    if (m_root_ != EMPTY_MARKER) {
      DumpRoot(m_root_, data, output);
    }
  }

  void DumpRoot(uint32 root_index, ScopeStatistics* data, FILE* output) {
    // Read the data to local vars
    uint64 max = data[root_index].Max;
    uint64 total = data[root_index].Accumulator;
    uint64 total_squares = data[root_index].SquaresAccumulator;
    uint32 hits = data[root_index].HitCount;
    const char* scope_id = data[root_index].ScopeId;

    // Converted data
    float32 total_f = static_cast<float32>(total);
    float32 total_squares_f = static_cast<float32>(total_squares);
    float32 hits_f = static_cast<float32>(hits);

    // Calculate additinal stats
    float32 avg = total_f / hits_f;
    float32 dev = 0.0;
    if (hits > 1) {
      dev = std::sqrt((total_squares_f - hits_f * avg * avg) / (hits_f - 1));
    }

    // Print
    fprintf(output, "[%s] Total: %lluns (100%%) Max: %lluns Avg: %.3fns Dev: %.3fns Hits: %lu (100%%)\n",
            scope_id, total, max, avg, dev, hits);

    // Recurse
    if (m_left_child_[root_index] != EMPTY_MARKER) {
      DumpNode(m_left_child_[root_index], root_index, data, output, 2);
    }
    if (m_right_sibling_[root_index] != EMPTY_MARKER) {
      DumpRoot(m_right_sibling_[root_index], data, output);
    }
  }

  void DumpNode(uint32 node_index, uint32 parent_index, ScopeStatistics* data, FILE* output, uint32 indent) {
    // Read parent data
    uint64 parent_total = data[parent_index].Accumulator;

    // Read the data to local vars
    uint64 max = data[node_index].Max;
    uint64 total = data[node_index].Accumulator;
    uint64 total_squares = data[node_index].SquaresAccumulator;
    uint32 hits = data[node_index].HitCount;
    const char* scope_id = data[node_index].ScopeId;

    // Converted data
    float32 parent_total_f = static_cast<float32>(parent_total);
    float32 total_f = static_cast<float32>(total);
    float32 total_squares_f = static_cast<float32>(total_squares);
    float32 hits_f = static_cast<float32>(hits);

    // Calculate additinal stats
    float32 parent_total_percent = 100 * total_f / parent_total_f;
    float32 avg = total_f / hits_f;
    float32 dev = 0.0;
    if (hits > 1) {
      dev = std::sqrt((total_squares_f - hits_f * avg * avg) / (hits_f - 1));
    }

    // Print
    fprintf(output, "%*s[%s] Total: %lluns (%.2f%%) Max: %lluns Avg: %.3fns Dev: %.3fns Hits: %lu\n",
            indent, "`-", scope_id, total, parent_total_percent, max, avg, dev, hits);

    // Recurse
    if (m_left_child_[node_index] != EMPTY_MARKER) {
      DumpNode(m_left_child_[node_index], node_index, data, output, indent+2);
    }
    if (m_right_sibling_[node_index] != EMPTY_MARKER) {
      DumpNode(m_right_sibling_[node_index], parent_index, data, output, indent);
    }
  }

 private:
  // Data members
  uint32 m_left_child_[ELG_SCOPE_PROFILER_HASH_TABLE_SIZE];
  uint32 m_right_sibling_[ELG_SCOPE_PROFILER_HASH_TABLE_SIZE];
  uint32 m_root_;
};

void DumpStatistics(FILE* output) {
  // Storage
  CallGraph* call_graph = ElgNew CallGraph();

  // Compute the tree structure
  for (uint32 table_index = 0;table_index < ELG_SCOPE_PROFILER_HASH_TABLE_SIZE;++table_index) {
    ScopeStatistics &current_statistics = g_scope_statistics_[table_index];
    if (current_statistics.ScopeId != NULL) {
      if (current_statistics.ParentIndex == ELG_SCOPE_PROFILER_EMPTY_PARENT_MARKER) {
        call_graph->AddNewRoot(table_index);
      } else {
        uint32 parent_index = current_statistics.ParentIndex;
        call_graph->AddNewChild(parent_index, table_index);
      }
    }
  }

  // Dump to output
  call_graph->DumpStatistics(g_scope_statistics_, output);

  // Delete the call graph
  ElgDelete call_graph;
}

}  // namespace Scope
}  // namespace Performance
}  // namespace Elg
