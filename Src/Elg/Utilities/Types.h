/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_TYPES_H_
#define ELG_UTILITIES_TYPES_H_

#include <cstdint>

#include "Elg/Utilities/Assert/StaticAssert.h"

// Define types + make compile time check
typedef std::uint8_t uint8;
STATIC_ASSERT(sizeof(uint8) == 1, "uint8 is not 8bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::uint16_t uint16;
STATIC_ASSERT(sizeof(uint16) == 2, "uint16 is not 16bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::uint32_t uint32;
STATIC_ASSERT(sizeof(uint32) == 4, "uint32 is not 32bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::uint64_t uint64;
STATIC_ASSERT(sizeof(uint64) == 8, "uint64 is not 64bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::int8_t int8;
STATIC_ASSERT(sizeof(int8) == 1, "int8 is not 8bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::int16_t int16;
STATIC_ASSERT(sizeof(int16) == 2, "int16 is not 16bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::int32_t int32;
STATIC_ASSERT(sizeof(int32) == 4, "int32 is not 32bits long on this system");  // NOLINT(runtime/sizeof)

typedef std::int64_t int64;
STATIC_ASSERT(sizeof(int64) == 8, "int64 is not 64bits long on this system");  // NOLINT(runtime/sizeof)

typedef float float32;
STATIC_ASSERT(sizeof(float32) == 4, "float32 is not 32bits long on this system");  // NOLINT(runtime/sizeof)

typedef double float64;
STATIC_ASSERT(sizeof(float64) == 8, "float64 is not 64bits long on this system");  // NOLINT(runtime/sizeof)

typedef uint8* uint8ptr;

typedef int8* int8ptr;

#endif  // ELG_UTILITIES_TYPES_H_

