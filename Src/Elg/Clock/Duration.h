/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#ifndef ELG_CLOCK_DURATION_H_
#define ELG_CLOCK_DURATION_H_

#include <type_traits>
#include <limits>

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/Utilities/Mpl/MplGcd.h"
#include "Elg/Utilities/Mpl/MplRatio.h"

// Forward declaration of Duration class
namespace Elg {
namespace Clock {

template<typename Representation, typename Period = Elg::Utilities::Mpl::Ratio<1>>
class Duration;

}  // namespace Clock
}  // namespace Elg

// Specialization of common_type for Duration class
namespace std {

template<typename LhsRep, typename LhsPeriod, typename RhsRep, typename RhsPeriod>
struct common_type<Elg::Clock::Duration<LhsRep, LhsPeriod>, Elg::Clock::Duration<RhsRep, RhsPeriod>> {
 private:
  typedef Elg::Utilities::Mpl::Gcd<LhsPeriod::Num, RhsPeriod::Num> NumGcd;
  typedef Elg::Utilities::Mpl::Gcd<LhsPeriod::Den, RhsPeriod::Den> DenGcd;
  typedef typename common_type<LhsRep, RhsRep>::type CommonRepresentation;
  typedef Elg::Utilities::Mpl::Ratio<NumGcd::value, LhsPeriod::Den * (RhsPeriod::Den / DenGcd::value)> CommonRatio;

 public:
  typedef Elg::Clock::Duration<CommonRepresentation, CommonRatio> type;
};

}  // namespace std

// The Duration class and friends
namespace Elg {
namespace Clock {

// Detail helper classes
namespace Detail {

template<typename T>
struct IsDuration {
  static const bool value = false;
};

template<typename Repr, typename Period>
struct IsDuration<Duration<Repr, Period>> {
  static const bool value = true;
};

template<typename LhsRepr, typename RhsRepr,
         bool IsConvertible = std::is_convertible<RhsRepr, typename std::common_type<LhsRepr, RhsRepr>::type>::value>
struct HasCommonRepresentation;

template<typename LhsRepr, typename RhsRepr>
struct HasCommonRepresentation<LhsRepr, RhsRepr, true> {
  typedef typename std::common_type<LhsRepr, RhsRepr>::type type;
};

template<typename LhsRepr, typename RhsRepr>
struct HasCommonRepresentation<LhsRepr, RhsRepr, false> {
};

template<typename LhsRepr, typename RhsRepr, bool IsDuration = Detail::IsDuration<RhsRepr>::value>
struct HasCommonRepresentationAndIsNotADuration;

template<typename LhsRepr, typename RhsRepr>
struct HasCommonRepresentationAndIsNotADuration<LhsRepr, RhsRepr, false> {
  typedef typename HasCommonRepresentation<LhsRepr, RhsRepr>::type type;
};

template<typename LhsRepr, typename RhsRepr>
struct HasCommonRepresentationAndIsNotADuration<LhsRepr, RhsRepr, true> {
};

}  // namespace Detail

// Treat as a floating point
template<typename Repr>
struct TreatAsFloatingPoint : std::is_floating_point<Repr> {
};

// Duration values class
template<typename Repr>
struct DurationValues {
  static Repr Zero() {
    return Repr(0);
  }

  static Repr Max() {
    return std::numeric_limits<Repr>::Max();
  }

