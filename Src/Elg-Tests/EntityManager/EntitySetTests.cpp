/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include <algorithm>

#include "Elg/Utilities/Types.h"
#include "Elg/EntityManager/EntityManager.h"
#include "Elg/EntityManager/DescriptionMacros.h"

template<uint32 Id>
struct Component {
};

struct EntityManagerDescription {
  ELG_ENTITY_MANAGER_DESCRIPTION_HEADER(1024);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(0, 1024, Component<0>);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(1, 1024, Component<1>);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(2, 1024, Component<2>);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(3, 1024, Component<3>);

  ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER(4);
};

typedef Elg::EntityManager::EntityManager<EntityManagerDescription> TestEntityManagerType;
typedef Elg::EntityManager::EntitySet<TestEntityManagerType> TestEntitySetType;

TEST(EntityManager_EntitySet_DefaultConstructor) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  TestEntitySetType set(my_em);

  CHECK_EQUAL(static_cast<uint32>(0), set.GetCount());

  delete my_em;
}

TEST(EntityManager_EntitySet_DefaultConstructorWithEntities) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();

  TestEntitySetType set(my_em);

  CHECK_EQUAL(static_cast<uint32>(3), set.GetCount());

  uint32* entities = set.GetEntities();
  uint32* entities_end = entities + set.GetCount();
  CHECK(std::find(entities, entities_end, e1) != entities_end);
  CHECK(std::find(entities, entities_end, e2) != entities_end);
  CHECK(std::find(entities, entities_end, e3) != entities_end);

  delete my_em;
}

TEST(EntityManager_EntitySet_ArrayConstructorOverflow) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;
  
  CHECK_THROW(TestEntitySetType(&my_em, my_em.GetDataEntities<Component<0>>(), 0xFFFFFFFF), 
              Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_EntitySet_ArrayConstructorNullArray) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  CHECK_THROW(TestEntitySetType(&my_em, NULL, 1), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_EntitySet_Empty) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  const uint32* data_entities = my_em->GetDataEntities<Component<0>>();
  uint32 data_count = my_em->GetDataCount<Component<0>>();
  TestEntitySetType set(my_em, data_entities, data_count);

  CHECK_EQUAL(static_cast<uint32>(0), set.GetCount());

  delete my_em;
}

TEST(EntityManager_EntitySet_WithEntities) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();

  my_em->CreateComponent<Component<0>>(e1);
  my_em->CreateComponent<Component<1>>(e2);
  my_em->CreateComponent<Component<0>>(e3);

  const uint32* data_entities = my_em->GetDataEntities<Component<0>>();
  uint32 data_count = my_em->GetDataCount<Component<0>>();
  TestEntitySetType set(my_em, data_entities, data_count);

  CHECK_EQUAL(static_cast<uint32>(2), set.GetCount());

  uint32* entities = set.GetEntities();
  uint32* entities_end = entities + set.GetCount();
  CHECK(std::find(entities, entities_end, e1) != entities_end);
  CHECK(std::find(entities, entities_end, e3) != entities_end);

  delete my_em;
}

TEST(EntityManager_EntitySet_CopyConstructor) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();

  TestEntitySetType set(my_em);
  TestEntitySetType other_set(set);
  set.Filter<Component<0>>();

  CHECK_EQUAL(static_cast<uint32>(0), set.GetCount());
  CHECK_EQUAL(static_cast<uint32>(3), other_set.GetCount());

  uint32* entities = other_set.GetEntities();
  uint32* entities_end = entities + other_set.GetCount();
  CHECK(std::find(entities, entities_end, e1) != entities_end);
  CHECK(std::find(entities, entities_end, e2) != entities_end);
  CHECK(std::find(entities, entities_end, e3) != entities_end);

  delete my_em;
}

TEST(EntityManager_EntitySet_AssignmentOperatorSelfAssign) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  uint32 created_ids[TestEntityManagerType::MaxEntityCount];
  for (uint32 i = 0;i < TestEntityManagerType::MaxEntityCount;++i) {
    created_ids[i] = my_em->CreateEntity();
  }

  TestEntitySetType set(my_em);

  // Do the self assign
  set = set;

  CHECK_EQUAL(TestEntityManagerType::MaxEntityCount, set.GetCount());

  uint32* entities = set.GetEntities();
  uint32* entities_end = entities + set.GetCount();

  for (uint32 i = 0;i < TestEntityManagerType::MaxEntityCount;++i) {
    CHECK(std::find(entities, entities_end, created_ids[i]) != entities_end);
  }

  delete my_em;
}

