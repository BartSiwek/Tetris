/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_MACROS_H_
#define ELG_UTILITIES_MACROS_H_

// Force inline macro
#if (__GNUC__ && __cplusplus)
#define FORCEINLINE __attribute__((always_inline))
#elif(_MSC_VER >= 1400)
#define FORCEINLINE __forceinline
#else
#error "Cannot define FORCEINLINE macro"
#endif

// Simple function name macro
#if (__GNUC__ && __cplusplus)
#define FUNC __FUNC__
#elif(_MSC_VER >= 1400)
#define FUNC __FUNCTION__
#else
#error "Cannot define FUNCNAME macro"
#endif

// Function name macro
#if (__GNUC__ && __cplusplus)
#define FUNCNAME __PRETTY_FUNCTION__
#elif(_MSC_VER >= 1400)
#define FUNCNAME __FUNCSIG__
#else
#error "Cannot define FUNCNAME macro"
#endif

// Debug breakpoint macro
#if (_MSC_VER >= 1310)
#include <intrin.h>
#include <windows.h>
#include <cstdlib>
#define DEBUGBREAK (IsDebuggerPresent() ? __debugbreak() : std::abort() )
#else
#define DEBUGBREAK abort()
#endif

// Thread local storage macro
#if (__GNUC__ && __cplusplus)
#define THREAD_LOCAL __thread
#elif(_MSC_VER >= 1310)
#define THREAD_LOCAL __declspec(thread)
#else
#error "Cannot define THREAD_LOCAL macro"
#endif

// Static code analysis assumptions
#if (_MSC_VER >= 1600)
#define STATIC_ANALYSIS_ASSUME(condition) __analysis_assume(!!(condition))
#else
#define STATIC_ANALYSIS_ASSUME(condition)
#endif

#endif  // ELG_UTILITIES_MACROS_H_
