/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Elg/Utilities/Assert/Assert.h"

#include <cstdarg>
#include <cstring>
#include <cstdio>

#include "Elg/Utilities/Types.h"

namespace Elg {
namespace Utilities {

void PrintAssert(const char* file, uint32 line, const char* format, ...) {
  va_list argptr;
  va_start(argptr, format);
  fprintf(stderr, "%s(%d): ", file, line);
  vfprintf(stderr, format, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);
}

}  // namespace Utilities
}  // namespace Elg
