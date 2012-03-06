/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_TETRISGAME_H_
#define TETRIS_TETRISGAME_H_

#include "Elg/Utilities/Types.h"
#include "Elg/Clock/GameClock.h"

#include "Tetris/Components/AllComponents.h"
#include "Tetris/Assets/AllAssets.h"

#include "Systems/UserInputSystem/UserInputSystem.h"
#include "Systems/GameLogicSystem/GameLogicSystem.h"
#include "Systems/GraphicsSystem/GraphicsSystem.h"

class TetrisGame {
 public:
  // Constructors
  TetrisGame(int32 argc, char** argv);

  // Destructor
  ~TetrisGame();

  // Methods, including static methods
  bool Update(Elg::Clock::GameClock::TimePoint current_simulation_time, Elg::Clock::GameClock::Duration DT);
  void Render(float32 interpolation_factor);

 private:
  // Consts
  static const uint32 CAPTION_LENGTH;

  // Data Members
  TetrisEntityManager *m_entity_manager_;
  TetrisAssetManager *m_asset_manager_;

  UserInputSystem *m_user_input_sys_;
  GameLogicSystem *m_game_logic_sys_;
  GraphicsSystem *m_graphics_sys_;

  char*  m_caption_buffer_;
};

#endif  // TETRIS_TETRISGAME_H_
