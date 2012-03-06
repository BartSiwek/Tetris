/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_ENTITYPOOL_ENTITYPOOL_INL_H_
#define ELG_ENTITYMANAGER_ENTITYPOOL_ENTITYPOOL_INL_H_

#include "Elg/EntityManager/EntityPool/EntityPool.h"

#include "Elg/Utilities/Asserts.h"
#include "Elg/EntityManager/IdMacros.h"

namespace Elg {
namespace EntityManager {

template<uint16 MaxEntities>
EntityPool<MaxEntities>::EntityPool()
    : m_head_(0), m_top_(0) {
  // Make a free list with a cycle (empty if head is active i.e. m_active_[m_entity_ids_[m_head_]] != m_head_)
  for (uint32 entity_id = 0;entity_id < MaxEntities;++entity_id) {
    m_entity_ids_[entity_id] = ELG_ENTITY_MANAGER_WRAP_ID(entity_id+1, 0x7FFF);
  }
}

template<uint16 MaxEntities>
EntityPool<MaxEntities>::~EntityPool() {
}

template<uint16 MaxEntities>
uint32 EntityPool<MaxEntities>::CreateEntity() {
  // Assert the non-emptiness
  ASSERT(m_top_ < MaxEntities, "EntityPool overflow");
  ASSERT(m_head_ < MaxEntities, "EntityPool overflow");

  // Advance top and get the new_entity_active_index
  uint32 new_entity_active_index = m_top_++;

  // Get the id pointed by head (1xREAD)
  uint32 head_id = m_entity_ids_[m_head_];

  // Get the new_entity_id, new_entity_generation
  uint32 new_entity_id = m_head_;
  uint32 new_entity_generation = ELG_ENTITY_MANAGER_UNWRAP_GENERATION(head_id);

  // Store new_entity_active_index and new_entity_generation (1xWRITE)
  m_entity_ids_[m_head_] = ELG_ENTITY_MANAGER_WRAP_ID(new_entity_active_index, new_entity_generation);

  // Advance m_head_
  m_head_ = ELG_ENTITY_MANAGER_UNWRAP_ID(head_id);

  // Create a new_id
  uint32 new_id = ELG_ENTITY_MANAGER_WRAP_ID(new_entity_id, new_entity_generation);

  // Store the new id into the right index
  m_active_[new_entity_active_index] = new_id;

  // Return the result
  return new_id;
}

template<uint16 MaxEntities>
inline uint32 EntityPool<MaxEntities>::GetActiveEntityCount() const {
  return m_top_;
}

template<uint16 MaxEntities>
inline const uint32* EntityPool<MaxEntities>::GetActiveEntities() const {
  return m_active_;
}

template<uint16 MaxEntities>
inline uint32 EntityPool<MaxEntities>::GetEntityIndex(uint32 entity_id) const {
  // Unwrap received_id and received_generation
  uint32 generation = ELG_ENTITY_MANAGER_UNWRAP_GENERATION(entity_id);
  uint32 entity_id_index = ELG_ENTITY_MANAGER_UNWRAP_ID(entity_id);

  if (entity_id_index < MaxEntities) {
    // Get the current generation (1xREAD)
    uint32 read_id = m_entity_ids_[entity_id_index];
    uint32 current_generation = ELG_ENTITY_MANAGER_UNWRAP_GENERATION(read_id);

    return (generation == current_generation ? entity_id_index : ELG_ENTITY_MANAGER_INVALID_ID);
  } else {
    return ELG_ENTITY_MANAGER_INVALID_ID;
  }
}

template<uint16 MaxEntities>
void EntityPool<MaxEntities>::DestroyEntity(uint32 entity_id_index) {
  // Get the active index and generation (1xREAD) - we assume input has been prefiltered
  uint32 read_id = m_entity_ids_[entity_id_index];
  uint32 current_generation = ELG_ENTITY_MANAGER_UNWRAP_GENERATION(read_id);
  uint32 active_index = ELG_ENTITY_MANAGER_UNWRAP_ID(read_id);

  // Get the top id (1xREAD)
  uint32 top_id = m_active_[--m_top_];

  // Unwrap the id and generation
  uint32 top_entity_generation = ELG_ENTITY_MANAGER_UNWRAP_GENERATION(top_id);
  uint32 top_entity_id = ELG_ENTITY_MANAGER_UNWRAP_ID(top_id);

  // Store top_id into active index (1xWRITE)
  m_active_[active_index] = top_id;

  // Store the new value to top_entity_id (1xWRTIE)
  m_entity_ids_[top_entity_id] = ELG_ENTITY_MANAGER_WRAP_ID(active_index, top_entity_generation);

  // Store the advance the generation and adjust the list head (1xWRITE)
  uint32 next_generation = ELG_ENTITY_MANAGER_INC_GENERATION(current_generation);
  m_entity_ids_[entity_id_index] = ELG_ENTITY_MANAGER_WRAP_ID(m_head_, next_generation);
  m_head_ = entity_id_index;
}

}  // namespace EntityManager
}  // namespace Elg

#endif  // ELG_ENTITYMANAGER_ENTITYPOOL_ENTITYPOOL_INL_H_
