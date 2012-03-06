/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_ENTITYMANAGER_INL_H_
#define ELG_ENTITYMANAGER_ENTITYMANAGER_INL_H_

#include "Elg/EntityManager/EntityManager.h"

#include "Elg/Utilities/Asserts.h"
#include "Elg/Utilities/Macros.h"
#include "Elg/EntityManager/IdMacros.h"
#include "Elg/EntityManager/EntityManagerProfiling.h"

namespace Elg {
namespace EntityManager {

// Constructors
template<typename EntityManagerDescriptionType>
EntityManager<EntityManagerDescriptionType>::EntityManager()
    : m_description_() {
}

// Destructor
template<typename EntityManagerDescriptionType>
EntityManager<EntityManagerDescriptionType>::~EntityManager() {
  // Nothing to do
}

// Methods, including static methods [Entity managment]
template<typename EntityManagerDescriptionType>
inline uint32 EntityManager<EntityManagerDescriptionType>::CreateEntity() {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  return m_description_.EntityPool.CreateEntity();
}

template<typename EntityManagerDescriptionType>
inline uint32 EntityManager<EntityManagerDescriptionType>::GetActiveEntityCount() const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  return m_description_.EntityPool.GetActiveEntityCount();
}

template<typename EntityManagerDescriptionType>
inline const uint32* EntityManager<EntityManagerDescriptionType>::GetActiveEntities() const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  return m_description_.EntityPool.GetActiveEntities();
}

template<typename EntityManagerDescriptionType>
inline bool EntityManager<EntityManagerDescriptionType>::IsEntityActive(uint32 entity_id) const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  return (m_description_.EntityPool.GetEntityIndex(entity_id) != ELG_ENTITY_MANAGER_INVALID_ID);
}

template<typename EntityManagerDescriptionType>
inline void EntityManager<EntityManagerDescriptionType>::DestroyEntity(uint32 entity_id) {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  uint32 entity_id_index = m_description_.EntityPool.GetEntityIndex(entity_id);
  ASSERT(entity_id_index != ELG_ENTITY_MANAGER_INVALID_ID, "Attempt to destroy an inactive entity");

  m_description_.EntityPool.DestroyEntity(entity_id_index);
  m_description_.DestroyAllComponents(entity_id_index);
}

// Methods, including static methods [Component managment]
template<typename EntityManagerDescriptionType> template<typename T>
inline uint32 EntityManager<EntityManagerDescriptionType>::CreateComponent(uint32 entity_id) {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  uint32 entity_id_index = m_description_.EntityPool.GetEntityIndex(entity_id);
  ASSERT(entity_id_index != ELG_ENTITY_MANAGER_INVALID_ID, "Attempt to create a component for inactive entity");

  EntityManagerDescriptionType::ComponentPoolType<ComponentPoolIndex>::type& pool =
    m_description_.GetComponentPool<ComponentPoolIndex>();

  ASSERT(pool.GetComponent(entity_id_index) == ELG_ENTITY_MANAGER_INVALID_ID,
         "Attempt to create multiple component instances for entity");

  return pool.CreateComponent(entity_id_index, entity_id);
}

template<typename EntityManagerDescriptionType> template<typename T>
inline uint32 EntityManager<EntityManagerDescriptionType>::GetComponent(uint32 entity_id) const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  uint32 entity_id_index = m_description_.EntityPool.GetEntityIndex(entity_id);
  ASSERT(entity_id_index != ELG_ENTITY_MANAGER_INVALID_ID, "Attempt to get a component for inactive entity");

  uint32 offset = m_description_.GetComponentPool<ComponentPoolIndex>().GetComponent(entity_id_index);

  // Can be (InvalidOffset == ELG_ENTITY_MANAGER_INVALID_ID)
  return offset;
}

template<typename EntityManagerDescriptionType> template<typename T>
inline bool EntityManager<EntityManagerDescriptionType>::HasComponent(uint32 entity_id) const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  uint32 entity_id_index = m_description_.EntityPool.GetEntityIndex(entity_id);

  ASSERT(entity_id_index != ELG_ENTITY_MANAGER_INVALID_ID, "Attempt to query a component status on not active entity");

  uint32 offset = m_description_.GetComponentPool<ComponentPoolIndex>().GetComponent(entity_id_index);

  return (offset != ELG_ENTITY_MANAGER_INVALID_ID);
}

template<typename EntityManagerDescriptionType> template<typename T>
inline void EntityManager<EntityManagerDescriptionType>::DestroyComponent(uint32 entity_id) {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  uint32 entity_id_index = m_description_.EntityPool.GetEntityIndex(entity_id);
  ASSERT(entity_id_index != ELG_ENTITY_MANAGER_INVALID_ID, "Attempt to destroy a component for inactive entity");

  EntityManagerDescriptionType::ComponentPoolType<ComponentPoolIndex>::type& pool =
    m_description_.GetComponentPool<ComponentPoolIndex>();

  ASSERT(pool.GetComponent(entity_id_index) != ELG_ENTITY_MANAGER_INVALID_ID,
         "Attempt to destory a non-existing component");

  return pool.DestroyComponent(entity_id_index);
}

// Methods, including static methods [Component data]
template<typename EntityManagerDescriptionType> template<typename T>
inline uint32 EntityManager<EntityManagerDescriptionType>::GetDataCount() const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  return m_description_.GetComponentPool<ComponentPoolIndex>().GetDataCount();
}

template<typename EntityManagerDescriptionType> template<typename T>
inline const uint32* EntityManager<EntityManagerDescriptionType>::GetDataEntities() const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  return m_description_.GetComponentPool<ComponentPoolIndex>().GetDataEntities();
}

template<typename EntityManagerDescriptionType> template<typename T>
inline const T* EntityManager<EntityManagerDescriptionType>::GetData() const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  return m_description_.GetComponentPool<ComponentPoolIndex>().GetData();
}

template<typename EntityManagerDescriptionType> template<typename T>
inline T* EntityManager<EntityManagerDescriptionType>::GetData() {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  enum { ComponentPoolIndex = EntityManagerDescriptionType::ComponentPoolIndex<T>::value };

  STATIC_ASSERT(ComponentPoolIndex != -1, "Invalid component type used");

  return m_description_.GetComponentPool<ComponentPoolIndex>().GetData();
}

// Methods, including static methods [Serialisation]
template<typename EntityManagerDescriptionType>
inline void EntityManager<EntityManagerDescriptionType>::Serialize(FILE* out) const {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  // Serialzie EntityPool

  m_description_.Serialize(out);
}

template<typename EntityManagerDescriptionType>
inline void EntityManager<EntityManagerDescriptionType>::Deserialize(FILE* in) {
  ELG_PROFILE_ENTITY_MANAGER_SCOPE;

  // Deserialize EntityPool

  m_description_.Deserialize(in);
}

}  // namespace Elg
}  // namespace EntityManager

#endif  // ELG_ENTITYMANAGER_ENTITYMANAGER_INL_H_
