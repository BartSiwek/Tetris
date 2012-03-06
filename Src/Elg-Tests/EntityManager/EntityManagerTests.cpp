/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include <cstdlib>
#include <ctime>
#include <set>
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

TEST(EntityManager_DefaultState) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetActiveEntityCount());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<1>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<2>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<3>>());

  delete my_em;
}

// EntityManager::CreateEntity tests
TEST(EntityManager_CreateEntity_UniqueIds) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  uint32 entities[TestEntityManagerType::MaxEntityCount];

  for (uint32 count = 0;count < TestEntityManagerType::MaxEntityCount;++count) {
    uint32 new_e = my_em->CreateEntity();
    CHECK_EQUAL(entities+count, std::find(entities, entities+count, new_e));
    entities[count] = new_e;
  }

  delete my_em;
}

TEST(EntityManager_CreateEntity_Active) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  uint32 e = my_em->CreateEntity();

  CHECK_EQUAL(true, my_em->IsEntityActive(e));

  delete my_em;
}

TEST(EntityManager_CreateEntity_NoOverflowAfterDelete) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Create max possible entities
  uint32 entities[TestEntityManagerType::MaxEntityCount];
  for (uint32 count = 0;count < TestEntityManagerType::MaxEntityCount;++count) {
    // This should not fail as long as there are free spots
    uint32 e = my_em->CreateEntity();
    entities[count] = e;
  }

  // Test for overflow (not testing - is asserted against)
  // CHECK_THROW(my_em.CreateEntity(), Elg::EntityManager::EntityManagerException);

  // Make a slot avaiable
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  // The rand is thread safe on VS according to MSDN
  uint32 index_to_remove = (rand() % TestEntityManagerType::MaxEntityCount);  // NOLINT(runtime/threadsafe_fn)
  my_em->DestroyEntity(entities[index_to_remove]);

  // Should not fail
  my_em->CreateEntity();

  // Should fail again (not testing - is asserted against)
  // CHECK_THROW(my_em.CreateEntity(), Elg::EntityManager::EntityManagerException);

  delete my_em;
}

TEST(EntityManager_CreateEntity_NewEntityIsClean) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Create max possible entities
  uint32 entities[TestEntityManagerType::MaxEntityCount];
  for (uint32 count = 0;count < TestEntityManagerType::MaxEntityCount;++count) {
    // This should not fail as long as there are free spots
    uint32 e = my_em->CreateEntity();
    entities[count] = e;
    if (count % 4 == 0) {
      my_em->CreateComponent<Component<0>>(e);
    } else if (count % 4 == 1) {
      my_em->CreateComponent<Component<1>>(e);
    } else if (count % 4 == 2) {
      my_em->CreateComponent<Component<2>>(e);
    } else {
      my_em->CreateComponent<Component<3>>(e);
    }
  }

  // Make a slot avaiable
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  // The rand is thread safe on VS according to MSDN
  uint32 index_to_remove = (rand() % TestEntityManagerType::MaxEntityCount);  // NOLINT(runtime/threadsafe_fn)
  my_em->DestroyEntity(entities[index_to_remove]);

  // Should not fail
  uint32 e = my_em->CreateEntity();

  // Check we are active and have no components
  CHECK_EQUAL(true, my_em->IsEntityActive(e));
  CHECK_EQUAL(false, my_em->HasComponent<Component<0>>(e));
  CHECK_EQUAL(false, my_em->HasComponent<Component<1>>(e));
  CHECK_EQUAL(false, my_em->HasComponent<Component<2>>(e));
  CHECK_EQUAL(false, my_em->HasComponent<Component<3>>(e));

  delete my_em;
}

TEST(EntityManager_CreateEntity_Overflow) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;
  
  // Create max possible entities
  for(uint32 i = 0;i < TestEntityManagerType::MaxEntityCount;++i) {
    my_em.CreateEntity();
  }
  
  // Test for overflow
  CHECK_THROW(my_em.CreateEntity(), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_GetActiveEntityCount) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Create max possible entities
  for (uint32 i = 0;i < TestEntityManagerType::MaxEntityCount;++i) {
    CHECK_EQUAL(i, my_em->GetActiveEntityCount());
    my_em->CreateEntity();
  }
  CHECK_EQUAL(TestEntityManagerType::MaxEntityCount, my_em->GetActiveEntityCount());

  delete my_em;
}

