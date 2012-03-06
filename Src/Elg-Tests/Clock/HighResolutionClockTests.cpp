/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include <cstdio>

#include "Elg/Clock/TimePoint.h"
#include "Elg/Clock/HighResolutionClock.h"

TEST(HighResolutionClock_Simple) {
  Elg::Clock::HighResolutionClock::TimePoint tstart = Elg::Clock::HighResolutionClock::Now();
  float32 a = static_cast<float32>(2.0);
  float32 b = static_cast<float32>(0.33);
  for (uint32 x = 0;x < 1000000;++x) {
    a = a * b;
    b = a * a;
  }
  Elg::Clock::HighResolutionClock::TimePoint tend = Elg::Clock::HighResolutionClock::Now();

  fprintf(stderr, "Time it takes to do 2 * 10^6 loops of float32 multiplication is:\n");
  fprintf(stderr, "-> %lld ns\n", (tend - tstart).Count());
  fprintf(stderr, "-> Avg: %f ns\n", (tend - tstart).Count() / float32(2000000));
  fprintf(stderr, "-> %lld us\n", Elg::Clock::DurationCast<Elg::Clock::Microseconds>(tend - tstart).Count());
  fprintf(stderr, "-> %lld ms\n", Elg::Clock::DurationCast<Elg::Clock::Milliseconds>(tend - tstart).Count());
  fprintf(stderr, "-> %lld s\n", Elg::Clock::DurationCast<Elg::Clock::Seconds>(tend - tstart).Count());
}
