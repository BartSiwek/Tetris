/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_DESCRIPTIONMACROS_H_
#define ELG_ENTITYMANAGER_DESCRIPTIONMACROS_H_

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "Elg/EntityManager/EntityPool/EntityPool.h"
#include "Elg/EntityManager/ComponentPool/ComponentPool.h"

#define ELG_ENTITY_MANAGER_DESCRIPTION_HEADER(ENTITY_COUNT)                                                           \
  static const uint32 MaxEntityCount = (ENTITY_COUNT);                                                                \
                                                                                                                      \
  typedef Elg::EntityManager::EntityPool<MaxEntityCount> EntityPoolType;                                              \
                                                                                                                      \
  EntityPoolType EntityPool;                                                                                          \
                                                                                                                      \
  template<typename ComponentType> struct ComponentPoolIndex {                                                        \
    enum { value = -1 };                                                                                              \
  };                                                                                                                  \
                                                                                                                      \
  template<typename ComponentType> struct ComponentMaxCount;                                                          \
                                                                                                                      \
  template<uint16 Index> struct ComponentPoolType;                                                                    \
                                                                                                                      \
  template<uint16 Index> typename ComponentPoolType<Index>::type& GetComponentPool();                                 \
  template<uint16 Index> const typename ComponentPoolType<Index>::type& GetComponentPool() const;


#define ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(COMPONENT_INDEX, COMPONENT_COUNT, ...)                               \
  static const uint16 BOOST_PP_CAT(MaxComponentInstances, COMPONENT_INDEX) = (COMPONENT_COUNT);                       \
                                                                                                                      \
  typedef Elg::EntityManager::ComponentPool< __VA_ARGS__,                                                             \
                                             BOOST_PP_CAT(MaxComponentInstances, COMPONENT_INDEX),                    \
                                             MaxEntityCount > BOOST_PP_CAT(ComponentPoolType, COMPONENT_INDEX);       \
                                                                                                                      \
  BOOST_PP_CAT(ComponentPoolType, COMPONENT_INDEX) BOOST_PP_CAT(ComponentPool, COMPONENT_INDEX);                      \
                                                                                                                      \
  template<>                                                                                                          \
  struct ComponentPoolIndex<__VA_ARGS__> {                                                                            \
    enum { value = (COMPONENT_INDEX) };                                                                               \
  };                                                                                                                  \
                                                                                                                      \
  template<>                                                                                                          \
  struct ComponentMaxCount<__VA_ARGS__> {                                                                             \
    enum { value = (COMPONENT_COUNT) };                                                                               \
  };                                                                                                                  \
                                                                                                                      \
  template<>                                                                                                          \
  struct ComponentPoolType<(COMPONENT_INDEX)> {                                                                       \
    typedef BOOST_PP_CAT(ComponentPoolType, COMPONENT_INDEX) type;                                                    \
  };                                                                                                                  \
                                                                                                                      \
  template<>                                                                                                          \
  ComponentPoolType<(COMPONENT_INDEX)>::type& GetComponentPool<(COMPONENT_INDEX)>() {                                 \
    return BOOST_PP_CAT(ComponentPool, COMPONENT_INDEX);                                                              \
  }                                                                                                                   \
                                                                                                                      \
  template<>                                                                                                          \
  const ComponentPoolType<(COMPONENT_INDEX)>::type& GetComponentPool<(COMPONENT_INDEX)>() const {                     \
    return BOOST_PP_CAT(ComponentPool, COMPONENT_INDEX);                                                              \
  }

#define ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_DESTROY_ALL_COMPONENTS(Z, N, DATA)                                      \
  if (BOOST_PP_CAT(ComponentPool, N).GetComponent(entity_id_index) != ELG_ENTITY_MANAGER_INVALID_ID) {                \
    BOOST_PP_CAT(ComponentPool, N).DestroyComponent(entity_id_index);                                                 \
  }

#define ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_SERIALIZE(Z, N, DATA)                                                   \
  BOOST_PP_CAT(ComponentPool, N).Serialize(out);

#define ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_DESERIALIZE(Z, N, DATA)                                                 \
  BOOST_PP_CAT(ComponentPool, N).Deserialize(in);

#define ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER(COMPONENT_COUNT)                                                        \
  static const uint16 ComponentCount = (COMPONENT_COUNT);                                                             \
                                                                                                                      \
  void DestroyAllComponents(uint32 entity_id_index) {                                                                 \
    BOOST_PP_REPEAT(COMPONENT_COUNT, ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_DESTROY_ALL_COMPONENTS, DONT_CARE)         \
  }                                                                                                                   \
                                                                                                                      \
  void Serialize(FILE* out) const {                                                                                   \
    BOOST_PP_REPEAT(COMPONENT_COUNT, ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_SERIALIZE, DONT_CARE)                      \
  }                                                                                                                   \
                                                                                                                      \
  void Deserialize(FILE* in) {                                                                                        \
    BOOST_PP_REPEAT(COMPONENT_COUNT, ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER_DESERIALIZE, DONT_CARE)                    \
  }

#endif  // ELG_ENTITYMANAGER_DESCRIPTIONMACROS_H_