TEST(EntityManager_GetActiveEntities) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  uint32 entities[TestEntityManagerType::MaxEntityCount];

  // Create max entities and check as we go
  for (uint32 count = 0;count < TestEntityManagerType::MaxEntityCount;++count) {
    // Create
    uint32 e = my_em->CreateEntity();
    entities[count] = e;

    // Check
    const uint32* active_entities = my_em->GetActiveEntities();
    for (uint32 entity_index = 0;entity_index < count+1;++entity_index) {
      bool found = false;
      for (uint32 active_index = 0;active_index < my_em->GetActiveEntityCount();++active_index) {
        if (entities[entity_index] == active_entities[active_index]) {
          found = true;
          break;
        }
      }
      CHECK_EQUAL(true, found);
    }
  }

  // Delete random entities and check as we go
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  for (uint32 count = TestEntityManagerType::MaxEntityCount;count > 0;--count) {
    // Delete
    // The rand is thread safe on VS according to MSDN
    uint32 index_to_remove = (rand() % count);  // NOLINT(runtime/threadsafe_fn)
    uint32 id_to_remove = entities[index_to_remove];
    entities[index_to_remove] = entities[count-1];

    my_em->DestroyEntity(id_to_remove);

    // Check
    const uint32* active_entities = my_em->GetActiveEntities();
    for (uint32 entity_index = 0;entity_index < count-1;++entity_index) {
      bool found = false;
      for (uint32 active_index = 0;active_index < my_em->GetActiveEntityCount();++active_index) {
        if (entities[entity_index] == active_entities[active_index]) {
          found = true;
          break;
        }
      }
      CHECK_EQUAL(true, found);
    }
  }

  delete my_em;
}

TEST(EntityManager_IsEntityActive) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  uint32 entities[TestEntityManagerType::MaxEntityCount];

  // Create max entities and check as we go
  for (uint32 count = 0;count < TestEntityManagerType::MaxEntityCount;++count) {
    // Create
    uint32 e = my_em->CreateEntity();
    entities[count] = e;

    // Check
    for (uint32 entity_index = 0;entity_index < count+1;++entity_index) {
      CHECK_EQUAL(true, my_em->IsEntityActive(entities[entity_index]));
    }
  }

  // Delete random entities and check as we go
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  for (uint32 count = TestEntityManagerType::MaxEntityCount;count > 0;--count) {
    // Delete
    // The rand is thread safe on VS according to MSDN
    uint32 index_to_remove = (rand() % count);  // NOLINT(runtime/threadsafe_fn)
    uint32 id_to_remove = entities[index_to_remove];
    entities[index_to_remove] = entities[count-1];

    my_em->DestroyEntity(id_to_remove);

    // Check
    CHECK_EQUAL(false, my_em->IsEntityActive(id_to_remove));

    for (uint32 entity_index = 0;entity_index < count-1;++entity_index) {
      CHECK_EQUAL(true, my_em->IsEntityActive(entities[entity_index]));
    }
  }

  delete my_em;
}

// EntityManager::DestoryEntity tests
TEST(EntityManager_DestoryEntity_InvalidEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Test destruction - should not fail
  my_em.DestroyEntity(0xFFFFFFFF);  // Internal knwoledge used about the structure of entity ids used
  */
}

TEST(EntityManager_DestoryEntity_NotActiveEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Test destruction - first destruction is ok - second is not okay but should not fail
  uint32 e = my_em.CreateEntity();
  my_em.DestroyEntity(e);
  my_em.DestroyEntity(e);
  */
}

TEST(EntityManager_DestoryEntity_ComponentsRemoved) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Create entity and some components
  uint32 e = my_em->CreateEntity();
  my_em->CreateComponent<Component<0>>(e);
  my_em->CreateComponent<Component<2>>(e);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e, my_em->GetDataEntities<Component<0>>()[0]);
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<1>>());
  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<2>>());
  CHECK_EQUAL(e, my_em->GetDataEntities<Component<2>>()[0]);
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<3>>());

  my_em->DestroyEntity(e);

  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<1>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<2>>());
  CHECK_EQUAL(static_cast<uint32>(0), my_em->GetDataCount<Component<3>>());

  delete my_em;
}

