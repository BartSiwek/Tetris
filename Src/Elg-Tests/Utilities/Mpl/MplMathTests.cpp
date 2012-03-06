/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Utilities/Mpl/MplMath.h"

TEST(Mpl_Sign) {
  CHECK_EQUAL(-1, Elg::Utilities::Mpl::Sign<-100>::value);
  CHECK_EQUAL(0, Elg::Utilities::Mpl::Sign<0>::value);
  CHECK_EQUAL(1, Elg::Utilities::Mpl::Sign<100>::value);
}

TEST(Mpl_Abs) {
  CHECK_EQUAL(100, Elg::Utilities::Mpl::Abs<-100>::value);
  CHECK_EQUAL(0, Elg::Utilities::Mpl::Abs<0>::value);
  CHECK_EQUAL(100, Elg::Utilities::Mpl::Abs<100>::value);
}

TEST(Mpl_Multiply_Basic) {
  typedef Elg::Utilities::Mpl::Multiply<10, 10> Result1;
  CHECK_EQUAL(100, Result1::value);

  typedef Elg::Utilities::Mpl::Multiply<10, -10> Result2;
  CHECK_EQUAL(-100, Result2::value);

  typedef Elg::Utilities::Mpl::Multiply<-10, 10> Result3;
  CHECK_EQUAL(-100, Result3::value);

  typedef Elg::Utilities::Mpl::Multiply<-10, -10> Result4;
  CHECK_EQUAL(100, Result4::value);
}

TEST(Mpl_Multiply_AlmostOverflow) {
  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), static_cast<int64>(0x7fffffff)> Almost1;
  CHECK_EQUAL(static_cast<int64>(0x7fffffff00000000), Almost1::value);

  typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), static_cast<int64>(0x7fffffff)> Almost2;
  CHECK_EQUAL(-static_cast<int64>(0x7fffffff00000000), Almost2::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), -static_cast<int64>(0x7fffffff)> Almost3;
  CHECK_EQUAL(-static_cast<int64>(0x7fffffff00000000), Almost3::value);

  typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), -static_cast<int64>(0x7fffffff)> Almost4;
  CHECK_EQUAL(static_cast<int64>(0x7fffffff00000000), Almost4::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x10000000), static_cast<int64>(0x10000000)> Almost5;
  CHECK_EQUAL(static_cast<int64>(0x100000000000000), Almost5::value);

  typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x10000000), static_cast<int64>(0x10000000)> Almost6;
  CHECK_EQUAL(-static_cast<int64>(0x100000000000000), Almost6::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x10000000), -static_cast<int64>(0x10000000)> Almost7;
  CHECK_EQUAL(-static_cast<int64>(0x100000000000000), Almost7::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x10000000), static_cast<int64>(0x10000000)> Almost8;
  CHECK_EQUAL(static_cast<int64>(0x100000000000000), Almost8::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x7fffffff), static_cast<int64>(0xffffffff)> Almost9;
  CHECK_EQUAL(static_cast<int64>(0x7ffffffe80000001), Almost9::value);

  // Here the bottom part overflows but its okay :)
  typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x7fffffff), static_cast<int64>(0xffffffff)> Almost10;
  CHECK_EQUAL(-static_cast<int64>(0x7ffffffe80000001), Almost10::value);

  typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x7fffffff), -static_cast<int64>(0xffffffff)> Almost11;
  CHECK_EQUAL(-static_cast<int64>(0x7ffffffe80000001), Almost11::value);

  typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x7fffffff), -static_cast<int64>(0xffffffff)> Almost12;
  CHECK_EQUAL(static_cast<int64>(0x7ffffffe80000001), Almost12::value);
}

