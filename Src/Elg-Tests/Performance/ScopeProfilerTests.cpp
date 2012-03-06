/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include "Elg/Performance/ScopeProfiler.h"

uint32 CommonGrandchild() {
  ELG_PROFILE_SCOPE;
  static uint32 x = 0;
  return ++x;
}

uint32 LeftChild() {
  ELG_PROFILE_SCOPE;
  uint32 result = CommonGrandchild() - CommonGrandchild();
  return result;
}

uint32 RightChild() {
  ELG_PROFILE_SCOPE;
  uint32 result = CommonGrandchild() * CommonGrandchild();
  return result;
}

uint32 Root() {
  ELG_PROFILE_SCOPE;
  uint32 result = LeftChild() + RightChild();
  return result;
}

TEST(ScopeProfilerHierarchy) {
  Root();
  Elg::Performance::Scope::DumpStatistics(stdout);
}

int Recurse(int x) {
  ELG_PROFILE_SCOPE;
  if (x >= 10) {
    return x;
  }
  return x * Recurse(x+1);
}

TEST(ScopeProfilerRecursive) {
  Recurse(1);
  Elg::Performance::Scope::DumpStatistics(stdout);
}
