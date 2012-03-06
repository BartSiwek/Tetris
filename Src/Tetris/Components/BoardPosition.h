/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_COMPONENTS_BOARDPOSITION_H_
#define TETRIS_COMPONENTS_BOARDPOSITION_H_

#include "Elg/Utilities/Types.h"

#include "Tetris/Components/HelperTypes.h"

struct BoardPosition {
  BoardLocation Location;
  uint16 Rotation;
};

#endif  // TETRIS_COMPONENTS_BOARDPOSITION_H_
