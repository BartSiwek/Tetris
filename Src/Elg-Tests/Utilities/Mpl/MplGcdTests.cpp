/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Utilities/Mpl/MplGcd.h"

TEST(Mpl_Gcd) {
  typedef Elg::Utilities::Mpl::Gcd<3, 5> Result1;
  CHECK_EQUAL(1, Result1::value);

  typedef Elg::Utilities::Mpl::Gcd<5, 3> Result2;
  CHECK_EQUAL(1, Result2::value);

  typedef Elg::Utilities::Mpl::Gcd<-3, 5> Result3;
  CHECK_EQUAL(1, Result3::value);

  typedef Elg::Utilities::Mpl::Gcd<5, -3> Result4;
  CHECK_EQUAL(1, Result4::value);

  typedef Elg::Utilities::Mpl::Gcd<3, -5> Result5;
  CHECK_EQUAL(1, Result5::value);

  typedef Elg::Utilities::Mpl::Gcd<-5, 3> Result6;
  CHECK_EQUAL(1, Result6::value);

  typedef Elg::Utilities::Mpl::Gcd<-3, -5> Result7;
  CHECK_EQUAL(1, Result7::value);

  typedef Elg::Utilities::Mpl::Gcd<-5, -3> Result8;
  CHECK_EQUAL(1, Result8::value);

  typedef Elg::Utilities::Mpl::Gcd<6, 10> Result9;
  CHECK_EQUAL(2, Result9::value);

  typedef Elg::Utilities::Mpl::Gcd<10, 6> Result10;
  CHECK_EQUAL(2, Result10::value);

  typedef Elg::Utilities::Mpl::Gcd<6, 15> Result11;
  CHECK_EQUAL(3, Result11::value);

  typedef Elg::Utilities::Mpl::Gcd<15, 6> Result12;
  CHECK_EQUAL(3, Result12::value);

  typedef Elg::Utilities::Mpl::Gcd<10, 15> Result13;
  CHECK_EQUAL(5, Result13::value);

  typedef Elg::Utilities::Mpl::Gcd<15, 10> Result14;
  CHECK_EQUAL(5, Result14::value);
}

TEST(Mpl_Gcd_CornerCases) {
  typedef Elg::Utilities::Mpl::Gcd<0, 10> Result1;
  CHECK_EQUAL(10, Result1::value);

  typedef Elg::Utilities::Mpl::Gcd<10, 0> Result2;
  CHECK_EQUAL(10, Result2::value);

  typedef Elg::Utilities::Mpl::Gcd<0, -10> Result3;
  CHECK_EQUAL(10, Result3::value);

  typedef Elg::Utilities::Mpl::Gcd<-10, 0> Result4;
  CHECK_EQUAL(10, Result4::value);
}
