/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>
#include <cstdlib>
#include <ctime>

#include "Elg/EntityManager/EntityPool/EntityPool.h"

typedef Elg::EntityManager::EntityPool<128> TestEntityPoolType;

TEST(EntityPool_DefaultState) {
  // Setup & execute the test
  TestEntityPoolType pool;

  // Check that count is equal to zero
  CHECK_EQUAL(static_cast<uint32>(0), pool.GetActiveEntityCount());

  // Check that IDs are not active (we use the knowledge of internals here)
  for (uint32 id = 0;id < 128;++id) {
    CHECK_EQUAL(ELG_ENTITY_MANAGER_INVALID_ID, pool.GetEntityIndex(id));
  }
}

TEST(EntityPool_UniqueIds) {
  // Setup
  TestEntityPoolType pool;
  uint32 ids[128];
  uint32 current_count = 0;

  // Execute & check
  for (uint32 current_id_index = 0;current_id_index < 128;++current_id_index) {
    // Create a new ID
    uint32 new_id = pool.CreateEntity();

    // Check if it differs from prev. ones
    for (uint32 prev_id_index = 0;prev_id_index < current_count;++prev_id_index) {
      CHECK(ids[prev_id_index] != new_id);
    }

    // Store it
    ids[current_count++] = new_id;
  }
}

TEST(EntityPool_SimpleCreate) {
  // Setup the test
  TestEntityPoolType pool;

  // Execute
  uint32 entity_id = pool.CreateEntity();

  // Check that count is equal to zero and ids match
  CHECK_EQUAL(static_cast<uint32>(1), pool.GetActiveEntityCount());
  CHECK(pool.GetEntityIndex(entity_id) != ELG_ENTITY_MANAGER_INVALID_ID);
  CHECK_EQUAL(entity_id, pool.GetActiveEntities()[0]);
}

TEST(EntityPool_SimpleCreateDelete) {
  // Setup the test
  TestEntityPoolType pool;

  // Execute
  uint32 entity_id = pool.CreateEntity();
  uint32 entity_id_index = pool.GetEntityIndex(entity_id);
  pool.DestroyEntity(entity_id_index);

  // Check that count is equal to zero and entity_id is not active
  CHECK_EQUAL(static_cast<uint32>(0), pool.GetActiveEntityCount());
  CHECK_EQUAL(ELG_ENTITY_MANAGER_INVALID_ID, pool.GetEntityIndex(entity_id));
}

TEST(EntityPool_FillCreate) {
  // Setup the test
  TestEntityPoolType pool;

  // Execute
  uint32 created_ids[TestEntityPoolType::MaxEntityCount];
  for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
    created_ids[current_id_index] = pool.CreateEntity();
  }

  // Check that count and active entities make sense
  CHECK_EQUAL(TestEntityPoolType::MaxEntityCount, pool.GetActiveEntityCount());
  const uint32* active_entities = pool.GetActiveEntities();
  for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
    CHECK(pool.GetEntityIndex(created_ids[current_id_index]) != ELG_ENTITY_MANAGER_INVALID_ID);
    CHECK_EQUAL(created_ids[current_id_index], active_entities[current_id_index]);
  }
}

TEST(EntityPool_FillCreateDelete) {
  // Setup the test
  TestEntityPoolType pool;

  // Execute
  uint32 created_ids[TestEntityPoolType::MaxEntityCount];
  for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
    created_ids[current_id_index] = pool.CreateEntity();
  }

  CHECK_EQUAL(TestEntityPoolType::MaxEntityCount, pool.GetActiveEntityCount());

  for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
    uint32 current_entity_id_index = pool.GetEntityIndex(created_ids[current_id_index]);
    pool.DestroyEntity(current_entity_id_index);
  }

  // Check that count is equal to zero and entity_ids are not active
  CHECK_EQUAL(static_cast<uint32>(0), pool.GetActiveEntityCount());
  for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
    CHECK_EQUAL(ELG_ENTITY_MANAGER_INVALID_ID, pool.GetEntityIndex(created_ids[current_id_index]));
  }
}

TEST(EntityPool_FillCreateDeleteRandom) {
  // Setup the test
  TestEntityPoolType pool;
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  for (uint32 interation_index = 0;interation_index < TestEntityPoolType::MaxEntityCount;++interation_index) {
    // Execute - create ids
    uint32 created_ids[TestEntityPoolType::MaxEntityCount];
    for (uint32 current_id_index = 0;current_id_index < TestEntityPoolType::MaxEntityCount;++current_id_index) {
      created_ids[current_id_index] = pool.CreateEntity();
    }

    CHECK_EQUAL(TestEntityPoolType::MaxEntityCount, pool.GetActiveEntityCount());

    // Delete ids in random order
    for (uint32 current_count = TestEntityPoolType::MaxEntityCount;current_count > 0;--current_count) {
      // The rand is thread safe on VS according to MSDN
      uint32 value = (rand() % current_count);  // NOLINT(runtime/threadsafe_fn)

      uint32 to_be_deleted = created_ids[value];
      created_ids[value] = created_ids[current_count-1];
      created_ids[current_count-1] = to_be_deleted;

      uint32 to_be_deleted_index = pool.GetEntityIndex(to_be_deleted);
      pool.DestroyEntity(to_be_deleted_index);
    }

    // Check that count is equal to zero and entity_ids are not active
    CHECK_EQUAL(static_cast<uint32>(0), pool.GetActiveEntityCount());
    for (uint32 current_id_index = 0;current_id_index < 10;++current_id_index) {
      CHECK_EQUAL(ELG_ENTITY_MANAGER_INVALID_ID, pool.GetEntityIndex(created_ids[current_id_index]));
    }
  }
}

