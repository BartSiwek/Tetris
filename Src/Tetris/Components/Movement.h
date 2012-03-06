/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_COMPONENTS_MOVEMENT_H_
#define TETRIS_COMPONENTS_MOVEMENT_H_

#include <vector>

#include "Elg/Utilities/Types.h"

#include "Tetris/Helpers/Constants.h"

#include "Tetris/Components/HelperTypes.h"


struct Movement {
  TetrisEvent Events[Constants::MAX_INPUT_EVENTS];
  uint32 EventCount;
};

#endif  // TETRIS_COMPONENTS_MOVEMENT_H_
