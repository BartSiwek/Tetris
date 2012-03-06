/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_MPL_MPLRATIO_H_
#define ELG_UTILITIES_MPL_MPLRATIO_H_

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/Utilities/Mpl/MplMath.h"
#include "Elg/Utilities/Mpl/MplGcd.h"

namespace Elg {
namespace Utilities {
namespace Mpl {

// Class representing rational numbers
template<int64 N, int64 D = 1>
struct Ratio {
  static_assert(D != 0, "Denominator cannot be zero");
  static_assert(N >= -static_cast<int64>(0x7fffffffffffffff) &&
                D >= -static_cast<int64>(0x7fffffffffffffff), "Out of range");

  // Note: N * sign(N) == abs(N)
  static const int64 Num = Sign<D>::value * N / Gcd<N, D>::value;

  static const int64 Den = Abs<D>::value / Gcd<N, D>::value;

  typedef Ratio<N, D> type;
};

template<int64 N, int64 D>
const int64 Ratio<N, D>::Num;

template<int64 N, int64 D>
const int64 Ratio<N, D>::Den;

namespace Detail {

// Helper classes
template<typename T>
struct IsRatio {
  static const bool value = false;
};

template<int64 N, int64 D>
struct IsRatio<Ratio<N, D>> {
  static const bool value = true;
};

// Ratio continous fraction compare
template<typename Lhs, typename Rhs>
struct RatioInverseCompare;

template<typename Lhs, typename Rhs>
struct RatioContinousFractionCompare;

template<typename Lhs, typename Rhs, int64 LhsSign = Sign<Lhs::Num>::value, int64 RhsSign = Sign<Rhs::Num>::value>
struct RatioSignBasedCompare;

// Ratio inverse compare
template<int64 LhsDen, typename Rhs>
struct RatioInverseCompare<Ratio<0, LhsDen>, Rhs> {
  static const bool value = true;
};

template<typename Lhs, int64 RhsDen>
struct RatioInverseCompare<Lhs, Ratio<0, RhsDen>> {
  static const bool value = false;
};

template<int64 LhsDen, int64 RhsDen>
struct RatioInverseCompare<Ratio<0, LhsDen>, Ratio<0, RhsDen>> {
  static const bool value = false;
};

template<typename Lhs, typename Rhs>
struct RatioInverseCompare {
  typedef Ratio<Rhs::Den, Rhs::Num> RhsInverse;
  typedef Ratio<Lhs::Den, Lhs::Num> LhsInverse;
  static const bool value = RatioContinousFractionCompare<RhsInverse, LhsInverse>::value;
};

// Ratio continous fraction compare - implementation
template<typename Lhs, typename Rhs>
struct RatioContinousFractionCompare {
 private:
  static const int64 LhsIntegral = Lhs::Num / Lhs::Den;
  static const int64 LhsRest = Lhs::Num % Lhs::Den;
  static const int64 RhsIntegral = Rhs::Num / Rhs::Den;
  static const int64 RhsRest = Rhs::Num % Rhs::Den;

