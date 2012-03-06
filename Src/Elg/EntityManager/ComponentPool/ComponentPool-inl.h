/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_COMPONENTPOOL_COMPONENTPOOL_INL_H_
#define ELG_ENTITYMANAGER_COMPONENTPOOL_COMPONENTPOOL_INL_H_

#include "Elg/EntityManager/ComponentPool/ComponentPool.h"

#include "Elg/Utilities/Types.h"
#include "Elg/EntityManager/IdMacros.h"

namespace Elg {
namespace EntityManager {

// Constructor
template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
ComponentPool<ComponentType, MaxInstances, MaxEntities>::ComponentPool()
    : m_top_(0) {
  // Set the entity id index -> storage index mapping to invalid values (aka empty)
  for (uint32 entity_id_index = 0;entity_id_index < MaxEntities;++entity_id_index) {
    m_entity_ids_[entity_id_index] = ELG_ENTITY_MANAGER_INVALID_ID;
  }

  // Set the storage index -> entity id index mapping to invalid values (aka empty)
  for (uint32 instance_index = 0;instance_index < MaxInstances;++instance_index) {
    m_active_entity_ids_[instance_index] = ELG_ENTITY_MANAGER_INVALID_ID;
  }
}

// Destructor
template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
ComponentPool<ComponentType, MaxInstances, MaxEntities>::~ComponentPool() {
  // Call the destructors on all allocated components
  for (uint32 instance_index = 0;instance_index < m_top_;++instance_index) {
    GetStorage(instance_index)->~ComponentType();
  }
}

// Methods, including static methods
template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
uint32 ComponentPool<ComponentType, MaxInstances, MaxEntities>::CreateComponent(uint32 entity_id_index,
                                                                                uint32 entity_id) {
  // Assert that the pool is not full (we assume that entity id index is in range)
  ASSERT(m_top_ < MaxInstances, "The maximum number of component instances was reached");

  // Get the instance index - we assume that the non-existence of a component was externaly checked
  uint32 instance_index = m_top_++;

  // Create the storage<->entity_id mapping (2xWRTIE)
  m_entity_ids_[entity_id_index] = instance_index;
  m_active_entity_ids_[instance_index] = entity_id;

  // Call placement new
  ComponentType* ptr = GetStorage(instance_index);
  new(ptr) ComponentType;

  // Return component id
  return instance_index;
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline uint32 ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetComponent(uint32 entity_id_index) const {
  // Just return
  return m_entity_ids_[entity_id_index];
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
void ComponentPool<ComponentType, MaxInstances, MaxEntities>::DestroyComponent(uint32 entity_id_index) {
  // Get the instance index (1xREAD) - we assume that the existence of a component was externaly checked
  uint32 instance_index = m_entity_ids_[entity_id_index];

  // Get the pointer to the top
  uint32 top_instance_index = --m_top_;

  // Get top entity index and generation (1xREAD)
  uint32 top_entity_id = m_active_entity_ids_[top_instance_index];
  uint32 top_entity_id_index = ELG_ENTITY_MANAGER_UNWRAP_ID(top_entity_id);

  // Adjust the entity index->instance index mapping (2xWRITE)
  m_entity_ids_[top_entity_id_index] = instance_index;
  m_entity_ids_[entity_id_index] = ELG_ENTITY_MANAGER_INVALID_ID;

  // Adjust the instance index->entity index mapping (1xREAD + 2xWRITE)
  m_active_entity_ids_[instance_index] = top_entity_id;
  m_active_entity_ids_[top_instance_index] = ELG_ENTITY_MANAGER_INVALID_ID;

  // Get the pointer to the top and removed component data
  ComponentType* top_ptr = GetStorage(top_instance_index);
  ComponentType* ptr = GetStorage(instance_index);

  // Move the top to the new location
  *ptr = std::move(*top_ptr);

  // Call the destructor (on the top object)
  top_ptr->~ComponentType();
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline uint32 ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetDataCount() const {
  return m_top_;
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline const uint32* ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetDataEntities() const {
  return m_active_entity_ids_;
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline ComponentType* ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetData() {
  return GetStorage(0);
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline const ComponentType* ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetData() const {
  return GetStorage(0);
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
void ComponentPool<ComponentType, MaxInstances, MaxEntities>::Serialize(FILE* /* out */) const {
  // Do serialzie
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
void ComponentPool<ComponentType, MaxInstances, MaxEntities>::Deserialize(FILE* /* in */) {
  // Do deserialize
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline ComponentType* ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetStorage(uint32 index) {
  ASSERT(index < MaxInstances, "Attempt to access storage with invalid index");
  return reinterpret_cast<ComponentType*>(m_storage_ + index);
}

template<typename ComponentType, uint16 MaxInstances, uint16 MaxEntities>
inline const ComponentType* ComponentPool<ComponentType, MaxInstances, MaxEntities>::GetStorage(uint32 index) const {
  ASSERT(index < MaxInstances, "Attempt to access storage with invalid index");
  return reinterpret_cast<const ComponentType*>(m_storage_ + index);
}

}  // namespace EntityManager
}  // namespace Elg

#endif  // ELG_ENTITYMANAGER_COMPONENTPOOL_COMPONENTPOOL_INL_H_
