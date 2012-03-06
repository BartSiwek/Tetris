/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#include "Elg/Clock/GameClock.h"
#include "Elg/Clock/TimePoint.h"
#include "Elg/Clock/HighResolutionClock.h"

namespace Elg {
namespace Clock {

GameClock::TimePoint GameClock::Now() {
  return HighResolutionClock::Now();
}

}  // namespace Elg
}  // namespace Clock
