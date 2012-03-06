/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Helpers/Constants.h"

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/StringHash/Hash.h"

#include "Tetris/Components/HelperTypes.h"

const TetrominoRotationType Constants::TETROMINO_ROTATION_CONVERSION[TETROMINO_TYPE_COUNT][4] = {
  // L piece = 0
  { L_ROT0, L_ROT1, L_ROT2, L_ROT3 },
  // Inv-L piece = 1
  { INV_L_ROT0, INV_L_ROT1, INV_L_ROT2, INV_L_ROT3 },
  // S piece = 2
  { S_ROT0, S_ROT1, S_ROT0, S_ROT1 },
  // Inv-S piece = 3
  { INV_S_ROT0, INV_S_ROT1, INV_S_ROT0, INV_S_ROT1 },
  // I piece = 4
  { I_ROT0, I_ROT1, I_ROT0, I_ROT1 },
  // T piece = 5
  { T_ROT0, T_ROT1, T_ROT2, T_ROT3 },
  // O piece = 6
  { O_ROT0, O_ROT0, O_ROT0, O_ROT0 },
};

const BoardOffset Constants::TETROMINO_ROTATION_TO_OFFSETS[TETROMINO_ROTATION_TYPE_COUNT][4] = {
  // L_ROT0 = 0
  { BoardOffset(1, -1), BoardOffset(0, -1), BoardOffset(0, 0), BoardOffset(0, 1) },
  // L_ROT1 = 1
  { BoardOffset(1, 1), BoardOffset(1, 0), BoardOffset(0, 0), BoardOffset(-1, 0) },
  // L_ROT2 = 2
  { BoardOffset(-1, 1), BoardOffset(0, 1), BoardOffset(0, 0), BoardOffset(0, -1) },
  // L_ROT3 = 3
  { BoardOffset(-1, -1), BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(1, 0) },
  // INV_L_ROT0 = 4
  { BoardOffset(1, 1), BoardOffset(0, 1), BoardOffset(0, 0), BoardOffset(0, -1) },
  // INV_L_ROT1 = 5
  { BoardOffset(-1, 1), BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(1, 0) },
  // INV_L_ROT2 = 6
  { BoardOffset(-1, -1), BoardOffset(0, -1), BoardOffset(0, 0), BoardOffset(0, 1) },
  // INV_L_ROT3 = 7
  { BoardOffset(1, -1), BoardOffset(1, 0), BoardOffset(0, 0), BoardOffset(-1, 0) },
  // I_ROT0 = 8
  { BoardOffset(0, -2), BoardOffset(0, -1), BoardOffset(0, 0), BoardOffset(0, 1) },
  // I_ROT1 = 9
  { BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(1, 0), BoardOffset(2, 0) },
  // S_ROT0 = 10
  { BoardOffset(0, 1), BoardOffset(0, 0), BoardOffset(1, 0), BoardOffset(1, -1) },
  // S_ROT1 = 11
  { BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(0, 1), BoardOffset(1, 1) },
  // INV_S_ROT0 = 12
  { BoardOffset(0, -1), BoardOffset(0, 0), BoardOffset(1, 0), BoardOffset(1, 1) },
  // INV_S_ROT1 = 13
  { BoardOffset(1, 0), BoardOffset(0, 0), BoardOffset(0, 1), BoardOffset(-1, 1) },
  // T_ROT0 = 14
  { BoardOffset(0, -1), BoardOffset(0, 0), BoardOffset(0, 1), BoardOffset(1, 0) },
  // T_ROT1 = 15
  { BoardOffset(1, 0), BoardOffset(0, 0), BoardOffset(0, 1), BoardOffset(-1, 0) },
  // T_ROT2 = 16
  { BoardOffset(0, 1), BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(0, -1) },
  // T_ROT3 = 17
  { BoardOffset(-1, 0), BoardOffset(0, 0), BoardOffset(1, 0), BoardOffset(0, -1) },
  // O_ROT0 = 18
  { BoardOffset(1, -1), BoardOffset(1, 0), BoardOffset(0, 0), BoardOffset(0, -1) }
};

const TetrominoType Constants::TETROMINO_INDEX_TO_TETROMINO_TYPE[TETROMINO_TYPE_COUNT] = {
  L,  // L = 0
  INV_L,  // INV_L = 1
  S,  // S = 2
  INV_S,  // INV_S = 3
  I,  // I = 4
  T,  // T = 5
  O   // O = 6
};

const uint32 Constants::TETROMINO_INDEX_TO_ASSET_ID[TETROMINO_TYPE_COUNT] = {
  ELG_STRING_HASH("ORANGE_TILE"),  // 0
  ELG_STRING_HASH("BLUE_TILE"),  // 1
  ELG_STRING_HASH("CYAN_TILE"),  // 2
  ELG_STRING_HASH("GREEN_TILE"),  // 3
  ELG_STRING_HASH("RED_TILE"),  // 4
  ELG_STRING_HASH("VIOLET_TILE"),  // 5
  ELG_STRING_HASH("YELLOW_TILE")  // 6
};
