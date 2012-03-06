/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/TetrisGame.h"

#include <SDL/SDL.h>

#include "Elg/Utilities/Types.h"
#include "Elg/Performance/FrameStatistics.h"
#include "Elg/EntityManager/EntityManager.h"
#include "Elg/AssetManager/AssetManager.h"

#include "Tetris/Systems/UserInputSystem/UserInputSystem.h"
#include "Tetris/Systems/GameLogicSystem/GameLogicSystem.h"
#include "Tetris/Systems/GraphicsSystem/GraphicsSystem.h"

const uint32 TetrisGame::CAPTION_LENGTH = 70;

TetrisGame::TetrisGame(int32 /* argc */, char** /* argv */)  // NOLINT(readability/casting) - Not a C-Style cast
    : m_entity_manager_(ElgNew TetrisEntityManager()),
      m_asset_manager_(ElgNew TetrisAssetManager(GetAssetDescriptors(), GetAssetDescriptorCount())),
      m_user_input_sys_(ElgNew UserInputSystem(m_entity_manager_)),
      m_game_logic_sys_(ElgNew GameLogicSystem(m_entity_manager_)),
      m_graphics_sys_(ElgNew GraphicsSystem(m_entity_manager_, m_asset_manager_)),
      m_caption_buffer_(ElgNew char[CAPTION_LENGTH]) {
  // Assert allocations
  ASSERT(m_entity_manager_ != NULL, "Allocation of memory for entity manager failed");
  ASSERT(m_asset_manager_ != NULL, "Allocation of memory for asset manager failed");
  ASSERT(m_user_input_sys_ != NULL, "Allocation of memory for user input system failed");
  ASSERT(m_game_logic_sys_ != NULL, "Allocation of memory for game logic system failed");
  ASSERT(m_graphics_sys_ != NULL, "Allocation of memory for graphics system failed");

  // Initialize SDL video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("Unable to initialize SDL\n");
  }

  SDL_WM_SetCaption("Tetris [Avg FPS:   0 Avg Ms:  0.00]", NULL);

  m_user_input_sys_->Initialize();
  m_game_logic_sys_->Initialize();
  m_graphics_sys_->Initialize();
}

TetrisGame::~TetrisGame() {
  // Release all the things
  ElgDelete m_graphics_sys_;
  ElgDelete m_game_logic_sys_;
  ElgDelete m_user_input_sys_;
  ElgDelete m_asset_manager_;
  ElgDelete m_entity_manager_;
  ElgDelete[] m_caption_buffer_;

  SDL_Quit();
}

bool TetrisGame::Update(Elg::Clock::GameClock::TimePoint current_simulation_time, Elg::Clock::GameClock::Duration DT) {
  uint32 current_fps = Elg::Performance::FrameStatistics::GetCurrentFps();
  uint32 avg_fps = Elg::Performance::FrameStatistics::GetAvgFps();
  uint32 current_frame_time = Elg::Performance::FrameStatistics::GetCurrentFrameTime();
  uint32 avg_frame_time = Elg::Performance::FrameStatistics::GetAvgFrameTime();

  sprintf_s(m_caption_buffer_, CAPTION_LENGTH,
            "Tetris [FPS: %3ld.%2ld Avg FPS: %3ld.%2ld Ms: %3ld.%2ld Avg Ms: %3ld.%2ld]",
            (current_fps / 1000), (current_fps % 1000) / 10,
            (avg_fps / 1000), (avg_fps % 1000) / 10,
            (current_frame_time / 1000), (current_frame_time % 1000) / 10,
            (avg_frame_time / 1000), (avg_frame_time % 1000) / 10);
  SDL_WM_SetCaption(m_caption_buffer_, NULL);

  if (!m_user_input_sys_->Update(current_simulation_time, DT)) {
    return false;
  }
  if (!m_game_logic_sys_->Update(current_simulation_time, DT)) {
    return false;
  }
  return true;
}

void TetrisGame::Render(float32 interpolation_factor) {
  m_graphics_sys_->Render(interpolation_factor);
}
