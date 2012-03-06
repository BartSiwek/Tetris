/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_STRINGHASH_X65599HASH_H_
#define ELG_UTILITIES_STRINGHASH_X65599HASH_H_

#include <cstring>

#include "Elg/Utilities/Macros.h"
#include "Elg/Utilities/Types.h"

namespace Elg {
namespace Utilities {
namespace StringHash {

FORCEINLINE uint32 X65599Hash(const char* s) {
  const uint32 length = std::strlen(s) + 1;
  uint32 hash = 0;

  for (uint32 index = 0; index < length; ++index) {
    hash = 65599 * hash + s[index];
  }

  return hash ^ (hash >> 16);
}

}  // namespace StringHash
}  // namespace Utilities
}  // namespace Elg

#endif  // ELG_UTILITIES_STRINGHASH_X65599HASH_H_
