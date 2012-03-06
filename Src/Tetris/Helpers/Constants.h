/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_HELPERS_CONSTANTS_H_
#define TETRIS_HELPERS_CONSTANTS_H_

#include "Elg/Utilities/Types.h"
#include "Tetris/Components/HelperTypes.h"

class Constants {
 public:
  // General tetris stuff
  static const TetrominoRotationType Constants::TETROMINO_ROTATION_CONVERSION[TETROMINO_TYPE_COUNT][4];
  static const BoardOffset Constants::TETROMINO_ROTATION_TO_OFFSETS[TETROMINO_ROTATION_TYPE_COUNT][4];
  static const TetrominoType TETROMINO_INDEX_TO_TETROMINO_TYPE[TETROMINO_TYPE_COUNT];
  static const uint32 TETROMINO_INDEX_TO_ASSET_ID[TETROMINO_TYPE_COUNT];
  static const int32 BOARD_WIDTH = 10;
  static const int32 BOARD_HEIGHT = 20;
  static const int16 TETROMINO_STARTING_ROW = 0;
  static const int16 TETROMINO_STARTING_COLUMN = 5;
  static const uint16 TETROMINO_STARTING_ROTATION = 0;
  static const uint16 TETROMINO_PREVIEW_ROTATION = 0;
  static const uint16 TILE_DEFAULT_ROTATION = 0;

  // User input consts
  static const int32 KEYBOARD_REPEAT_DELAY = 128;
  static const int32 KEYBOARD_REPEAT_INTERVAL = 64;
  static const uint32 MAX_INPUT_EVENTS = 16;

  // Graphics system consts
  static const int32 TETROMINO_RENDERING_OFFSET_ROW = -1;
  static const int32 TETROMINO_RENDERING_OFFSET_COLUMN = -2;
  static const int32 TILE_WIDTH = 24;
  static const int32 TILE_HEIGHT = 24;
  static const int32 BOARD_POSITION_X = 277;
  static const int32 BOARD_POSITION_Y = 57;
  static const int32 BOARD_BORDER = 3;
  static const int32 PREVIEW_POSITION_X = 563;
  static const int32 PREVIEW_POSITION_Y = 157;
  static const int32 PREVIEW_BORDER = 3;
  static const int32 TETROMINO_PREVIEW_OFFSET_ROW = -1;
  static const int32 TETROMINO_PREVIEW_OFFSET_COLUMN = 0;
};

#endif  // TETRIS_HELPERS_CONSTANTS_H_
