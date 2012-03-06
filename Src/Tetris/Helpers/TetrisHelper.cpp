/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Helpers/TetrisHelper.h"

#include <vector>

#include "Elg/Utilities/Types.h"
#include "Tetris/Components/HelperTypes.h"
#include "Tetris/Helpers/Constants.h"

void TetrisHelper::GetBoardLocations(const BoardLocation &base_location, uint16 rotation, TetrominoType type,
                                     BoardLocation locations[4]) {
  // Get the offsets
  TetrominoRotationType rt = Constants::TETROMINO_ROTATION_CONVERSION[type][rotation];
  const BoardOffset* offsets = Constants::TETROMINO_ROTATION_TO_OFFSETS[rt];

  // Transform the position via offsets to get the individual positions
  for (uint32 offset_index = 0;offset_index < 4;++offset_index) {
    locations[offset_index] = base_location + offsets[offset_index];
  }
}

bool TetrisHelper::AreWithinBoard(const BoardLocation locations[4]) {
  for (uint32 location_index = 0;location_index < 4;++location_index) {
    if (locations[location_index].column < 0 || locations[location_index].column >= Constants::BOARD_WIDTH) {
      return false;
    }
    if (locations[location_index].row < 0 || locations[location_index].row >= Constants::BOARD_HEIGHT) {
      return false;
    }
  }
  return true;
}

bool TetrisHelper::AreNotColiding(const BoardLocation locations[4],
                                  const std::vector<BoardLocation*> &other_locations) {
  std::vector<BoardLocation*>::const_iterator location_it = other_locations.begin();
  std::vector<BoardLocation*>::const_iterator location_end = other_locations.end();
  while (location_it != location_end) {
    for (uint32 location_index = 0;location_index < 4;++location_index) {
      if (locations[location_index] == **location_it) {
        return false;
      }
    }
    ++location_it;
  }
  return true;
}