  static Repr Min() {
    return std::numeric_limits<Repr>::Min();
  }
};

// Arithmetic
template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline typename std::common_type< Duration<LhsRepr, LhsPeriod>, Duration<RhsRepr, RhsPeriod> >::type
operator+(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonDurationType;
  return CommonDurationType(lhs) += rhs;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline typename std::common_type< Duration<LhsRepr, LhsPeriod>, Duration<RhsRepr, RhsPeriod> >::type
operator-(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonDurationType;
  return CommonDurationType(lhs) -= rhs;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr>
inline Duration<typename Detail::HasCommonRepresentation<LhsRepr, RhsRepr>::type, LhsPeriod>
operator*(const Duration<LhsRepr, LhsPeriod>& d, const RhsRepr& s) {
  typedef typename std::common_type<LhsRepr, RhsRepr>::type CommonRepresentation;
  return Duration<CommonRepresentation, LhsPeriod>(d) *= s;
}

template<typename LhsRepr, typename RhsRepr, typename RhsPeriod>
inline Duration<typename Detail::HasCommonRepresentation<RhsRepr, LhsRepr>::type, RhsPeriod>
operator*(const LhsRepr& s, const Duration<RhsRepr, RhsPeriod>& d) {
  return d * s;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr>
inline Duration<typename Detail::HasCommonRepresentationAndIsNotADuration<LhsRepr, RhsRepr>::type, LhsPeriod>
operator/(const Duration<LhsRepr, LhsPeriod>& d, const RhsRepr& s) {
  typedef typename std::common_type<LhsRepr, RhsRepr>::type CommonRepresentation;
  return Duration<CommonRepresentation, LhsPeriod>(d) /= s;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline typename std::common_type<LhsRepr, RhsRepr>::type
operator/(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonDurationType;
  return CommonDurationType(lhs).Count() / CommonDurationType(rhs).Count();
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr>
inline Duration<typename Detail::HasCommonRepresentationAndIsNotADuration<LhsRepr, RhsRepr>::type, LhsPeriod>
operator%(const Duration<LhsRepr, LhsPeriod>& d, const RhsRepr& s) {
  typedef typename std::common_type<LhsRepr, RhsRepr>::type CommonRepresentation;
  return Duration<CommonRepresentation, LhsPeriod>(d) %= s;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline typename std::common_type< Duration<LhsRepr, LhsPeriod>, Duration<RhsRepr, RhsPeriod> >::type
operator%(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonDurationType;
  return CommonDurationType(lhs) %= rhs;
}

// Comparisons of durations
template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator==(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonType;
  return CommonType(lhs).Count() == CommonType(rhs).Count();
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator!=(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  return !(lhs == rhs);
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator<(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<LhsRepr, LhsPeriod> LhsDuration;
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonType;
  return CommonType(lhs).Count() < CommonType(rhs).Count();
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator<=(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  return !(rhs < lhs);
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator>(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  return rhs < lhs;
}

template<typename LhsRepr, typename LhsPeriod, typename RhsRepr, typename RhsPeriod>
inline bool operator>=(const Duration<LhsRepr, LhsPeriod>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  return !(lhs < rhs);
}

// The Duration class
template<typename R, typename P>
class Duration {
 public:
  // Typedefs
  typedef R Repr;
  typedef P Period;

  // Asserts
  STATIC_ASSERT(!Detail::IsDuration<Repr>::value, "Representation tyoe cannot be a duration");
  STATIC_ASSERT(Elg::Utilities::Mpl::Detail::IsRatio<Period>::value, "Period typw must be a specialization of ratio");
  STATIC_ASSERT(Period::Num > 0, "Period must be positive");

  // Constructions
  Duration() : m_r_() {
  }

  Duration(const Duration& other) : m_r_(other.m_r_) {
  }

  template<typename OtherRepr>
  explicit Duration(const OtherRepr& r,
                    typename std::enable_if< std::is_convertible<OtherRepr, Repr>::value &&
                                            (TreatAsFloatingPoint<Repr>::value ||
                                            !TreatAsFloatingPoint<OtherRepr>::value)>::type* /* dummy */ = 0)
    : m_r_(static_cast<Repr>(r)) {
  }

  template<typename OtherRep, typename OtherPeriod>
  Duration(const Duration<OtherRep, OtherPeriod>& d,
           typename std::enable_if< TreatAsFloatingPoint<Repr>::value ||
                                   (Elg::Utilities::Mpl::RatioDivide<OtherPeriod, Period>::type::Den == 1 &&
                                   !TreatAsFloatingPoint<OtherRep>::value)>::type* /* dummy */ = 0)
    : m_r_(DurationCast<Duration>(d).Count()) {
  }

  ~Duration() {
  }

  Duration& operator=(const Duration& other) {
    m_r_ = other.m_r_;
    return *this;
  }

  Repr Count() const {
    return m_r_;
  }

  Duration operator+() const {
    return *this;
  }

  Duration operator-() const {
    return Duration(-m_r_);
  }

  Duration& operator++() {
    ++m_r_;
    return *this;
  }

  Duration operator++(int /* dummy */) {
    return Duration(m_r_++);
  }

  Duration& operator--() {
    --m_r_;
    return *this;
  }

  Duration operator--(int /* dummy */) {
    return Duration(m_r_--);
  }

  Duration& operator+=(const Duration& d) {
    m_r_ += d.Count();
    return *this;
  }

  Duration& operator-=(const Duration& d) {
    m_r_ -= d.Count();
    return *this;
  }

  Duration& operator*=(const Repr& rhs) {
    m_r_ *= rhs;
    return *this;
  }

  Duration& operator/=(const Repr& rhs) {
    m_r_ /= rhs;
    return *this;
  }

  Duration& operator%=(const Repr& rhs) {
    m_r_ %= rhs;
    return *this;
  }

  Duration& operator%=(const Duration& d) {
    m_r_ %= d.Count();
    return *this;
  }

  static Duration Zero() {
    return Duration(DurationValues<Repr>::Zero());
  }

  static Duration Min() {
    return Duration(DurationValues<Repr>::Min());
  }

  static Duration Max() {
    return Duration(DurationValues<Repr>::Max());
  }

 private:
  Repr m_r_;
};

// The DurationCast class
template<typename Dest, typename Repr, typename Period>
inline typename std::enable_if<Detail::IsDuration<Dest>::value, Dest>::type
DurationCast(const Duration<Repr, Period>& d) {
  typedef typename Dest::Period DestPeriod;
  typedef typename Dest::Repr DestRepr;
  typedef Elg::Utilities::Mpl::RatioDivide<Period, DestPeriod> RatioDivivided;
  typedef typename RatioDivivided::type CommonRatio;
  typedef typename std::common_type<DestRepr, Repr>::type CommonRepr;

  return Dest(static_cast<DestRepr>(static_cast<CommonRepr>(d.Count()) *
              static_cast<CommonRepr>(CommonRatio::Num) / static_cast<CommonRepr>(CommonRatio::Den)));
}

}  // namespace Clock
}  // namespace Elg

#include "Elg/Clock/DurationTypedefs.h"

#endif  // ELG_CLOCK_DURATION_H_
