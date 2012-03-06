/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_STRINGHASH_X65599HASHPREPROCESSOR_H_
#define ELG_UTILITIES_STRINGHASH_X65599HASHPREPROCESSOR_H_

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/punctuation.hpp>

#include "Elg/Utilities/Types.h"

// index = 0 -> hash = string[0]
// index = 1 -> hash = 65599 * (string[0]) + string[1]
// index = 2 -> hash = 65599 * (65599 * (string[0]) + string[1]) + string[2]
// index = 3 -> hash = 65599 * (65599 * (65599 * (string[0]) + string[1]) + string[2]) + string[3]
// index = 3 -> hash = 65599 * (65599 * (65599 * (65599 * (string[0]) + string[1]) + string[2]) + string[3]) + string[4]
//                                                ^^^^^^^^^         ^^^^^^^^^^^^^
//                                                PREFIX            POSTFIX

#define ELG_X65599_PREFIX(Z, N, DATA)                                                                                 \
  65599 * BOOST_PP_LPAREN()
#define ELG_X65599_POSTFIX(Z, N, DATA)                                                                                \
  BOOST_PP_RPAREN() + DATA[BOOST_PP_ADD(N, 1)]
#define ELG_X65599_REPEAT_PREFIX(VAR, N)                                                                              \
  BOOST_PP_REPEAT(N, ELG_X65599_PREFIX, DONT_CARE)
#define ELG_X65599_REPEAT_POSTFIX(VAR, N)                                                                             \
  BOOST_PP_REPEAT(N, ELG_X65599_POSTFIX, VAR)
#define ELG_X65599_PREHASH(VAR, N)                                                                                    \
  static_cast<uint32>(ELG_X65599_REPEAT_PREFIX(VAR, N) VAR[0] ELG_X65599_REPEAT_POSTFIX(VAR, N))
#define ELG_X65599(VAR, N)                                                                                            \
  ELG_X65599_PREHASH(VAR, BOOST_PP_SUB(N, 1)) ^ (ELG_X65599_PREHASH(VAR, BOOST_PP_SUB(N, 1)) >> 16)

#endif  // ELG_UTILITIES_STRINGHASH_X65599HASHPREPROCESSOR_H_
