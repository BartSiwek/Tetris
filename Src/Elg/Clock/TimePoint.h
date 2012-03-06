/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#ifndef ELG_CLOCK_TIMEPOINT_H_
#define ELG_CLOCK_TIMEPOINT_H_

#include <type_traits>
#include <limits>

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/Clock/Duration.h"

namespace Elg {
namespace Clock {

// Time point class
template<typename ClockType, typename DurationType>
class TimePoint {
 public:
  typedef ClockType Clock;
  typedef DurationType Duration;
  typedef typename DurationType::Repr Repr;
  typedef typename DurationType::Period Period;

  TimePoint() : m_d_(DurationType::Zero()) {
  }

  explicit TimePoint(const DurationType& dur)
    : m_d_(dur) {
  }

  template<typename OtherDurationType>
  TimePoint(const TimePoint<ClockType, OtherDurationType>& t)
    : m_d_(t.TimeSinceEpoch()) {
  }

  DurationType TimeSinceEpoch() const {
    return m_d_;
  }

  TimePoint& operator+=(const DurationType& dur) {
    m_d_ += dur;
    return *this;
  }

  TimePoint& operator-=(const DurationType& dur) {
    m_d_ -= dur;
    return *this;
  }

  static TimePoint Epoch() {
    return TimePoint(DurationType::Zero());
  }

  static TimePoint Min() {
    return TimePoint(DurationType::Min());
  }

  static TimePoint Max() {
    return TimePoint(DurationType::Max());
  }

 private:
  DurationType m_d_;
};

// TimePoint arithmetics
template<typename ClockType, typename LhsDuration, typename RhsRepr, typename RhsPeriod>
inline TimePoint<ClockType, typename std::common_type< LhsDuration, Duration<RhsRepr, RhsPeriod> >::type>
operator+(const TimePoint<ClockType, LhsDuration>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  typedef Duration<RhsRepr, RhsPeriod> RhsDuration;
  typedef typename std::common_type<LhsDuration, RhsDuration>::type CommonDurationType;
  typedef TimePoint<ClockType, CommonDurationType> DestTimePointType;
  return DestTimePointType(lhs) += rhs;
}

template<typename LhsRepr, typename LhsPeriod, typename ClockType, typename RhsDuration>
inline TimePoint<ClockType, typename std::common_type<Duration<LhsRepr, LhsPeriod>, RhsDuration>::type>
operator+(const Duration<LhsRepr, LhsPeriod>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return rhs + lhs;
}

template<typename ClockType, typename LhsDuration, typename RhsRepr, typename RhsPeriod>
inline TimePoint<ClockType, typename std::common_type< LhsDuration, Duration<RhsRepr, RhsPeriod> >::type>
operator-(const TimePoint<ClockType, LhsDuration>& lhs, const Duration<RhsRepr, RhsPeriod>& rhs) {
  return lhs + (-rhs);
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline typename std::common_type<LhsDuration, RhsDuration>::type
operator-(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return lhs.TimeSinceEpoch() - rhs.TimeSinceEpoch();
}

// TimePoint comparisons
template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator==(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return lhs.TimeSinceEpoch() == rhs.TimeSinceEpoch();
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator!=(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return !(lhs == rhs);
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator<(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return  lhs.TimeSinceEpoch() < rhs.TimeSinceEpoch();
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator<=(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return !(rhs < lhs);
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator>(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return rhs < lhs;
}

template<typename ClockType, typename LhsDuration, typename RhsDuration>
inline bool operator>=(const TimePoint<ClockType, LhsDuration>& lhs, const TimePoint<ClockType, RhsDuration>& rhs) {
  return !(lhs < rhs);
}

// TimePoint casting
template<typename Dest, typename ClockType, typename Dur>
inline typename std::enable_if< Detail::IsDuration<Dest>::value, TimePoint<ClockType, Dest> >::type
TimePointCast(const TimePoint<ClockType, Dur>& t) {
  typedef TimePoint<ClockType, Dest> DestTimePoint;
  return DestTimePoint(DurationCast<Dest>(t.TimeSinceEpoch()));
}

}  // namespace Elg
}  // namespace Clock

#endif  // ELG_CLOCK_TIMEPOINT_H_