TEST(EntityPool_RemoveingTopElement) {
  // Setup the test
  TestEntityPoolType pool;
  STATIC_ASSERT(TestEntityPoolType::MaxEntityCount >= 3, "The test requires the EntityPool with at least 3 slots");

  // Execute
  uint32 ids[3];
  for (uint32 current_id_index = 0;current_id_index < 3;++current_id_index) {
    ids[current_id_index] = pool.CreateEntity();
  }

  CHECK_EQUAL(static_cast<uint32>(3), pool.GetActiveEntityCount());
  CHECK_EQUAL(ids[0], pool.GetActiveEntities()[0]);
  CHECK(pool.GetEntityIndex(ids[0]) != ELG_ENTITY_MANAGER_INVALID_ID);
  CHECK_EQUAL(ids[1], pool.GetActiveEntities()[1]);
  CHECK(pool.GetEntityIndex(ids[1]) != ELG_ENTITY_MANAGER_INVALID_ID);
  CHECK_EQUAL(ids[2], pool.GetActiveEntities()[2]);
  CHECK(pool.GetEntityIndex(ids[2]) != ELG_ENTITY_MANAGER_INVALID_ID);

  uint32 id_1_index = pool.GetEntityIndex(ids[1]);
  pool.DestroyEntity(id_1_index);
  uint32 id_2_index = pool.GetEntityIndex(ids[2]);
  pool.DestroyEntity(id_2_index);

  ids[1] = pool.CreateEntity();

  // Check
  CHECK_EQUAL(static_cast<uint32>(2), pool.GetActiveEntityCount());
  CHECK_EQUAL(ids[0], pool.GetActiveEntities()[0]);
  CHECK(pool.GetEntityIndex(ids[0]) != ELG_ENTITY_MANAGER_INVALID_ID);
  CHECK_EQUAL(ids[1], pool.GetActiveEntities()[1]);
  CHECK(pool.GetEntityIndex(ids[1]) != ELG_ENTITY_MANAGER_INVALID_ID);
}

TEST(EntityPool_RandomOperations) {
    // Setup the test
    TestEntityPoolType pool;
    uint32 current_count = 0;
    uint32 ids[TestEntityPoolType::MaxEntityCount];
    for (uint32 current_index = 0;current_index < TestEntityPoolType::MaxEntityCount;++current_index) {
      ids[current_index] = 0xFFFFFFFF;
    }

    // Run the test and check as we go
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    for (uint32 operation_index = 0;operation_index < 100000;++operation_index) {
      // The rand is thread safe on VS according to MSDN
      uint32 value = (rand() % TestEntityPoolType::MaxEntityCount);  // NOLINT(runtime/threadsafe_fn)
      if (value < current_count) {
        // Delete thing at value
        uint32 to_be_deleted = ids[value];
        ids[value] = ids[--current_count];
        ids[current_count] = 0xFFFFFFFF;
        uint32 to_be_deleted_index = pool.GetEntityIndex(to_be_deleted);
        pool.DestroyEntity(to_be_deleted_index);
      } else {
        // Create one
        uint32 created = pool.CreateEntity();
        ids[current_count++] = created;
      }

      // Do checking
      CHECK_EQUAL(pool.GetActiveEntityCount(), current_count);
      const uint32* current_active = pool.GetActiveEntities();
      for (uint32 current_active_index = 0;current_active_index < current_count;++current_active_index) {
        bool found = false;
        for (uint32 current_id_index = 0;current_id_index < current_count;++current_id_index) {
          if (ids[current_id_index] == current_active[current_active_index]) {
            found = true;
            break;
          }
        }
        CHECK_EQUAL(true, found);
      }
    }
}

TEST(EntityPool_Overflow) {
  // This is just asserted - not testing

  /*
  // Setup
  TestEntityPoolType pool;
  
  // Execute
  for(uint32 operaion_index = 0;operaion_index < TestEntityPoolType::MaxEntityCount;++operaion_index) {
    pool.CreateEntity();
  }

  CHECK_THROW(pool.CreateEntity(), Elg::EntityManager::EntityManagerException);
  */
}
