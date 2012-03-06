/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_CLOCK_GAMECLOCK_H_
#define ELG_CLOCK_GAMECLOCK_H_

#include "Elg/Utilities/Types.h"

#include "Elg/Clock/HighResolutionClock.h"

namespace Elg {
namespace Clock {

class GameClock {
 public:
  typedef HighResolutionClock ClockImplementation;
  typedef ClockImplementation::Duration Duration;
  typedef ClockImplementation::Repr Repr;
  typedef ClockImplementation::Period Period;
  typedef ClockImplementation::TimePoint TimePoint;

  static const bool IsMonotonic = ClockImplementation::IsMonotonic;

  static TimePoint Now();
};

}  // namespace Clock
}  // namespace Elg

#endif  // ELG_CLOCK_GAMECLOCK_H_
