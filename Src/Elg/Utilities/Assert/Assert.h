/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_ASSERT_ASSERT_H_
#define ELG_UTILITIES_ASSERT_ASSERT_H_

#include "Elg/Utilities/Types.h"

namespace Elg {
namespace Utilities {

void PrintAssert(const char* file, uint32 line, const char* format, ...);

}  // namespace Utilities
}  // namespace Elg

#endif  // ELG_UTILITIES_ASSERT_ASSERT_H_