TEST(EntityManager_EntitySet_BracketOperator) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();
  uint32 e4 = my_em->CreateEntity();

  TestEntitySetType set(my_em);

  CHECK_EQUAL(static_cast<uint32>(4), set.GetCount());

  CHECK(set[0] == e1 || set[0] == e2 || set[0] == e3 || set[0] == e4);
  CHECK(set[1] == e1 || set[1] == e2 || set[1] == e3 || set[1] == e4);
  CHECK(set[2] == e1 || set[2] == e2 || set[2] == e3 || set[2] == e4);
  CHECK(set[3] == e1 || set[3] == e2 || set[3] == e3 || set[3] == e4);

  delete my_em;
}

TEST(EntityManager_EntitySet_Filter) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Preapore the inactive entity
  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();
  uint32 e4 = my_em->CreateEntity();
  uint32 e5 = my_em->CreateEntity();
  uint32 e6 = my_em->CreateEntity();
  uint32 e7 = my_em->CreateEntity();

  // e1 and e2 got Component<0>
  my_em->CreateComponent<Component<0>>(e1);
  my_em->CreateComponent<Component<0>>(e2);

  // e3 and e4 got Component<1>
  my_em->CreateComponent<Component<1>>(e3);
  my_em->CreateComponent<Component<1>>(e4);

  // e5, e6 and e7 got Component<2>
  my_em->CreateComponent<Component<2>>(e5);
  my_em->CreateComponent<Component<2>>(e6);
  my_em->CreateComponent<Component<2>>(e7);

  // e6 got Component<3>
  my_em->CreateComponent<Component<3>>(e6);

  // C0 Filtering
  TestEntitySetType* C0_C1_set = new TestEntitySetType(my_em);
  CHECK_EQUAL(static_cast<uint32>(7), C0_C1_set->GetCount());

  C0_C1_set->Filter<Component<0>>();
  CHECK_EQUAL(static_cast<uint32>(2), C0_C1_set->GetCount());

  uint32* C0_entities = C0_C1_set->GetEntities();
  uint32* C0_entities_end = C0_C1_set->GetEntities() + C0_C1_set->GetCount();
  CHECK(std::find(C0_entities, C0_entities_end, e1) != C0_entities_end);
  CHECK(std::find(C0_entities, C0_entities_end, e2) != C0_entities_end);

  // C1 Filtering
  C0_C1_set->Filter<Component<1>>();
  CHECK_EQUAL(static_cast<uint32>(0), C0_C1_set->GetCount());

  delete C0_C1_set;

  // C2 Filtering
  TestEntitySetType* C2_C3_set = new TestEntitySetType(my_em);
  CHECK_EQUAL(static_cast<uint32>(7), C2_C3_set->GetCount());

  C2_C3_set->Filter<Component<2>>();
  CHECK_EQUAL(static_cast<uint32>(3), C2_C3_set->GetCount());

  uint32* C2_C3_entities = C2_C3_set->GetEntities();
  uint32* C2_C3_entities_end = C2_C3_set->GetEntities() + C2_C3_set->GetCount();
  CHECK(std::find(C2_C3_entities, C2_C3_entities_end, e5) != C2_C3_entities_end);
  CHECK(std::find(C2_C3_entities, C2_C3_entities_end, e6) != C2_C3_entities_end);
  CHECK(std::find(C2_C3_entities, C2_C3_entities_end, e7) != C2_C3_entities_end);

  // C3 Filtering
  C2_C3_set->Filter<Component<3>>();
  CHECK_EQUAL(static_cast<uint32>(1), C2_C3_set->GetCount());

  C2_C3_entities = C2_C3_set->GetEntities();
  C2_C3_entities_end = C2_C3_set->GetEntities() + C2_C3_set->GetCount();
  CHECK(std::find(C2_C3_entities, C2_C3_entities_end, e6) != C2_C3_entities_end);

  delete C2_C3_set;

  // C3 Filtering
  TestEntitySetType* C3_C2_set = new TestEntitySetType(my_em);
  CHECK_EQUAL(static_cast<uint32>(7), C3_C2_set->GetCount());

  C3_C2_set->Filter<Component<3>>();
  CHECK_EQUAL(static_cast<uint32>(1), C3_C2_set->GetCount());

  uint32* C3_C2_entities = C3_C2_set->GetEntities();
  uint32* C3_C2_entities_end = C3_C2_set->GetEntities() + C3_C2_set->GetCount();
  CHECK(std::find(C3_C2_entities, C3_C2_entities_end, e6) != C3_C2_entities_end);

  // C2 Filtering
  C3_C2_set->Filter<Component<2>>();
  CHECK_EQUAL(static_cast<uint32>(1), C3_C2_set->GetCount());

  C3_C2_entities = C3_C2_set->GetEntities();
  C3_C2_entities_end = C3_C2_set->GetEntities() + C3_C2_set->GetCount();
  CHECK(std::find(C3_C2_entities, C3_C2_entities_end, e6) != C3_C2_entities_end);

  delete C3_C2_set;

  delete my_em;
}
