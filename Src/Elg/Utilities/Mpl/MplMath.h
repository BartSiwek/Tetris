/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_MPL_MPLMATH_H_
#define ELG_UTILITIES_MPL_MPLMATH_H_

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"

namespace Elg {
namespace Utilities {
namespace Mpl {

// Sign
template<int64 Value>
struct Sign {
  static const int64 value = (Value < 0) ? (-1) : (Value > 0 ? 1 : 0);
};

// Abs
template<int64 Value>
struct Abs {
  static const int64  value = Value * Sign<Value>::value;
};

// Multiply - uses http://www.fefe.de/intof.html
template<int64 Lhs, int64 Rhs>
struct Multiply {
 private:
  static const uint64 MAX = static_cast<uint64>(0x7fffffffffffffff);

  static const uint64 c = static_cast<uint64>(1) << 32;  // Half of 64

  static const uint64 LhsLow = Abs<Lhs>::value % c;
  static const uint64 LhsHigh = Abs<Lhs>::value / c;
  static const uint64 RhsLow = Abs<Rhs>::value % c;
  static const uint64 RhsHigh = Abs<Rhs>::value / c;

  static const uint64 a = LhsLow * RhsHigh + RhsLow * LhsHigh;

  STATIC_ASSERT(LhsHigh == 0 || RhsHigh == 0, "Overflow in MPL Multiply");
  STATIC_ASSERT(a < (c >> 1), "Overflow in MPL Multiply");  // Account for the sign bit
  STATIC_ASSERT(RhsLow * LhsLow <= MAX, "Overflow in MPL Multiply");  // This is just to be safe
  STATIC_ASSERT(a * c <= MAX -  RhsLow * LhsLow, "Overflow in MPL Multiply");

 public:
  static const int64 value = Lhs * Rhs;
};

// Add
template<int64 Lhs, int64 Rhs, bool>
struct AddCheckedImplementation;

template<int64 Lhs, int64 Rhs>
struct AddCheckedImplementation<Lhs, Rhs, true> {
  static const int64 MAX = static_cast<int64>(0x7fffffffffffffff);

  enum { value = (Lhs <= MAX - Rhs) };
};

template<int64 Lhs, int64 Rhs>
struct AddCheckedImplementation<Lhs, Rhs, false> {
  static const int64 MIN = -static_cast<int64>(0x8000000000000000);

  enum { value = (Lhs >= MIN - Rhs) };
};

template<int64 Lhs, int64 Rhs>
struct AddChecked : AddCheckedImplementation<Lhs, Rhs, (Rhs >= 0)> {
};

template<int64 Lhs, int64 Rhs>
struct Add {
  static_assert(AddChecked<Lhs, Rhs>::value != 0, "Overflow in Add");

  static const int64 value = Lhs + Rhs;
};

}  // namespace Mpl
}  // namespace Utilities
}  // namespace Elg

#endif  // ELG_UTILITIES_MPL_MPLMATH_H_