TEST(EntityManager_CreateComponent_InvalidEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Test creation (Internal knwoledge used about the structure of entity ids used)
  CHECK_THROW(my_em.CreateComponent<Component<0>>(0xFFFFFFFF), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_CreateComponent_EntityNotActive) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();
  my_em.DestroyEntity(e);

  // This should fail
  CHECK_THROW(my_em.CreateComponent<Component<0>>(e), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_CreateComponent_DoubleCreate) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();
  uint32 offset1 = my_em.CreateComponent<Component<0>>(e);
  uint32 offset2 = my_em.CreateComponent<Component<0>>(e);

  CHECK(offset1 == offset2);
  */
}

TEST(EntityManager_CreateComponent_DistinctComponents) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Prepare
  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();

  uint32 offset1 = my_em->CreateComponent<Component<0>>(e1);
  uint32 offset2 = my_em->CreateComponent<Component<0>>(e2);

  // Check
  CHECK(offset1 != offset2);

  delete my_em;
}

TEST(EntityManager_CreateComponent) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  const uint32* data_entities;
  const uint32* data_entities_end;

  // Preapore the inactive entity
  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();
  uint32 e4 = my_em->CreateEntity();
  uint32 e5 = my_em->CreateEntity();
  uint32 e6 = my_em->CreateEntity();
  uint32 e7 = my_em->CreateEntity();

  my_em->CreateComponent<Component<0>>(e1);
  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e1, my_em->GetDataEntities<Component<0>>()[0]);

  my_em->CreateComponent<Component<1>>(e2);
  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<1>>());
  CHECK_EQUAL(e2, my_em->GetDataEntities<Component<1>>()[0]);

  my_em->CreateComponent<Component<2>>(e3);
  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<2>>());
  CHECK_EQUAL(e3, my_em->GetDataEntities<Component<2>>()[0]);

  my_em->CreateComponent<Component<3>>(e4);
  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<3>>());
  CHECK_EQUAL(e4, my_em->GetDataEntities<Component<3>>()[0]);

  my_em->CreateComponent<Component<0>>(e5);
  my_em->CreateComponent<Component<1>>(e5);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<0>>());
  data_entities = my_em->GetDataEntities<Component<0>>();
  data_entities_end = my_em->GetDataEntities<Component<0>>() + my_em->GetDataCount<Component<0>>();
  CHECK(std::find(data_entities, data_entities_end, e1) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e5) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e5) != data_entities_end);

  my_em->CreateComponent<Component<1>>(e6);
  my_em->CreateComponent<Component<2>>(e6);

  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e5) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  my_em->CreateComponent<Component<2>>(e7);
  my_em->CreateComponent<Component<3>>(e7);

  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<3>>());
  data_entities = my_em->GetDataEntities<Component<3>>();
  data_entities_end = my_em->GetDataEntities<Component<3>>() + my_em->GetDataCount<Component<3>>();
  CHECK(std::find(data_entities, data_entities_end, e4) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  delete my_em;
}

TEST(EntityManager_GetComponent_InvalidEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Test creation (Internal knwoledge used about the structure of entity ids used)
  CHECK_THROW(my_em.CreateComponent<Component<0>>(0xFFFFFFFF), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_GetComponent_EntityNotActive) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();
  my_em.DestroyEntity(e);

  // This should fail
  CHECK_THROW(my_em.GetComponent<Component<0>>(e), Elg::EntityManager::EntityManagerException);
  */
}

TEST(EntityManager_GetComponent_NoComponent) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Preapore the inactive entity
  uint32 e = my_em->CreateEntity();

  // This should fail
  CHECK_EQUAL(TestEntityManagerType::InvalidOffset, my_em->GetComponent<Component<0>>(e));

  delete my_em;
}

TEST(EntityManager_GetComponent) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Prepare
  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();

  uint32 offset1 = my_em->CreateComponent<Component<0>>(e1);
  uint32 offset2 = my_em->CreateComponent<Component<0>>(e2);

  uint32 read_offset1 = my_em->GetComponent<Component<0>>(e1);
  CHECK_EQUAL(read_offset1, offset1);

  uint32 read_offset2 = my_em->GetComponent<Component<0>>(e2);
  CHECK_EQUAL(read_offset2, offset2);

  delete my_em;
}

TEST(EntityManager_HasComponent_InvalidEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Shound not throw
  CHECK(my_em.HasComponent<Component<0>>(0xFFFFFFFF) == false);
  */
}

TEST(EntityManager_HasComponent_EntityNotActive) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();
  my_em.DestroyEntity(e);

  // Shound not throw
  CHECK(my_em.HasComponent<Component<0>>(e) == false);
  */
}

