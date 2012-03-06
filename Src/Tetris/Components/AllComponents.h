/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_COMPONENTS_ALLCOMPONENTS_H_
#define TETRIS_COMPONENTS_ALLCOMPONENTS_H_

#include "Tetris/Components/BoardPosition.h"
#include "Tetris/Components/Graphics.h"
#include "Tetris/Components/Movement.h"
#include "Tetris/Components/Tetromino.h"
#include "Tetris/Components/Tile.h"

#include "Elg/EntityManager/DescriptionMacros.h"

struct TetrisEntityManagerDescription {
  ELG_ENTITY_MANAGER_DESCRIPTION_HEADER(1024);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(0, 1024, BoardPosition);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(1, 1024, Graphics);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(2, 1024, Movement);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(3, 1024, Tetromino);

  ELG_ENTITY_MANAGER_DESCRIPTION_COMPONENT(4, 1024, Tile);

  ELG_ENTITY_MANAGER_DESCRIPTION_FOOTER(5);
};

namespace Elg { namespace EntityManager { template<typename EntityManagerDescriptionType> class EntityManager; } }
namespace Elg { namespace EntityManager { template<typename EntityManagerType> class EntitySet; } }

typedef Elg::EntityManager::EntityManager<TetrisEntityManagerDescription> TetrisEntityManager;
typedef Elg::EntityManager::EntitySet<TetrisEntityManager> TetrisEntitySet;

#endif  // TETRIS_COMPONENTS_ALLCOMPONENTS_H_
