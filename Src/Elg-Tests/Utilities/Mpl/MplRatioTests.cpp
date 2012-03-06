/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Utilities/Mpl/MplRatio.h"

TEST(Ratio_Basic) {
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Ratio1;
  CHECK_EQUAL(1, Ratio1::Num);
  CHECK_EQUAL(2, Ratio1::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Ratio2;
  CHECK_EQUAL(-1, Ratio2::Num);
  CHECK_EQUAL(2, Ratio2::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, -2> Ratio3;
  CHECK_EQUAL(-1, Ratio3::Num);
  CHECK_EQUAL(2, Ratio3::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, -2> Ratio4;
  CHECK_EQUAL(1, Ratio4::Num);
  CHECK_EQUAL(2, Ratio4::Den);

  typedef Elg::Utilities::Mpl::Ratio<9, 6> Ratio5;
  CHECK_EQUAL(3, Ratio5::Num);
  CHECK_EQUAL(2, Ratio5::Den);

  typedef Elg::Utilities::Mpl::Ratio<-9, 6> Ratio6;
  CHECK_EQUAL(-3, Ratio6::Num);
  CHECK_EQUAL(2, Ratio6::Den);

  typedef Elg::Utilities::Mpl::Ratio<9, -6> Ratio7;
  CHECK_EQUAL(-3, Ratio7::Num);
  CHECK_EQUAL(2, Ratio7::Den);

  typedef Elg::Utilities::Mpl::Ratio<-9, -6> Ratio8;
  CHECK_EQUAL(3, Ratio8::Num);
  CHECK_EQUAL(2, Ratio8::Den);
};

TEST(Ratio_Add) {
  // Addtion when denominats GCD == 1
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs1;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(5, Result1::Num);
  CHECK_EQUAL(6, Result1::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs2;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(-1, Result2::Num);
  CHECK_EQUAL(6, Result2::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs3;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(1, Result3::Num);
  CHECK_EQUAL(6, Result3::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs4;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(-5, Result4::Num);
  CHECK_EQUAL(6, Result4::Den);

  // When denominators GCD == 2
  typedef Elg::Utilities::Mpl::Ratio<1, 4> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<1, 6> Rhs5;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(5, Result5::Num);
  CHECK_EQUAL(12, Result5::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 4> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<1, 6> Rhs6;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(-1, Result6::Num);
  CHECK_EQUAL(12, Result6::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, 4> Lhs7;
  typedef Elg::Utilities::Mpl::Ratio<-1, 6> Rhs7;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs7, Rhs7> Result7;
  CHECK_EQUAL(1, Result7::Num);
  CHECK_EQUAL(12, Result7::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 4> Lhs8;
  typedef Elg::Utilities::Mpl::Ratio<-1, 6> Rhs8;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs8, Rhs8> Result8;
  CHECK_EQUAL(-5, Result8::Num);
  CHECK_EQUAL(12, Result8::Den);

  // When denominators GCD == 6 and secondary GCD = 3
  typedef Elg::Utilities::Mpl::Ratio<1, 6> Lhs9;
  typedef Elg::Utilities::Mpl::Ratio<1, 12> Rhs9;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs9, Rhs9> Result9;
  CHECK_EQUAL(1, Result9::Num);
  CHECK_EQUAL(4, Result9::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 6> Lhs10;
  typedef Elg::Utilities::Mpl::Ratio<1, 12> Rhs10;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs10, Rhs10> Result10;
  CHECK_EQUAL(-1, Result10::Num);
  CHECK_EQUAL(12, Result10::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, 6> Lhs11;
  typedef Elg::Utilities::Mpl::Ratio<-1, 12> Rhs11;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs11, Rhs11> Result11;
  CHECK_EQUAL(1, Result11::Num);
  CHECK_EQUAL(12, Result11::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 6> Lhs12;
  typedef Elg::Utilities::Mpl::Ratio<-1, 12> Rhs12;
  typedef Elg::Utilities::Mpl::RatioAdd<Lhs12, Rhs12> Result12;
  CHECK_EQUAL(-1, Result12::Num);
  CHECK_EQUAL(4, Result12::Den);
}

TEST(Ratio_Multiply) {
  // Multiply when both GCD are == 1
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs1;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(1, Result1::Num);
  CHECK_EQUAL(6, Result1::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs2;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(-1, Result2::Num);
  CHECK_EQUAL(6, Result2::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs3;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(-1, Result3::Num);
  CHECK_EQUAL(6, Result3::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs4;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(1, Result4::Num);
  CHECK_EQUAL(6, Result4::Den);

  // When Lhs num and Rhs den has GCD == 3 and Lhs den and Rhs num has GCD == 1
  typedef Elg::Utilities::Mpl::Ratio<3, 2> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs5;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(1, Result5::Num);
  CHECK_EQUAL(2, Result5::Den);

  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs6;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(-1, Result6::Num);
  CHECK_EQUAL(2, Result6::Den);

  typedef Elg::Utilities::Mpl::Ratio<3, 2> Lhs7;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs7;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs7, Rhs7> Result7;
  CHECK_EQUAL(-1, Result7::Num);
  CHECK_EQUAL(2, Result7::Den);

  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Lhs8;
  typedef Elg::Utilities::Mpl::Ratio<-1, 3> Rhs8;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs8, Rhs8> Result8;
  CHECK_EQUAL(1, Result8::Num);
  CHECK_EQUAL(2, Result8::Den);

  // When Lhs num and Rhs den has GCD == 1 and Lhs den and Rhs num has GCD == 2
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs9;
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Rhs9;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs9, Rhs9> Result9;
  CHECK_EQUAL(1, Result9::Num);
  CHECK_EQUAL(3, Result9::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs10;
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Rhs10;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs10, Rhs10> Result10;
  CHECK_EQUAL(-1, Result10::Num);
  CHECK_EQUAL(3, Result10::Den);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs11;
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Rhs11;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs11, Rhs11> Result11;
  CHECK_EQUAL(-1, Result11::Num);
  CHECK_EQUAL(3, Result11::Den);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs12;
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Rhs12;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs12, Rhs12> Result12;
  CHECK_EQUAL(1, Result12::Num);
  CHECK_EQUAL(3, Result12::Den);

  // When Lhs num and Rhs den has GCD == 3 and Lhs den and Rhs num has GCD == 2
  typedef Elg::Utilities::Mpl::Ratio<3, 2> Lhs13;
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Rhs13;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs13, Rhs13> Result13;
  CHECK_EQUAL(1, Result13::Num);
  CHECK_EQUAL(1, Result13::Den);

  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Lhs14;
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Rhs14;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs14, Rhs14> Result14;
  CHECK_EQUAL(-1, Result14::Num);
  CHECK_EQUAL(1, Result14::Den);

  typedef Elg::Utilities::Mpl::Ratio<3, 2> Lhs15;
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Rhs15;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs15, Rhs15> Result15;
  CHECK_EQUAL(-1, Result15::Num);
  CHECK_EQUAL(1, Result15::Den);

  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Lhs16;
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Rhs16;
  typedef Elg::Utilities::Mpl::RatioMultiply<Lhs16, Rhs16> Result16;
  CHECK_EQUAL(1, Result16::Num);
  CHECK_EQUAL(1, Result16::Den);
}

TEST(Ratio_Equal) {
  // Equal
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Rhs1;
  typedef Elg::Utilities::Mpl::RatioEqual<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(true, Result1::value);

  typedef Elg::Utilities::Mpl::Ratio<3, 6> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Rhs2;
  typedef Elg::Utilities::Mpl::RatioEqual<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(true, Result2::value);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<2, 4> Rhs3;
  typedef Elg::Utilities::Mpl::RatioEqual<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(true, Result3::value);

  typedef Elg::Utilities::Mpl::Ratio<3, 6> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<2, 4> Rhs4;
  typedef Elg::Utilities::Mpl::RatioEqual<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(true, Result4::value);

  // Not equal
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs5;
  typedef Elg::Utilities::Mpl::RatioEqual<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(false, Result5::value);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<1, 3> Rhs6;
  typedef Elg::Utilities::Mpl::RatioNotEqual<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(true, Result6::value);
}

TEST(Ratio_Less_Obvious) {
  // Less (obvious cases)
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<0, 2> Rhs1;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(false, Result1::value);

  typedef Elg::Utilities::Mpl::Ratio<1, 2> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Rhs2;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(false, Result2::value);

  typedef Elg::Utilities::Mpl::Ratio<0, 2> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Rhs3;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(true, Result3::value);

  typedef Elg::Utilities::Mpl::Ratio<0, 2> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Rhs4;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(false, Result4::value);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<1, 2> Rhs5;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(true, Result5::value);

  typedef Elg::Utilities::Mpl::Ratio<-1, 2> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<0, 2> Rhs6;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(true, Result6::value);
}

TEST(Ratio_Less_Positive) {
  // Less (the positive case)
  typedef Elg::Utilities::Mpl::Ratio<0, 1> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<1, 1> Rhs1;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(true, Result1::value);

  typedef Elg::Utilities::Mpl::Ratio<1, 1> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<0, 1> Rhs2;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(false, Result2::value);

  // Zero levels
  typedef Elg::Utilities::Mpl::Ratio<1, 1> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<2, 1> Rhs3;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(true, Result3::value);

  typedef Elg::Utilities::Mpl::Ratio<2, 1> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<1, 1> Rhs4;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(false, Result4::value);

  // One level
  typedef Elg::Utilities::Mpl::Ratio<3, 2> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<4, 3> Rhs5;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(false, Result5::value);

  typedef Elg::Utilities::Mpl::Ratio<4, 3> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<3, 2> Rhs6;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(true, Result6::value);

  // Two levels
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Lhs7;
  typedef Elg::Utilities::Mpl::Ratio<3, 4> Rhs7;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs7, Rhs7> Result7;
  CHECK_EQUAL(true, Result7::value);

  typedef Elg::Utilities::Mpl::Ratio<3, 4> Lhs8;
  typedef Elg::Utilities::Mpl::Ratio<2, 3> Rhs8;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs8, Rhs8> Result8;
  CHECK_EQUAL(false, Result8::value);
}

TEST(Ratio_Less_Negative) {
  // Less (the negative case)
  typedef Elg::Utilities::Mpl::Ratio<0, 1> Lhs1;
  typedef Elg::Utilities::Mpl::Ratio<-1, 1> Rhs1;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs1, Rhs1> Result1;
  CHECK_EQUAL(false, Result1::value);

  typedef Elg::Utilities::Mpl::Ratio<-1, 1> Lhs2;
  typedef Elg::Utilities::Mpl::Ratio<0, 1> Rhs2;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs2, Rhs2> Result2;
  CHECK_EQUAL(true, Result2::value);

  // Zero levels
  typedef Elg::Utilities::Mpl::Ratio<-1, 1> Lhs3;
  typedef Elg::Utilities::Mpl::Ratio<-2, 1> Rhs3;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs3, Rhs3> Result3;
  CHECK_EQUAL(false, Result3::value);

  typedef Elg::Utilities::Mpl::Ratio<-2, 1> Lhs4;
  typedef Elg::Utilities::Mpl::Ratio<-1, 1> Rhs4;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs4, Rhs4> Result4;
  CHECK_EQUAL(true, Result4::value);

  // One level
  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Lhs5;
  typedef Elg::Utilities::Mpl::Ratio<-4, 3> Rhs5;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs5, Rhs5> Result5;
  CHECK_EQUAL(true, Result5::value);

  typedef Elg::Utilities::Mpl::Ratio<-4, 3> Lhs6;
  typedef Elg::Utilities::Mpl::Ratio<-3, 2> Rhs6;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs6, Rhs6> Result6;
  CHECK_EQUAL(false, Result6::value);

  // Two levels
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Lhs7;
  typedef Elg::Utilities::Mpl::Ratio<-3, 4> Rhs7;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs7, Rhs7> Result7;
  CHECK_EQUAL(false, Result7::value);

  typedef Elg::Utilities::Mpl::Ratio<-3, 4> Lhs8;
  typedef Elg::Utilities::Mpl::Ratio<-2, 3> Rhs8;
  typedef Elg::Utilities::Mpl::RatioLess<Lhs8, Rhs8> Result8;
  CHECK_EQUAL(true, Result8::value);
}