TEST(EntityManager_HasComponent) {
  TestEntityManagerType* my_em = new TestEntityManagerType();

  // Preapore the inactive entity
  uint32 e = my_em->CreateEntity();

  // This should be false
  CHECK_EQUAL(false, my_em->HasComponent<Component<0>>(e));

  // Add one
  my_em->CreateComponent<Component<0>>(e);

  // This should be true now
  CHECK_EQUAL(true, my_em->HasComponent<Component<0>>(e));

  // Destroy one
  my_em->DestroyComponent<Component<0>>(e);

  // This should be false again
  CHECK_EQUAL(false, my_em->HasComponent<Component<0>>(e));

  delete my_em;
}

TEST(EntityManager_DestroyComponent_InvalidEntity) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Shound not throw
  my_em.DestroyComponent<Component<0>>(0xFFFFFFFF);
  */
}

TEST(EntityManager_DestroyComponent_EntityNotActive) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();
  my_em.DestroyEntity(e);

  // Shound not throw
  my_em.DestroyComponent<Component<0>>(e);
  */
}

TEST(EntityManager_DestroyComponent_NoComponent) {
  // This is just asserted - not testing

  /*
  TestEntityManagerType my_em;

  // Preapore the inactive entity
  uint32 e = my_em.CreateEntity();

  // Shound not throw
  my_em.DestroyComponent<Component<0>>(e);
  */
}

TEST(EntityManager_DestroyComponent) {
  TestEntityManagerType* my_em = new TestEntityManagerType();
  const uint32* data_entities;
  const uint32* data_entities_end;

  // Preapore the inactive entity
  uint32 e1 = my_em->CreateEntity();
  uint32 e2 = my_em->CreateEntity();
  uint32 e3 = my_em->CreateEntity();
  uint32 e4 = my_em->CreateEntity();
  uint32 e5 = my_em->CreateEntity();
  uint32 e6 = my_em->CreateEntity();
  uint32 e7 = my_em->CreateEntity();

  my_em->CreateComponent<Component<0>>(e1);
  my_em->CreateComponent<Component<0>>(e5);

  my_em->CreateComponent<Component<1>>(e2);
  my_em->CreateComponent<Component<1>>(e5);
  my_em->CreateComponent<Component<1>>(e6);

  my_em->CreateComponent<Component<2>>(e3);
  my_em->CreateComponent<Component<2>>(e6);
  my_em->CreateComponent<Component<2>>(e7);

  my_em->CreateComponent<Component<3>>(e4);
  my_em->CreateComponent<Component<3>>(e7);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<1>>());
  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<2>>());
  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<3>>());

  // Destroy
  my_em->DestroyComponent<Component<0>>(e1);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e5, my_em->GetDataEntities<Component<0>>()[0]);

  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e5) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<3>>());
  data_entities = my_em->GetDataEntities<Component<3>>();
  data_entities_end = my_em->GetDataEntities<Component<3>>() + my_em->GetDataCount<Component<3>>();
  CHECK(std::find(data_entities, data_entities_end, e4) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  // Destroy
  my_em->DestroyComponent<Component<1>>(e5);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e5, my_em->GetDataEntities<Component<0>>()[0]);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(3), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<3>>());
  data_entities = my_em->GetDataEntities<Component<3>>();
  data_entities_end = my_em->GetDataEntities<Component<3>>() + my_em->GetDataCount<Component<3>>();
  CHECK(std::find(data_entities, data_entities_end, e4) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  // Destroy
  my_em->DestroyComponent<Component<2>>(e6);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e5, my_em->GetDataEntities<Component<0>>()[0]);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<3>>());
  data_entities = my_em->GetDataEntities<Component<3>>();
  data_entities_end = my_em->GetDataEntities<Component<3>>() + my_em->GetDataCount<Component<3>>();
  CHECK(std::find(data_entities, data_entities_end, e4) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  // Destroy
  my_em->DestroyComponent<Component<3>>(e7);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<0>>());
  CHECK_EQUAL(e5, my_em->GetDataEntities<Component<0>>()[0]);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<1>>());
  data_entities = my_em->GetDataEntities<Component<1>>();
  data_entities_end = my_em->GetDataEntities<Component<1>>() + my_em->GetDataCount<Component<1>>();
  CHECK(std::find(data_entities, data_entities_end, e2) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e6) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(2), my_em->GetDataCount<Component<2>>());
  data_entities = my_em->GetDataEntities<Component<2>>();
  data_entities_end = my_em->GetDataEntities<Component<2>>() + my_em->GetDataCount<Component<2>>();
  CHECK(std::find(data_entities, data_entities_end, e3) != data_entities_end);
  CHECK(std::find(data_entities, data_entities_end, e7) != data_entities_end);

  CHECK_EQUAL(static_cast<uint32>(1), my_em->GetDataCount<Component<3>>());
  CHECK_EQUAL(e4, my_em->GetDataEntities<Component<3>>()[0]);

  delete my_em;
}

