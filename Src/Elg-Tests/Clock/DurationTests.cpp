/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Clock/Duration.h"

TEST(Duration_CommonType) {
  typedef Elg::Clock::Duration<int32, Elg::Utilities::Mpl::Ratio<1, 10>> DurationType1;
  typedef Elg::Clock::Duration<float64, Elg::Utilities::Mpl::Ratio<1, 6>> DurationType2;
  typedef std::common_type<DurationType1, DurationType2>::type ResultType;

  bool is_same_result = std::is_same<typename ResultType::Repr, float64>::value;
  CHECK_EQUAL(true, is_same_result);
  CHECK_EQUAL(1, ResultType::Period::Num);
  CHECK_EQUAL(30, ResultType::Period::Den);
}

TEST(Duration_Construction) {
  // Default constructor
  Elg::Clock::Duration<int32> d1;

  // Copy constructor
  Elg::Clock::Duration<int32> d2 = d1;

  // Constructor form another representation
  Elg::Clock::Duration<int32> d3(static_cast<int16>(100));
  Elg::Clock::Duration<int16> d4 = Elg::Clock::Duration<int16>(100);

  // Constructor form another representation - not working
  // Elg::Clock::Duration<int32> d5(3.5);
  Elg::Clock::Duration<int32, Elg::Utilities::Mpl::Ratio<1, 10>> d6(100);
  Elg::Clock::Duration<int32, Elg::Utilities::Mpl::Ratio<1, 100>> d7 = d6;
  // Elg::Clock::Duration<int32, Elg::Utilities::Mpl::Ratio<1, 10>> d8 = d7;

  // Constructor from another duration
  Elg::Clock::Duration<int16> d9 = Elg::Clock::Duration<int16>(100);
  Elg::Clock::Duration<int32> d10 = d9;
}

TEST(Duration_Casting) {
  Elg::Clock::Duration<int32> d1(100);
  Elg::Clock::Duration<float32> d2 = Elg::Clock::DurationCast<Elg::Clock::Duration<float32>>(d1);
  Elg::Clock::Duration<int32> d3 = Elg::Clock::DurationCast<Elg::Clock::Duration<int32>>(d2);
}

TEST(Duration_Artihmetic) {
  Elg::Clock::Duration<int32> d1(100);
  Elg::Clock::Duration<float64> d2 = d1 * 2.5;
  Elg::Clock::Duration<float64> d3 = d1 * 1.5;
  Elg::Clock::Duration<int32> d4 = d1 / 2;
  Elg::Clock::Duration<int32> d5 = d1 % 3;
  Elg::Clock::Duration<int32> d6 = d1 % Elg::Clock::Duration<int32>(22);
  Elg::Clock::Duration<float64> d7 = d1 + d2;
  Elg::Clock::Duration<float64> d8 = d1 - d2;
  float64 result = d2 / d1;

  CHECK_EQUAL(250.0, d2.Count());
  CHECK_EQUAL(150.0, d3.Count());
  CHECK_EQUAL(50, d4.Count());
  CHECK_EQUAL(1, d5.Count());
  CHECK_EQUAL(12, d6.Count());
  CHECK_EQUAL(350.0, d7.Count());
  CHECK_EQUAL(-150.0, d8.Count());
  CHECK_EQUAL(2.5, result);
}

TEST(Duration_Comparison) {
  Elg::Clock::Duration<int32> d1(100);
  Elg::Clock::Duration<int32> d2(50);

  CHECK_EQUAL(false, d1 == d2);
  CHECK_EQUAL(true, d1 != d2);

  CHECK_EQUAL(false, d1 < d2);
  CHECK_EQUAL(false, d1 <= d2);
  CHECK_EQUAL(true, d1 > d2);
  CHECK_EQUAL(true, d1 >= d2);
}
