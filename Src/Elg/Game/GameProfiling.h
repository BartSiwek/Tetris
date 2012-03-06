/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_GAME_GAMEPROFILING_H_
#define ELG_GAME_GAMEPROFILING_H_

#include "Elg/Performance/ScopeProfiler.h"

#define ELG_PROFILE_GAME

#ifdef ELG_PROFILE_GAME
#define ELG_PROFILE_GAME_LOOP ELG_PROFILE_NAMED_SCOPE("GameLoop")
#else  // ELG_PROFILE_GAME
#define ELG_PROFILE_GAME_LOOP
#endif  // ELG_PROFILE_GAME

#endif  // ELG_GAME_GAMEPROFILING_H_
