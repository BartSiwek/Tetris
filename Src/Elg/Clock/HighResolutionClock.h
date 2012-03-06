/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#ifndef ELG_CLOCK_HIGHRESOLUTIONCLOCK_H_
#define ELG_CLOCK_HIGHRESOLUTIONCLOCK_H_

#include "Elg/Clock/DurationTypedefs.h"

namespace Elg { namespace Clock { template<typename ClockType, typename DurationType> class TimePoint; } }

namespace Elg {
namespace Clock {

class HighResolutionClock {
 public:
  typedef Nanoseconds Duration;
  typedef NanosecondsRepr Repr;
  typedef NanosecondsPeriod Period;
  typedef TimePoint<HighResolutionClock, Duration> TimePoint;

  static const bool IsMonotonic = true;

  static Repr TimeSinceEpoch();
  static TimePoint Now();
};

}  // namespace Elg
}  // namespace Clock

#endif  // ELG_CLOCK_HIGHRESOLUTIONCLOCK_H_
