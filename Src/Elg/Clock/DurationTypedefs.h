/*
* Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
*/

#ifndef ELG_CLOCK_DURATIONTYPEDEFS_H_
#define ELG_CLOCK_DURATIONTYPEDEFS_H_

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Mpl/MplRatioTypedefs.h"

namespace Elg {
namespace Clock {

template<typename Representation, typename Period> class Duration;

typedef int64 NanosecondsRepr;
typedef Elg::Utilities::Mpl::Nano NanosecondsPeriod;
typedef Duration<NanosecondsRepr, NanosecondsPeriod> Nanoseconds;

typedef int64 MicrosecondsRepr;
typedef Elg::Utilities::Mpl::Micro MicrosecondsPeriod;
typedef Duration<MicrosecondsRepr, MicrosecondsPeriod> Microseconds;

typedef int64 MillisecondsRepr;
typedef Elg::Utilities::Mpl::Milli MillisecondsPeriod;
typedef Duration<MillisecondsRepr, MillisecondsPeriod> Milliseconds;

typedef int64 SecondsRepr;
typedef Elg::Utilities::Mpl::Ratio<1, 1> SecondsPeriod;
typedef Duration<SecondsRepr, SecondsPeriod> Seconds;

typedef int32 MinutesRepr;
typedef Elg::Utilities::Mpl::Ratio<60, 1> MinutesPeriod;
typedef Duration<MinutesRepr, MinutesPeriod> Minutes;

typedef int32 HoursRepr;
typedef Elg::Utilities::Mpl::Ratio<3600, 1> HoursPeriod;
typedef Duration<HoursRepr, HoursPeriod> Hours;

}  // namespace Clock
}  // namespace Elg

#endif  // ELG_CLOCK_DURATIONTYPEDEFS_H_
