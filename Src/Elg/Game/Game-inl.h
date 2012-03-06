/*s
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_GAME_GAME_INL_H_
#define ELG_GAME_GAME_INL_H_

#include "Elg/Game/Game.h"

#include "Elg/Utilities/Types.h"
#include "Elg/Clock/GameClock.h"
#include "Elg/Performance/FrameStatistics.h"
#include "Elg/Game/GameProfiling.h"

namespace Elg {
namespace Game {

template<typename GameImpl>
const Elg::Clock::GameClock::Duration Game<GameImpl>::DT =
  Elg::Clock::GameClock::Duration(Elg::Clock::Seconds(1)) / 60;

template<typename GameImpl>
Game<GameImpl>::Game(int32 argc, char** argv)
    : m_game_implementation_(argc, argv),
      m_current_simulation_time_(Elg::Clock::GameClock::TimePoint::Epoch()),
      m_current_game_time_(Elg::Clock::GameClock::TimePoint::Epoch()),
      m_accumulator_(Elg::Clock::GameClock::Duration::Zero()) {
}

template<typename GameImpl>
Game<GameImpl>::~Game() {
}

template<typename GameImpl>
int32 Game<GameImpl>::Run() {
  m_current_game_time_ = Elg::Clock::GameClock::Now();
  m_accumulator_ = DT;

  for (;;) {
    ELG_PROFILE_GAME_LOOP;

    Elg::Clock::GameClock::TimePoint new_time = Elg::Clock::GameClock::Now();
    Elg::Clock::GameClock::Duration frame_time = new_time - m_current_game_time_;

    Elg::Performance::FrameStatistics::RegisterFrameTime(frame_time);

    if (frame_time > Elg::Clock::Milliseconds(250)) {
      // Note: Clamp max frame time to avoid spiral of death...
      frame_time = Elg::Clock::Milliseconds(250);
    }

    m_current_game_time_ = new_time;
    m_accumulator_ += frame_time;

    while (m_accumulator_ >= DT) {
      // Update and check for quit condition
      bool should_quit = !m_game_implementation_.Update(m_current_simulation_time_, DT);
      if (should_quit) {
        return 0;
      }

      m_current_simulation_time_ += DT;
      m_accumulator_ -= DT;
    }

    const float32 interpolation_factor =
      static_cast<float32>(Elg::Clock::DurationCast<Elg::Clock::Milliseconds>(m_accumulator_).Count()) /
      static_cast<float32>(Elg::Clock::DurationCast<Elg::Clock::Milliseconds>(DT).Count());
    m_game_implementation_.Render(interpolation_factor);
  }
  // Ureachable...
}

}  // namespace Game
}  // namespace Elg

#endif  // ELG_GAME_GAME_INL_H_
