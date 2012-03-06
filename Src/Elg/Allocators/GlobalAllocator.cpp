/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Elg/Allocators/GlobalAllocator.h"

#include <string>
#include <cstdio>

#include "Elg/Utilities/Types.h"
#include "Elg/Allocators/MallocAllocator.h"

typedef Elg::Allocators::MallocAllocator GlobalAllocatorType;

GlobalAllocatorType GlobalAllocator;

void* Malloc(uint32 size, const std::string &/* file */, uint32 /* line */) {
  return GlobalAllocator.Malloc(size);
}

void Free(void* ptr) {
  GlobalAllocator.Free(ptr);
}

// New operators
void *operator new[](uint32 size, const std::string &/* file */, uint32 /* line */) throw() {
  return GlobalAllocator.Malloc(size);
}

void *operator new(uint32 size, const std::string &/* file */, uint32 /* line */) throw() {
  return GlobalAllocator.Malloc(size);
}

// Delete operators
void operator delete[](void* ptr, const std::string &/* file */, uint32 /* line */) throw () {
  GlobalAllocator.Free(ptr);
}

void operator delete(void* ptr, const std::string &/* file */, uint32 /* line */) throw () {
  GlobalAllocator.Free(ptr);
}
