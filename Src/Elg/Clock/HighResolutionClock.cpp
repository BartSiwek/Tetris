/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#include "Elg/Clock/HighResolutionClock.h"

#include "Elg/Utilities/Asserts.h"
#include "Elg/Clock/TimePoint.h"

#ifdef _WIN32

// This is used only for this define case
#include <windows.h>  // NOLINT(build/include_order)

namespace Elg {
namespace Clock {

int64 GetFrequency() {
  LARGE_INTEGER large_integer;
  VERIFY_TRUE(QueryPerformanceFrequency(&large_integer), "QueryPerformanceFrequency failed");
  return large_integer.QuadPart;
}

int64 GetStartTime() {
  LARGE_INTEGER large_integer;
  VERIFY_TRUE(QueryPerformanceCounter(&large_integer), "QueryPerformanceCounter failed");
  return large_integer.QuadPart;
}

int64 g_frequency_ = GetFrequency();
int64 g_start_time_ = GetStartTime();

HighResolutionClock::Repr HighResolutionClock::TimeSinceEpoch() {
  LARGE_INTEGER large_integer;
  VERIFY_TRUE(QueryPerformanceCounter(&large_integer), "QueryPerformanceCounter failed");
  int64 nanosecond_count = (1000 * 1000 * 1000 * (large_integer.QuadPart - g_start_time_)) / g_frequency_;
  return nanosecond_count;
}

HighResolutionClock::TimePoint HighResolutionClock::Now() {
  LARGE_INTEGER large_integer;
  VERIFY_TRUE(QueryPerformanceCounter(&large_integer), "QueryPerformanceCounter failed");
  int64 nanosecond_count = (1000 * 1000 * 1000 * (large_integer.QuadPart - g_start_time_)) / g_frequency_;
  return TimePoint(HighResolutionClock::Duration(nanosecond_count));
}

}  // namespace Elg
}  // namespace Clock

#else
#error "Undefined high resolution counter"
#endif  // _WIN32
