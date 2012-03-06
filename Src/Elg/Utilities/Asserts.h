/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_ASSERTS_H_
#define ELG_UTILITIES_ASSERTS_H_

#include <cassert>

#include "Elg/Utilities/Macros.h"
#include "Elg/Utilities/Assert/Assert.h"
#include "Elg/Utilities/Assert/StaticAssert.h"

// Runtime assertion
#ifdef ENABLE_ASSERTS
#define ASSERT(condition, format, ...) STATIC_ANALYSIS_ASSUME(condition);                                             \
                                       (condition) ? (void)true : (Elg::Utilities::PrintAssert(__FILE__, __LINE__,    \
                                                                     "Assertion [" #condition "] failed. " format,    \
                                                                     __VA_ARGS__), DEBUGBREAK)
#define VERIFY_TRUE(condition, format, ...) ASSERT(condition, format, __VA_ARGS__)
#define VERIFY_FALSE(condition, format, ...) ASSERT(!(condition), format, __VA_ARGS__)
#else
#define ASSERT(condition, format, ...) STATIC_ANALYSIS_ASSUME(condition)
#define VERIFY_TRUE(condition, format, ...) (condition); STATIC_ANALYSIS_ASSUME(condition)
#define VERIFY_FALSE(condition, format, ...) (condition); STATIC_ANALYSIS_ASSUME(condition)
#endif

#endif  // ELG_UTILITIES_ASSERTS_H_