TEST(Mpl_Multiply_Overflow) {
  // Obvious overflow
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), static_cast<int64>(0x100000000)> Over1;
  // CHECK_EQUAL(0, Over1::value);
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), -static_cast<int64>(0x100000000)> Over2;
  // CHECK_EQUAL(0, Over2::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), static_cast<int64>(0x100000000)> Over3;
  // CHECK_EQUAL(0, Over3::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), -static_cast<int64>(0x100000000)> Over4;
  // CHECK_EQUAL(0, Over4::value);

  // Overflow due to sign bit
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), static_cast<int64>(0xffffffff)> Over5;
  // CHECK_EQUAL(static_cast<int64>(0xffffffff00000000), Over5::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), static_cast<int64>(0xffffffff)> Over6;
  // CHECK_EQUAL(-static_cast<int64>(0xffffffff00000000), Over6::value);
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), -static_cast<int64>(0xffffffff)> Over7;
  // CHECK_EQUAL(-static_cast<int64>(0xffffffff00000000), Over7::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), -static_cast<int64>(0xffffffff)> Over8;
  // CHECK_EQUAL(static_cast<int64>(0xffffffff00000000), Over8::value);

  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), static_cast<int64>(0x8fffffff)> Over9;
  // CHECK_EQUAL(0, Over9::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), static_cast<int64>(0x8fffffff)> Over10;
  // CHECK_EQUAL(-static_cast<int64>(0x8fffffff00000000), Over10::value);
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x100000000), -static_cast<int64>(0x8fffffff)> Over11;
  // CHECK_EQUAL(-static_cast<int64>(0x8fffffff00000000), Over11::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x100000000), -static_cast<int64>(0x8fffffff)> Over12;
  // CHECK_EQUAL(static_cast<int64>(0x8fffffff00000000), Over12::value);

  // Overflow after sumation
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x1ffffffff), static_cast<int64>(0x7fffffff)> Over13;
  // CHECK_EQUAL(static_cast<int64>(0x7fffffff00000000), Over13::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x1ffffffff), static_cast<int64>(0x7fffffff)> Over14;
  // CHECK_EQUAL(-static_cast<int64>(0x7fffffff00000000), Over14::value);
  // typedef Elg::Utilities::Mpl::Multiply<static_cast<int64>(0x1ffffffff), -static_cast<int64>(0x7fffffff)> Over15;
  // CHECK_EQUAL(-static_cast<int64>(0x7fffffff00000000), Over15::value);
  // typedef Elg::Utilities::Mpl::Multiply<-static_cast<int64>(0x1ffffffff), -static_cast<int64>(0x7fffffff)> Over16;
  // CHECK_EQUAL(static_cast<int64>(0x7fffffff00000000), Over16::value);
}

TEST(Add_Multiply_Basic) {
  typedef Elg::Utilities::Mpl::Add<1, 2> Result1;
  CHECK_EQUAL(3, Result1::value);

  typedef Elg::Utilities::Mpl::Add<1, -2> Result2;
  CHECK_EQUAL(-1, Result2::value);

  typedef Elg::Utilities::Mpl::Add<3, -2> Result3;
  CHECK_EQUAL(1, Result3::value);

  typedef Elg::Utilities::Mpl::Add<-1, 2> Result4;
  CHECK_EQUAL(1, Result4::value);

  typedef Elg::Utilities::Mpl::Add<-3, 2> Result5;
  CHECK_EQUAL(-1, Result5::value);

  typedef Elg::Utilities::Mpl::Add<-1, 1> Result6;
  CHECK_EQUAL(0, Result6::value);
}

TEST(Mpl_Add_AlmostOverflow) {
  typedef Elg::Utilities::Mpl::Add<static_cast<int64>(0x7ffffffffffffffe), static_cast<int64>(1)> Almost1;
  CHECK_EQUAL(static_cast<int64>(0x7fffffffffffffff), Almost1::value);

  typedef Elg::Utilities::Mpl::Add<-static_cast<int64>(0x7ffffffffffffffe), static_cast<int64>(1)> Almost2;
  CHECK_EQUAL(-static_cast<int64>(0x7ffffffffffffffd), Almost2::value);

  typedef Elg::Utilities::Mpl::Add<static_cast<int64>(0x7ffffffffffffffe), -static_cast<int64>(1)> Almost3;
  CHECK_EQUAL(static_cast<int64>(0x7ffffffffffffffd), Almost3::value);

  typedef Elg::Utilities::Mpl::Add<-static_cast<int64>(0x7ffffffffffffffe), -static_cast<int64>(1)> Almost4;
  CHECK_EQUAL(-static_cast<int64>(0x7fffffffffffffff), Almost4::value);

  typedef Elg::Utilities::Mpl::Add<-static_cast<int64>(0x7fffffffffffffff), -static_cast<int64>(1)> Almost5;
  CHECK_EQUAL(-static_cast<int64>(0x8000000000000000), Almost5::value);
}

TEST(Mpl_Add_Overflow) {
  // Top overflow
  // typedef Elg::Utilities::Mpl::Add<static_cast<int64>(0x7fffffffffffffff), static_cast<int64>(1)> Over1;
  // CHECK_EQUAL(static_cast<int64>(0x8000000000000000), Over1::value);

  // Bottom overflow
  // typedef Elg::Utilities::Mpl::Add<-static_cast<int64>(0x8000000000000000), -static_cast<int64>(1)> Over2;
  // CHECK_EQUAL(-static_cast<int64>(0x80000000000000), Over2::value);
}
