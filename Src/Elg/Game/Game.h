/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_GAME_GAME_H_
#define ELG_GAME_GAME_H_

#include "Elg/Utilities/Types.h"

#include "Elg/Clock/GameClock.h"

namespace Elg {
namespace Game {

template<typename GameImpl>
class Game {
 public:
  // Constructors
  Game(int32 argc, char** argv);

  // Destructor
  virtual ~Game();

  // Methods, including static methods
  int32 Run();

 private:
  // Data Members [static]
  static const Elg::Clock::GameClock::Duration DT;

  // Data Members [instance]
  Elg::Clock::GameClock::TimePoint m_current_simulation_time_;  // Current simulation time
  Elg::Clock::GameClock::TimePoint m_current_game_time_;        // Current game time
  Elg::Clock::GameClock::Duration m_accumulator_;

  // Data Members [implementations]
  GameImpl m_game_implementation_;
};

}  // namespace Game
}  // namespace Elg

#include "Elg/Game/Game-inl.h"

#endif  // ELG_GAME_GAME_H_
