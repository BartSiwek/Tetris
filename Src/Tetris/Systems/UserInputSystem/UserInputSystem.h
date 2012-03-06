/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_SYSTEMS_USERINPUTSYSTEM_USERINPUTSYSTEM_H_
#define TETRIS_SYSTEMS_USERINPUTSYSTEM_USERINPUTSYSTEM_H_

#include <SDL/SDL.h>

#include <vector>

#include "Elg/Clock/GameClock.h"

#include "Tetris/Components/AllComponents.h"
#include "Tetris/Helpers/Constants.h"

class UserInputSystem {
 public:
  // Constructors
  explicit UserInputSystem(TetrisEntityManager *entity_manager);

  // Destructors
  ~UserInputSystem();

  // Methods, including static methods
  void Initialize();
  bool Update(Elg::Clock::GameClock::TimePoint time, Elg::Clock::GameClock::Duration dt);

 private:
  // Methods, including static methods
  bool ProcessKeydownEvent(const SDL_Event &event, uint32 *event_count, TetrisEvent *events);
  void ProcessEvents(const uint32 &event_count, const TetrisEvent *events);

  // Data members
  TetrisEntityManager *m_entity_manager_;
};

#endif  // TETRIS_SYSTEMS_USERINPUTSYSTEM_USERINPUTSYSTEM_H_
