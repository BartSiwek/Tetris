/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_HELPERS_TETRISHELPER_H_
#define TETRIS_HELPERS_TETRISHELPER_H_

#include <vector>

#include "Elg/Utilities/Types.h"
#include "Tetris/Components/HelperTypes.h"

class TetrisHelper {
 public:
  static void GetBoardLocations(const BoardLocation &base_location, uint16 rotation, TetrominoType type,
                                BoardLocation locations[4]);
  static bool AreWithinBoard(const BoardLocation locations[4]);
  static bool AreNotColiding(const BoardLocation locations[4], const std::vector<BoardLocation*> &other_locations);
};

#endif  // TETRIS_HELPERS_TETRISHELPER_H_
