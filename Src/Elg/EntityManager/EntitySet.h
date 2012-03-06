/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_ENTITYSET_H_
#define ELG_ENTITYMANAGER_ENTITYSET_H_

#include <cstring>

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"

namespace Elg {
namespace EntityManager {

template<typename EntityManagerType>
class EntitySet {
 public:
  // Consts
  static const uint32 MaxEntityCount = EntityManagerType::MaxEntityCount;

  // Constructors
  explicit EntitySet(const EntityManagerType* owner)
    : m_entity_manager_(owner), m_current_count_(owner->GetActiveEntityCount()) {
    const uint32* entities_source = owner->GetActiveEntities();
    memcpy(m_entities_, entities_source, m_current_count_ * sizeof(*entities_source));
  }

  EntitySet(const EntityManagerType* owner, const uint32* entities, uint32 count)
      : m_entity_manager_(owner), m_current_count_(count) {
    ASSERT(count < MaxEntityCount, "Entity count exceeds MaxEntities of EntitySet");
    ASSERT(entities != NULL, "Entity array is NULL");

    memcpy(m_entities_, entities, count * sizeof(*entities));
  }

  EntitySet(const EntitySet& other)
      : m_entity_manager_(other.m_entity_manager_), m_current_count_(other.m_current_count_) {
    ASSERT(m_current_count_ < MaxEntityCount, "Attempt to overrun the entity storage while copying");

    memcpy(m_entities_, other.m_entities_, m_current_count_ * sizeof(*other.m_entities_));
  }

  // Destructor
  ~EntitySet() {
    // Nothing to do
  }

  // Operators
  EntitySet& operator=(const EntitySet& other) {
    m_entity_manager_ = other.m_entity_manager_;
    m_current_count_ = other.m_current_count_;

    ASSERT(m_current_count_ < MaxEntityCount, "Attempt to overrun the entity storage while copying");

    memcpy(m_entities_, other.m_entities_, m_current_count_ * sizeof(*other.m_entities_));

    return *this;
  }

  uint32 operator[](uint32 index) const {
    ASSERT(index < MaxEntityCount, "Index exceeds max entity count");
    ASSERT(index < m_current_count_, "Index exceeds entity count");
    return m_entities_[index];
  }

  // Member funtions
  template<typename T>
  EntitySet& Filter() {
    uint32 entity_index = 0;
    uint32 insertion_index = 0;

    while (entity_index < m_current_count_) {
      if (m_entity_manager_->HasComponent<T>(m_entities_[entity_index])) {
        m_entities_[insertion_index] = m_entities_[entity_index];
        ++insertion_index;
      }
      ++entity_index;
    }

    m_current_count_ = insertion_index;

    return *this;
  }

  const uint32* GetEntities() const {
    return m_entities_;
  }

  uint32* GetEntities() {
    return m_entities_;
  }

  uint32 GetCount() const {
    return m_current_count_;
  }

 private:
  // Data
  const EntityManagerType* m_entity_manager_;
  uint32 m_entities_[MaxEntityCount];
  uint32 m_current_count_;
};

}  // namespace EntityManager
}  // namespace Elg

#endif  // ELG_ENTITYMANAGER_ENTITYSET_H_
