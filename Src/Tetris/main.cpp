/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <SDL/SDL.h>

#include <cstdio>

#include <exception>

#include "Elg/Utilities/Types.h"
#include "Elg/Performance/ScopeProfiler.h"
#include "Elg/Game/Game.h"

#include "Tetris/TetrisGame.h"

int32 main(int32 argc, char** argv) {
  // Run! Run like the wind...
  int result;
  try {
    ELG_PROFILE_NAMED_SCOPE("Main");
    Elg::Game::Game<TetrisGame> tetris(argc, argv);
    result = tetris.Run();
  } catch(const std::exception &e) {
    fprintf(stderr, e.what());
    result = 1;
  }

  ELG_PROFILER_DUMP(stderr);
  system("PAUSE");
  return result;
}
