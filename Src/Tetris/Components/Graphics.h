/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_COMPONENTS_GRAPHICS_H_
#define TETRIS_COMPONENTS_GRAPHICS_H_

#include <string>

#include "Elg/Utilities/Types.h"

#include "Tetris/Components/HelperTypes.h"

struct Graphics {
  int32 ScreenX;
  int32 ScreenY;
  uint32 RepresentationAssetIdHash;
  TetrominoRotationType Representation;
};

#endif  // TETRIS_COMPONENTS_GRAPHICS_H_