/*
// EntityManager::Serialize and EntityManager::Deserialize tests
TEST(Serialize_Deserialize) {
  const uint32 max_entities = 32;
  MyEntityManager em(max_entities);
  
  // Prepare the em - entities
  std::vector<Entity> entities;
  std::vector<Entity> entities_to_delete;
  for(uint32 entity_index = 0;entity_index < max_entities;++entity_index) {
    if(entity_index % 2 == 0) {
      entities.push_back(em.CreateEntity());
    } else {
      entities_to_delete.push_back(em.CreateEntity());
    }
  }
  
  for(uint32 entity_index = 0;entity_index < entities_to_delete.size();++entity_index) {
    em.DestroyEntity(entities_to_delete[entity_index]);
  }
  entities_to_delete.clear();
  
  // Prepare the em - C<0> components (uint16 data)
  std::map<Entity, std::vector<C<0>> > entity_C0_components;
  
  C<0>* e0_c0 = em.AddComponent<C<0>>(entities[0]);
  e0_c0->data = (static_cast<uint16>(1) << 0);
  entity_C0_components[entities[0]].push_back(*e0_c0);
  
  C<0>* e1_c0 = em.AddComponent<C<0>>(entities[1]);
  C<0>* e1_c1 = em.AddComponent<C<0>>(entities[1]);
  e1_c0->data = (static_cast<uint16>(1) << 0);
  e1_c1->data = (static_cast<uint16>(1) << 1);
  entity_C0_components[entities[1]].push_back(*e1_c0);
  entity_C0_components[entities[1]].push_back(*e1_c1);
  
  C<0>* e2_c0 = em.AddComponent<C<0>>(entities[2]);
  C<0>* e2_c1 = em.AddComponent<C<0>>(entities[2]);
  C<0>* e2_c2 = em.AddComponent<C<0>>(entities[2]);
  e2_c0->data = (static_cast<uint16>(1) << 0);
  e2_c1->data = (static_cast<uint16>(1) << 1);
  e2_c2->data = (static_cast<uint16>(1) << 2);
  entity_C0_components[entities[2]].push_back(*e2_c0);
  entity_C0_components[entities[2]].push_back(*e2_c1);
  entity_C0_components[entities[2]].push_back(*e2_c2);
  
  // Prepare the em - C<1> components (uint32 data)
  std::map<Entity, std::vector<C<1>> > entity_C1_components;
  
  C<1>* e2_c3 = em.AddComponent<C<1>>(entities[2]);
  C<1>* e2_c4 = em.AddComponent<C<1>>(entities[2]);
  C<1>* e2_c5 = em.AddComponent<C<1>>(entities[2]);
  e2_c3->data = (static_cast<uint32>(1) << 16);
  e2_c4->data = (static_cast<uint32>(1) << 17);
  e2_c5->data = (static_cast<uint32>(1) << 18);
  entity_C1_components[entities[2]].push_back(*e2_c3);
  entity_C1_components[entities[2]].push_back(*e2_c4);
  entity_C1_components[entities[2]].push_back(*e2_c5);
  
  C<1>* e3_c0 = em.AddComponent<C<1>>(entities[3]);
  C<1>* e3_c1 = em.AddComponent<C<1>>(entities[3]);
  e3_c0->data = (static_cast<uint32>(1) << 16);
  e3_c1->data = (static_cast<uint32>(1) << 17);
  entity_C1_components[entities[3]].push_back(*e3_c0);
  entity_C1_components[entities[3]].push_back(*e3_c1);
  
  C<1>* e4_c0 = em.AddComponent<C<1>>(entities[4]);
  e4_c0->data = (static_cast<uint32>(1) << 16);
  entity_C1_components[entities[4]].push_back(*e4_c0);
  
  // Prepare the em - C<2> components (uint64 data)
  std::map<Entity, std::vector<C<2>> > entity_C2_components;
  
  C<2>* e4_c1 = em.AddComponent<C<2>>(entities[4]);
  e4_c1->data = (static_cast<uint64>(1) << 32);
  entity_C2_components[entities[4]].push_back(*e4_c1);
  
  C<2>* e5_c0 = em.AddComponent<C<2>>(entities[5]);
  C<2>* e5_c1 = em.AddComponent<C<2>>(entities[5]);
  e5_c0->data = (static_cast<uint64>(1) << 32);
  e5_c1->data = (static_cast<uint64>(1) << 33);
  entity_C2_components[entities[5]].push_back(*e5_c0);
  entity_C2_components[entities[5]].push_back(*e5_c1);
  
  C<2>* e6_c0 = em.AddComponent<C<2>>(entities[6]);
  C<2>* e6_c1 = em.AddComponent<C<2>>(entities[6]);
  C<2>* e6_c2 = em.AddComponent<C<2>>(entities[6]);
  e6_c0->data = (static_cast<uint64>(1) << 32);
  e6_c1->data = (static_cast<uint64>(1) << 33);
  e6_c2->data = (static_cast<uint64>(1) << 34);
  entity_C2_components[entities[6]].push_back(*e6_c0);
  entity_C2_components[entities[6]].push_back(*e6_c1);
  entity_C2_components[entities[6]].push_back(*e6_c2);
  
  // Prepare the em - C<3> components (char data)
  std::map<Entity, std::vector<C<3>> > entity_C3_components;
  
  C<3>* e6_c3 = em.AddComponent<C<3>>(entities[6]);
  C<3>* e6_c4 = em.AddComponent<C<3>>(entities[6]);
  C<3>* e6_c5 = em.AddComponent<C<3>>(entities[6]);
  e6_c3->data = 'a';
  e6_c4->data = 'b';
  e6_c5->data = 'c';
  entity_C3_components[entities[6]].push_back(*e6_c3);
  entity_C3_components[entities[6]].push_back(*e6_c4);
  entity_C3_components[entities[6]].push_back(*e6_c5);
  
  C<3>* e7_c0 = em.AddComponent<C<3>>(entities[7]);
  C<3>* e7_c1 = em.AddComponent<C<3>>(entities[7]);
  e7_c0->data = 'a';
  e7_c1->data = 'b';
  entity_C3_components[entities[7]].push_back(*e7_c0);
  entity_C3_components[entities[7]].push_back(*e7_c1);
  
  C<3>* e8_c0 = em.AddComponent<C<3>>(entities[8]);
  e8_c0->data = 'a';
  entity_C3_components[entities[8]].push_back(*e8_c0);
  
  // Setup file and serialize
  FILE* out = fopen("test_temp.dat", "wb");
  CHECK(out != NULL);
  em.Serialize(out);
  fclose(out);
  
  // Open a file and deserialzie
  FILE* in = fopen("test_temp.dat", "rb");
  CHECK(in != NULL);
  MyEntityManager em_deserialzied(0);
  em_deserialzied.Deserialize(in);
  fclose(in);
  
  // Delete the file
  CHECK(remove("test_temp.dat") == 0);
  
  // Compare entities that are active
  std::vector<Entity> deserialized_entities;
  em.GetEntities(&deserialized_entities);
  CHECK(deserialized_entities.size() == entities.size());
  for(uint32 entity_index = 0;entity_index < entities.size();++entity_index) {
    CHECK(deserialized_entities[entity_index] == entities[entity_index]);
  }

  // Compare C<0> component state and components
  std::vector<Entity> deserialized_C0_entities;
  em_deserialzied.GetEntities<C<0>>(&deserialized_C0_entities);
  CHECK(deserialized_C0_entities.size() == 3);

  std::sort(deserialized_C0_entities.begin(), deserialized_C0_entities.end());
  CHECK(deserialized_C0_entities[0] == entities[0]);
  CHECK(deserialized_C0_entities[1] == entities[1]);
  CHECK(deserialized_C0_entities[2] == entities[2]);
  
  std::vector<C<0>*> E0_C0_components;
  em_deserialzied.GetComponent<C<0>>(deserialized_C0_entities[0], &E0_C0_components);
  CHECK(E0_C0_components.size() == 1);
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[0]].begin(), 
                  entity_C0_components[deserialized_C0_entities[0]].end(),
                  *E0_C0_components[0]) != entity_C0_components[deserialized_C0_entities[0]].end());

  std::vector<C<0>*> E1_C0_components;
  em_deserialzied.GetComponent<C<0>>(deserialized_C0_entities[1], &E1_C0_components);
  CHECK(E1_C0_components.size() == 2);
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[1]].begin(), 
                  entity_C0_components[deserialized_C0_entities[1]].end(),
                  *E1_C0_components[0]) != entity_C0_components[deserialized_C0_entities[1]].end());
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[1]].begin(), 
                  entity_C0_components[deserialized_C0_entities[1]].end(),
                  *E1_C0_components[1]) != entity_C0_components[deserialized_C0_entities[1]].end());
  
  std::vector<C<0>*> E2_C0_components;
  em_deserialzied.GetComponent<C<0>>(deserialized_C0_entities[2], &E2_C0_components);
  CHECK(E2_C0_components.size() == 3);
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[2]].begin(),
                  entity_C0_components[deserialized_C0_entities[2]].end(),
                  *E2_C0_components[0]) != entity_C0_components[deserialized_C0_entities[2]].end());
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[2]].begin(),
                  entity_C0_components[deserialized_C0_entities[2]].end(),
                  *E2_C0_components[1]) != entity_C0_components[deserialized_C0_entities[2]].end());
  CHECK(std::find(entity_C0_components[deserialized_C0_entities[2]].begin(),
                  entity_C0_components[deserialized_C0_entities[2]].end(),
                  *E2_C0_components[2]) != entity_C0_components[deserialized_C0_entities[2]].end());
  
  // Compare C<1> component state and components
  std::vector<Entity> deserialized_C1_entities;
  em_deserialzied.GetEntities<C<1>>(&deserialized_C1_entities);
  CHECK(deserialized_C0_entities.size() == 3);

  std::sort(deserialized_C1_entities.begin(), deserialized_C1_entities.end());
  CHECK(deserialized_C1_entities[0] == entities[2]);
  CHECK(deserialized_C1_entities[1] == entities[3]);
  CHECK(deserialized_C1_entities[2] == entities[4]);
  
  std::vector<C<1>*> E2_C1_components;
  em_deserialzied.GetComponent<C<1>>(deserialized_C1_entities[0], &E2_C1_components);
  CHECK(E2_C1_components.size() == 3);
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[0]].begin(), 
                  entity_C1_components[deserialized_C1_entities[0]].end(),
                  *E2_C1_components[0]) != entity_C1_components[deserialized_C1_entities[0]].end());
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[0]].begin(), 
                  entity_C1_components[deserialized_C1_entities[0]].end(),
                  *E2_C1_components[1]) != entity_C1_components[deserialized_C1_entities[0]].end());
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[0]].begin(), 
                  entity_C1_components[deserialized_C1_entities[0]].end(),
                  *E2_C1_components[2]) != entity_C1_components[deserialized_C1_entities[0]].end());

  std::vector<C<1>*> E3_C1_components;
  em_deserialzied.GetComponent<C<1>>(deserialized_C1_entities[1], &E3_C1_components);
  CHECK(E3_C1_components.size() == 2);
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[1]].begin(), 
                  entity_C1_components[deserialized_C1_entities[1]].end(),
                  *E3_C1_components[0]) != entity_C1_components[deserialized_C1_entities[1]].end());
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[1]].begin(), 
                  entity_C1_components[deserialized_C1_entities[1]].end(),
                  *E3_C1_components[1]) != entity_C1_components[deserialized_C1_entities[1]].end());
  
  std::vector<C<1>*> E4_C1_components;
  em_deserialzied.GetComponent<C<1>>(deserialized_C1_entities[2], &E4_C1_components);
  CHECK(E4_C1_components.size() == 1);
  CHECK(std::find(entity_C1_components[deserialized_C1_entities[2]].begin(), 
                  entity_C1_components[deserialized_C1_entities[2]].end(),
                  *E4_C1_components[0]) != entity_C1_components[deserialized_C1_entities[2]].end());
  
  // Compare C<2> component state and components
  std::vector<Entity> deserialized_C2_entities;
  em_deserialzied.GetEntities<C<2>>(&deserialized_C2_entities);
  CHECK(deserialized_C2_entities.size() == 3);

  std::sort(deserialized_C2_entities.begin(), deserialized_C2_entities.end());
  CHECK(deserialized_C2_entities[0] == entities[4]);
  CHECK(deserialized_C2_entities[1] == entities[5]);
  CHECK(deserialized_C2_entities[2] == entities[6]);
  
  std::vector<C<2>*> E4_C2_components;
  em_deserialzied.GetComponent<C<2>>(deserialized_C2_entities[0], &E4_C2_components);
  CHECK(E4_C2_components.size() == 1);
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[0]].begin(), 
                  entity_C2_components[deserialized_C2_entities[0]].end(),
                  *E4_C2_components[0]) != entity_C2_components[deserialized_C2_entities[0]].end());

  std::vector<C<2>*> E5_C2_components;
  em_deserialzied.GetComponent<C<2>>(deserialized_C2_entities[1], &E5_C2_components);
  CHECK(E5_C2_components.size() == 2);
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[1]].begin(), 
                  entity_C2_components[deserialized_C2_entities[1]].end(),
                  *E5_C2_components[0]) != entity_C2_components[deserialized_C2_entities[1]].end());
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[1]].begin(), 
                  entity_C2_components[deserialized_C2_entities[1]].end(),
                  *E5_C2_components[1]) != entity_C2_components[deserialized_C2_entities[1]].end());
  
  std::vector<C<2>*> E6_C2_components;
  em_deserialzied.GetComponent<C<2>>(deserialized_C2_entities[2], &E6_C2_components);
  CHECK(E6_C2_components.size() == 3);
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[2]].begin(),
                  entity_C2_components[deserialized_C2_entities[2]].end(),
                  *E6_C2_components[0]) != entity_C2_components[deserialized_C2_entities[2]].end());
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[2]].begin(),
                  entity_C2_components[deserialized_C2_entities[2]].end(),
                  *E6_C2_components[1]) != entity_C2_components[deserialized_C2_entities[2]].end());
  CHECK(std::find(entity_C2_components[deserialized_C2_entities[2]].begin(),
                  entity_C2_components[deserialized_C2_entities[2]].end(),
                  *E6_C2_components[2]) != entity_C2_components[deserialized_C2_entities[2]].end());
  
  // Compare C<3> component state and components
  std::vector<Entity> deserialized_C3_entities;
  em_deserialzied.GetEntities<C<3>>(&deserialized_C3_entities);
  CHECK(deserialized_C3_entities.size() == 3);

  std::sort(deserialized_C3_entities.begin(), deserialized_C3_entities.end());
  CHECK(deserialized_C3_entities[0] == entities[6]);
  CHECK(deserialized_C3_entities[1] == entities[7]);
  CHECK(deserialized_C3_entities[2] == entities[8]);
  
  std::vector<C<3>*> E4_C3_components;
  em_deserialzied.GetComponent<C<3>>(deserialized_C3_entities[0], &E4_C3_components);
  CHECK(E4_C3_components.size() == 3);
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[0]].begin(), 
                  entity_C3_components[deserialized_C3_entities[0]].end(),
                  *E4_C3_components[0]) != entity_C3_components[deserialized_C3_entities[0]].end());
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[0]].begin(), 
                  entity_C3_components[deserialized_C3_entities[0]].end(),
                  *E4_C3_components[1]) != entity_C3_components[deserialized_C3_entities[0]].end());
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[0]].begin(), 
                  entity_C3_components[deserialized_C3_entities[0]].end(),
                  *E4_C3_components[2]) != entity_C3_components[deserialized_C3_entities[0]].end());
  
  std::vector<C<3>*> E5_C3_components;
  em_deserialzied.GetComponent<C<3>>(deserialized_C3_entities[1], &E5_C3_components);
  CHECK(E5_C3_components.size() == 2);
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[1]].begin(), 
                  entity_C3_components[deserialized_C3_entities[1]].end(),
                  *E5_C3_components[0]) != entity_C3_components[deserialized_C3_entities[1]].end());
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[1]].begin(), 
                  entity_C3_components[deserialized_C3_entities[1]].end(),
                  *E5_C3_components[1]) != entity_C3_components[deserialized_C3_entities[1]].end());
  
  std::vector<C<3>*> E6_C3_components;
  em_deserialzied.GetComponent<C<3>>(deserialized_C3_entities[2], &E6_C3_components);
  CHECK(E6_C3_components.size() == 1);
  CHECK(std::find(entity_C3_components[deserialized_C3_entities[2]].begin(), 
                  entity_C3_components[deserialized_C3_entities[2]].end(),
                  *E6_C3_components[0]) != entity_C3_components[deserialized_C3_entities[2]].end());
}
*/
