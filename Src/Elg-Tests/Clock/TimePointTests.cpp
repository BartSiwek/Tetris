/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Clock/TimePoint.h"

TEST(TimePoint_Construction) {
  // Default constructor
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p1;
  CHECK_EQUAL(0, p1.TimeSinceEpoch().Count());

  // Duration constructor
  Elg::Clock::Milliseconds d2(100);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p2(d2);
  CHECK_EQUAL(100, p2.TimeSinceEpoch().Count());

  // Other duration constructor
  Elg::Clock::Milliseconds d3(50);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p3(d3);
  Elg::Clock::TimePoint<void, Elg::Clock::Microseconds> p4(p3);
  CHECK_EQUAL(50000, p4.TimeSinceEpoch().Count());
}

TEST(TimePoint_Casting) {
  Elg::Clock::Milliseconds d1(50);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p1(d1);

  Elg::Clock::TimePoint<void, Elg::Clock::Microseconds> p2 =
    Elg::Clock::TimePointCast<Elg::Clock::Microseconds>(p1);

  CHECK_EQUAL(50000, p2.TimeSinceEpoch().Count());
}

TEST(TimePoint_Artihmetic) {
  Elg::Clock::Milliseconds d1(100);
  Elg::Clock::Milliseconds d2(50);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p1(d1);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p2(d2);

  p1 += d2;
  p2 -= d1;

  CHECK_EQUAL(150, p1.TimeSinceEpoch().Count());
  CHECK_EQUAL(-50, p2.TimeSinceEpoch().Count());

  CHECK_EQUAL(250, (p1 + d1).TimeSinceEpoch().Count());
  CHECK_EQUAL(250, (d1 + p1).TimeSinceEpoch().Count());
  CHECK_EQUAL(-100, (p2 - d2).TimeSinceEpoch().Count());
  CHECK_EQUAL(200, (p1 - p2).Count());
}

TEST(TimePoint_Comparison) {
  Elg::Clock::Milliseconds d1(100);
  Elg::Clock::Milliseconds d2(50);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p1(d1);
  Elg::Clock::TimePoint<void, Elg::Clock::Milliseconds> p2(d2);

  CHECK_EQUAL(false, p1 == p2);
  CHECK_EQUAL(true, p1 != p2);

  CHECK_EQUAL(false, p1 < p2);
  CHECK_EQUAL(false, p1 <= p2);
  CHECK_EQUAL(true, p1 > p2);
  CHECK_EQUAL(true, p1 >= p2);
}