 public:
  static const bool value = (LhsIntegral < RhsIntegral) ||
                            (LhsIntegral == RhsIntegral &&
                             RatioInverseCompare< Ratio<LhsRest, Lhs::Den>, Ratio<RhsRest, Rhs::Den> >::value);
};

// Ratio sign based compare - implementation
template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, -1, -1> {
  static const bool value = RatioContinousFractionCompare< Ratio<-Rhs::Num, Rhs::Den>,
                                                           Ratio<-Lhs::Num, Lhs::Den> >::value;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, -1, 0> {
  static const bool value = true;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, -1, 1> {
  static const bool value = true;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 0, -1> {
  static const bool value = false;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 0, 0> {
  static const bool value = false;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 0, 1> {
  static const bool value = true;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 1, -1> {
  static const bool value = false;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 1, 0> {
  static const bool value = false;
};

template<typename Lhs, typename Rhs>
struct RatioSignBasedCompare<Lhs, Rhs, 1, 1> {
  static const bool value = RatioContinousFractionCompare<Lhs, Rhs>::value;
};

}  // namespace Detail

// Rational number addition
template<typename Lhs, typename Rhs>
struct RatioAdd {
 private:
  // A little proof.
  // Let M = GCD(LhsDen, RhsDen) then LhsDen = M * LhsRest and RhsDen = M * RhsRest
  // To simplify the rational number we calculate
  //   GCD(LhsNum * RhsDen + RhsNum * LhsDen, RhsDen * LhsDen) =
  //   GCD(LhsNum * M * RhsRest + RhsNum * M * LhsRest, M * M * LhsRest * RhsRest) =
  //   M * GCD(LhsNum * RhsRest + RhsNum * LhsRest, M * LhsRest * RhsRest)
  // Lets denote
  //   N = LhsNum * RhsRest + RhsNum * LhsRest
  // Observe that
  //  GCD(N, M * LhsRest * RhsRest) = GCD(N, M)
  // This is true since:
  // A) If a number divides N and M then it divides (M * LhsRest * RhsRest) so GCD(N, M) is less or equal to
  //    GCD(N, M * LhsRest * RhsRest)
  // B) If a number (call it K) divides N and (M * LhsRest * RhsRest) then it must divide M. If not it would have to
  //    divide (LhsRest * RhsRest) but since these two are coprime it must divide one of them - assume LhsRest - and
  //    not the other (same argument works for RhsRest). Summarising we got a number K that divides LhsRest and N and
  //    does not divide M and RhsRest.
  //    Now: K | N <=> K | LhsNum * RhsRest + RhsNum * LhsRest <=> K | LhsNum * RhsRest <=> K | LhsNum
  //    So K is a common divisor of LhsNum and LhsDen (as result of dividing LhsRest). This is not possible since
  //    LhsNum and LhsDen are coprime in this representation.
  //    So K must divide M.
  //    So GCD(N, M * LhsRest * RhsRest) is less or equal GCD(N, M)
  // By A) and B) we get equality
  static const int64 M = Gcd<Lhs::Den, Rhs::Den>::value;
  static const int64 N = Add< Multiply<Lhs::Num, (Rhs::Den / M)>::value,
                              Multiply<Rhs::Num, (Lhs::Den / M)>::value >::value;

  // The new numerator may have common factors with the denominator, but they have to also be factors of GCD.
  static const int64 K = Gcd<N, M>::value;

 public:
  typedef Ratio<N / K, Multiply<Lhs::Den / K, Rhs::Den / M>::value> type;

  static const int64 Num = type::Num;
  static const int64 Den = type::Den;
};

template<typename Lhs, typename Rhs>
const int64 RatioAdd<Lhs, Rhs>::Num;

template<typename Lhs, typename Rhs>
const int64 RatioAdd<Lhs, Rhs>::Den;

/// Subtract
template<typename Lhs, typename Rhs>
struct RatioSubtract {
  typedef typename RatioAdd< Lhs, Ratio<-Rhs::Num, Rhs::Den> >::type type;

  static const int64 Num = type::Num;
  static const int64 Den = type::Den;
};

template<typename Lhs, typename Rhs>
const int64 RatioSubtract<Lhs, Rhs>::Num;

template<typename Lhs, typename Rhs>
const int64 RatioSubtract<Lhs, Rhs>::Den;


/// Multiply
template<typename Lhs, typename Rhs>
struct RatioMultiply {
 private:
  static const int64 LhsNumRhsDenGcd = Gcd<Lhs::Num, Rhs::Den>::value;
  static const int64 RhsNumLhsDenGcd = Gcd<Rhs::Num, Lhs::Den>::value;

 public:
  typedef Ratio< Multiply<(Lhs::Num / LhsNumRhsDenGcd), (Rhs::Num / RhsNumLhsDenGcd)>::value,
                 Multiply<(Lhs::Den / RhsNumLhsDenGcd), (Rhs::Den / LhsNumRhsDenGcd)>::value > type;

  static const int64 Num = type::Num;
  static const int64 Den = type::Den;
};

template<typename Lhs, typename Rhs>
const int64 RatioMultiply<Lhs, Rhs>::Num;

template<typename Lhs, typename Rhs>
const int64 RatioMultiply<Lhs, Rhs>::Den;

/// Divide
template<typename Dividend, typename Divisor>
struct RatioDivide {
  static_assert(Divisor::Num != 0, "Division by 0");

  typedef typename RatioMultiply< Dividend, Ratio<Divisor::Den, Divisor::Num> >::type type;

  static const int64 Num = type::Num;
  static const int64 Den = type::Den;
};

template<typename Dividend, typename Divisor>
const int64 RatioDivide<Dividend, Divisor>::Num;

template<typename Dividend, typename Divisor>
const int64 RatioDivide<Dividend, Divisor>::Den;

/// Equal
template<typename Lhs, typename Rhs>
struct RatioEqual {
  static const bool value = (Lhs::Num == Rhs::Num && Lhs::Den == Rhs::Den);
};

/// Not equal
template<typename Lhs, typename Rhs>
struct RatioNotEqual {
  static const bool value = !RatioEqual<Lhs, Rhs>::value;
};

// Comparisons
template<typename Lhs, typename Rhs>
struct RatioLess {
  static const bool value = Detail::RatioSignBasedCompare<Lhs, Rhs>::value;
};

template<typename Lhs, typename Rhs>
struct RatioLessEqual {
  static const bool value = !RatioLess<Rhs, Lhs>::value;
};

template<typename Lhs, typename Rhs>
struct RatioGreater {
  static const bool value = RatioLess<Rhs, Lhs>::value;
};

template<typename Lhs, typename Rhs>
struct RatioGreaterEqual {
  static const bool value = !RatioLess<Lhs, Rhs>::value;
};

}  // namespace Mpl
}  // namespace Utilities
}  // namespace Elg

#include "Elg/Utilities/Mpl/MplRatioTypedefs.h"

#endif  // ELG_UTILITIES_MPL_MPLRATIO_H_
