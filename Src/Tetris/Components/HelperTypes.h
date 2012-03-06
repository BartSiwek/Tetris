/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_COMPONENTS_HELPERTYPES_H_
#define TETRIS_COMPONENTS_HELPERTYPES_H_

enum TetrisEvent {
  MOVE_LEFT,
  MOVE_RIGHT,
  ROTATE,
  DROP
};

enum TetrominoType {
  L = 0,
  INV_L = 1,
  S = 2,
  INV_S = 3,
  I = 4,
  T = 5,
  O = 6,
  TETROMINO_TYPE_COUNT = 7
};

enum TetrominoRotationType {
  // L Tetromino
  L_ROT0 = 0,
  L_ROT1 = 1,
  L_ROT2 = 2,
  L_ROT3 = 3,
  // Inverted L Tetromino
  INV_L_ROT0 = 4,
  INV_L_ROT1 = 5,
  INV_L_ROT2 = 6,
  INV_L_ROT3 = 7,
  // I Tetromino
  I_ROT0 = 8,
  I_ROT1 = 9,
  // S Tetromino
  S_ROT0 = 10,
  S_ROT1 = 11,
  // Inverted S Tetromino
  INV_S_ROT0 = 12,
  INV_S_ROT1 = 13,
  // T Tetromino
  T_ROT0 = 14,
  T_ROT1 = 15,
  T_ROT2 = 16,
  T_ROT3 = 17,
  // O Tetromino
  O_ROT0 = 18,
  // Max
  TETROMINO_ROTATION_TYPE_COUNT = 19,
  // Rest
  TILE = 100,
  PREVIEW = 101,
  BOARD = 102
};

struct BoardOffset {
  BoardOffset() : row_offset(0), column_offset(0) {
  }

  BoardOffset(int16 ro, int16 co) : row_offset(ro), column_offset(co) {
  }

  inline BoardOffset& operator=(const BoardOffset &other) {
      row_offset = other.row_offset;
      column_offset = other.column_offset;
      return *this;
  }

  friend inline BoardOffset operator+(const BoardOffset &lhs, const BoardOffset &rhs) {
    return BoardOffset(lhs.row_offset + rhs.row_offset, lhs.column_offset + rhs.column_offset);
  }

  inline BoardOffset& operator+=(const BoardOffset &other) {
      row_offset += other.row_offset;
      column_offset += other.column_offset;
      return *this;
  }

  friend inline BoardOffset operator-(const BoardOffset &lhs, const BoardOffset &rhs) {
    return BoardOffset(lhs.row_offset - rhs.row_offset, lhs.column_offset - rhs.column_offset);
  }

  inline BoardOffset& operator-=(const BoardOffset &other) {
      row_offset -= other.row_offset;
      column_offset -= other.column_offset;
      return *this;
  }

  friend inline BoardOffset operator-(const BoardOffset &other) {
    return BoardOffset(-other.row_offset, -other.column_offset);
  }

  int16 row_offset;
  int16 column_offset;
};

struct BoardLocation {
  BoardLocation() : row(0), column(0) {
  }

  BoardLocation(int16 r, int16 c) : row(r), column(c) {
  }

  inline BoardLocation& operator=(const BoardLocation &other) {
      row = other.row;
      column = other.column;
      return *this;
  }

  bool operator==(const BoardLocation &other) const {
    return (row == other.row) && (column == other.column);
  }

  friend inline BoardLocation operator+(const BoardLocation &lhs, const BoardOffset &rhs) {
    return BoardLocation(lhs.row + rhs.row_offset, lhs.column + rhs.column_offset);
  }

  inline BoardLocation& operator+=(const BoardOffset &other) {
    row += other.row_offset;
    column += other.column_offset;
    return *this;
  }

  friend inline BoardLocation operator-(const BoardLocation &lhs, const BoardOffset &rhs) {
    return BoardLocation(lhs.row - rhs.row_offset, lhs.column - rhs.column_offset);
  }

  inline BoardLocation& operator-=(const BoardOffset &other) {
    row -= other.row_offset;
    column -= other.column_offset;
    return *this;
  }

  int16 row;
  int16 column;
};

#endif  // TETRIS_COMPONENTS_HELPERTYPES_H_
