/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSETMANAGERS_POOLMANAGER_INL_H_
#define ELG_ASSETMANAGER_ASSETMANAGERS_POOLMANAGER_INL_H_

#include "Elg/AssetManager/AssetManagers/PoolManager.h"

#include <functional>
#include <algorithm>
#include <string>

#include "Elg/Allocators/GlobalAllocator.h"
#include "Elg/Clock/GameClock.h"

#include "Elg/AssetManager/Assets/Image.h"
#include "Elg/AssetManager/AssetDescriptor.h"
#include "Elg/AssetManager/Asset.h"

namespace Elg {
namespace AssetManager {
namespace AssetManagers {

// Constructors
template<typename Loader, uint32 PoolSize>
PoolManager<Loader, PoolSize>::PoolManager(const Elg::AssetManager::AssetDescriptor *asset_descriptors,
                                           uint32 asset_descriptor_count)
    : m_asset_id_count_(0),
      m_asset_id_hashes_(NULL),
      m_filenames_(NULL),
      m_internal_id_to_pool_slot_(NULL),
      m_loader_() {
  // Using directives
  using Elg::AssetManager::AssetDescriptor;

  // Temporary data structure
  const AssetDescriptor** valid_descriptors = ElgNew const AssetDescriptor*[asset_descriptor_count];
  ASSERT(valid_descriptors != NULL, "Memory allocation for temporary AssetDescriptor array failed");

  // Filter out appropriate descriptors
  for (uint32 descriptor_index = 0;descriptor_index < asset_descriptor_count;++descriptor_index) {
    if (asset_descriptors[descriptor_index].type_id_hash() == Loader::ASSET_TYPE_ID_HASH) {
      valid_descriptors[m_asset_id_count_++] = asset_descriptors + descriptor_index;
    }
  }

  // Sort according to hash
  std::sort(valid_descriptors, valid_descriptors + m_asset_id_count_, AssetDescriptorComparator());

  // Check if we got a duplicate hash
  ASSERT(HasNoDuplicates(valid_descriptors, m_asset_id_count_) == true, "Asset hash conflict detected");

  // Allocate the memory
  m_asset_id_hashes_ = ElgNew uint32[m_asset_id_count_];
  m_filenames_ = ElgNew std::string[m_asset_id_count_];
  m_internal_id_to_pool_slot_ = ElgNew uint32[m_asset_id_count_];

  ASSERT(m_asset_id_hashes_ != NULL, "Memory allocation for asset hashes array failed");
  ASSERT(m_filenames_ != NULL, "Memory allocation for asset filenames array failed");
  ASSERT(m_internal_id_to_pool_slot_ != NULL, "Memory allocation for asset id map failed");

  // Put data into structures
  for (uint32 descriptor_index = 0;descriptor_index < m_asset_id_count_;++descriptor_index) {
    STATIC_ANALYSIS_ASSUME(descriptor_index < asset_descriptor_count);  // Tell static code analysis that we are ok
    m_asset_id_hashes_[descriptor_index] = valid_descriptors[descriptor_index]->id_hash();
    m_filenames_[descriptor_index] = valid_descriptors[descriptor_index]->filename();
  }

  // Initialize the rest of the members [Internal id <-> pool mapping]
  for (uint32 internal_id_index = 0;internal_id_index < PoolSize;++internal_id_index) {
    m_internal_id_to_pool_slot_[internal_id_index] = PoolSize;
  }
  for (uint32 pool_slot_index = 0;pool_slot_index < PoolSize;++pool_slot_index) {
    m_pool_slot_to_internal_id_[pool_slot_index] = static_cast<uint64>(0);
  }

  // Initialize the rest of the members [asset managment]
  for (uint32 pool_index = 0;pool_index < PoolSize;++pool_index) {
    m_pool_usage_[pool_index] = Elg::Clock::GameClock::TimePoint::Epoch();
  }

  for (uint32 pool_index = 0;pool_index < PoolSize;++pool_index) {
    m_pool_[pool_index] = NULL;
  }

  // Delete the allocated memory
  ElgDelete[] valid_descriptors;
}

// Destructors
template<typename Loader, uint32 PoolSize>
PoolManager<Loader, PoolSize>::~PoolManager() {
  for (uint32 pool_index = 0;pool_index < PoolSize;++pool_index) {
    if (m_pool_usage_[pool_index] > Elg::Clock::GameClock::TimePoint::Epoch()) {
      m_loader_.Unload(m_pool_[pool_index]);

      m_pool_[pool_index] = NULL;
      m_pool_usage_[pool_index] = Elg::Clock::GameClock::TimePoint::Epoch();
      m_internal_id_to_pool_slot_[m_pool_slot_to_internal_id_[pool_index]-1] = PoolSize;
      m_pool_slot_to_internal_id_[pool_index] = 0;
    }
  }

  ElgDelete[] m_internal_id_to_pool_slot_;
}

// Methods, including static methods
template<typename Loader, uint32 PoolSize>
uint32 PoolManager<Loader, PoolSize>::Get(uint32 id_hash) {
  uint32* location = std::lower_bound(m_asset_id_hashes_, m_asset_id_hashes_ + m_asset_id_count_, id_hash);
  uint32 offset = (location - m_asset_id_hashes_);
  ASSERT(offset != m_asset_id_count_, "Requested asset with unknwon id");

  if (m_internal_id_to_pool_slot_[offset] == PoolSize) {
    // Get a free slot
    uint32 pool_slot = FreeSlot();

    // Load and set initial usage time
    LoadIntoSlot(offset, pool_slot);
    UpdateUsageTime(offset);
  }

  // Return the internal id
  return offset;
}

template<typename Loader, uint32 PoolSize>
typename Loader::loaded_type* PoolManager<Loader, PoolSize>::GetData(uint32 internal_id) {
  if (m_internal_id_to_pool_slot_[internal_id] == PoolSize) {
    // Since the asset is not loaded
    uint32 pool_slot = FreeSlot();
    LoadIntoSlot(internal_id, pool_slot);
  }

  UpdateUsageTime(internal_id);  // Update last usage time (or set initial usage time)
  return m_pool_[m_internal_id_to_pool_slot_[internal_id]];
}

template<typename Loader, uint32 PoolSize>
uint32 PoolManager<Loader, PoolSize>::FreeSlot() {
  // Try to locate a free slot (it will have usage time of 0) or a slot with earliest usage time -> just use min
  uint32 min_index = 0;
  for (uint32 slot_index = 1;slot_index < PoolSize;++slot_index) {
    if (m_pool_usage_[slot_index] < m_pool_usage_[min_index]) {
      min_index = slot_index;
    }
  }

  // So we got a good place - check if we got something to deallocate
  if (m_pool_usage_[min_index] > Elg::Clock::GameClock::TimePoint::Epoch()) {
    m_loader_.Unload(m_pool_[min_index]);

    m_pool_[min_index] = NULL;
    m_pool_usage_[min_index] = Elg::Clock::GameClock::TimePoint::Epoch();
    m_internal_id_to_pool_slot_[m_pool_slot_to_internal_id_[min_index]-1] = PoolSize;
    m_pool_slot_to_internal_id_[min_index] = 0;
  }

  return min_index;
}

template<typename Loader, uint32 PoolSize>
void PoolManager<Loader, PoolSize>::LoadIntoSlot(uint32 internal_id, uint32 slot) {
  std::string &filename = m_filenames_[internal_id];
  Loader::loaded_type* asset = m_loader_.Load(filename);
  m_pool_[slot] = asset;

  m_internal_id_to_pool_slot_[internal_id] = slot;
  m_pool_slot_to_internal_id_[slot] = internal_id+1;
}

template<typename Loader, uint32 PoolSize>
void PoolManager<Loader, PoolSize>::UpdateUsageTime(uint32 internal_id) {
  m_pool_usage_[m_internal_id_to_pool_slot_[internal_id]] = Elg::Clock::GameClock::Now();
}

template<typename Loader, uint32 PoolSize>
bool PoolManager<Loader, PoolSize>::HasNoDuplicates(const AssetDescriptor** descriptors, uint32 count) {
  bool is_unique = true;
  for (uint32 descriptor_index = 1;descriptor_index < count;++descriptor_index) {
    if (descriptors[descriptor_index-1]->id_hash() == descriptors[descriptor_index]->id_hash()) {
      is_unique = false;
      break;
    }
  }

  return is_unique;
}

template<typename Loader, uint32 PoolSize>
bool PoolManager<Loader, PoolSize>::AssetDescriptorComparator::operator()(const AssetDescriptor* lhs,
                                                                          const AssetDescriptor* rhs) {
  return (lhs->id_hash() < rhs->id_hash());
}

}  // namespace AssetManagers
}  // namespace AssetManager
}  // namespace Elg

#endif  // ELG_ASSETMANAGER_ASSETMANAGERS_POOLMANAGER_INL_H_
