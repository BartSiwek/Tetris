/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_MPL_MPLRATIOTYPEDEFS_H_
#define ELG_UTILITIES_MPL_MPLRATIOTYPEDEFS_H_

namespace Elg {
namespace Utilities {
namespace Mpl {

template<int64 N, int64 D> struct Ratio;

typedef Ratio<1,       1000000000000000000> Atto;
typedef Ratio<1,          1000000000000000> Femto;
typedef Ratio<1,             1000000000000> Pico;
typedef Ratio<1,                1000000000> Nano;
typedef Ratio<1,                   1000000> Micro;
typedef Ratio<1,                      1000> Milli;
typedef Ratio<1,                       100> Centi;
typedef Ratio<1,                        10> Deci;
typedef Ratio<                       10, 1> Deca;
typedef Ratio<                      100, 1> Hecto;
typedef Ratio<                     1000, 1> Kilo;
typedef Ratio<                  1000000, 1> Mega;
typedef Ratio<               1000000000, 1> Giga;
typedef Ratio<            1000000000000, 1> Tera;
typedef Ratio<         1000000000000000, 1> Peta;
typedef Ratio<      1000000000000000000, 1> Exa;

}  // namespace Mpl
}  // namespace Utilities
}  // namespace Elg

#endif  // ELG_UTILITIES_MPL_MPLRATIOTYPEDEFS_H_
