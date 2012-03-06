/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_SYSTEMS_GAMELOGICSYSTEM_GAMELOGICSYSTEM_H_
#define TETRIS_SYSTEMS_GAMELOGICSYSTEM_GAMELOGICSYSTEM_H_

#include <vector>

#include "Elg/Utilities/Types.h"
#include "Elg/Clock/GameClock.h"

#include "Tetris/Assets/AllAssets.h"
#include "Tetris/Components/AllComponents.h"
#include "Tetris/Helpers/Constants.h"

class GameLogicSystem {
 public:
  // Constructors
  explicit GameLogicSystem(TetrisEntityManager *entity_manager);

  // Destructor
  ~GameLogicSystem();

  // Methods, including static methods
  void Initialize();
  bool Update(Elg::Clock::GameClock::TimePoint time, Elg::Clock::GameClock::Duration timeStep);

 private:
  // Methods, including static methods
  bool ProcessEvents();
  void ProcessMovementEvent(int16 dc, int16 dr, uint32 movable_entity);
  bool ProcessDropEvent(uint32 movable_entity);
  bool PerformTetrominoFall();
  void UpdateGraphicsData();
  bool PlaceTetrominoOnBoard(uint32 tetromino);
  void RemoveLines();
  bool CanPlaceNextTetrominoOnBoard();
  void PlaceNextTetrominoOnBoard();
  void GenerateNextTetromino();
  void UpdateLevel();

  void GetTileLocations(std::vector<BoardLocation*> *tile_locations);
  int32 RandomInteger(int32 min, int32 max);

  static Elg::Clock::Milliseconds GetFallPeriod(uint32 level);

  // Data memebrs
  TetrisEntityManager *m_entity_manager_;
  uint32 m_next_tetromino_;
  Elg::Clock::Milliseconds m_accumulated_time_;
  Elg::Clock::Milliseconds m_fall_period_;
  uint32 m_level_;
  uint32 m_lines_;
};

#endif  // TETRIS_SYSTEMS_GAMELOGICSYSTEM_GAMELOGICSYSTEM_H_
