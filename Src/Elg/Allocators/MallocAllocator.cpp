/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Elg/Allocators/MallocAllocator.h"

#include <cstdlib>
#include "Elg/Utilities/Types.h"

namespace Elg {
namespace Allocators {

void* MallocAllocator::Malloc(uint32 size) {
  return std::malloc(size);
}

void  MallocAllocator::Free(void* ptr) {
  std::free(ptr);
}

}  // namespace Allocators
}  // namespace Elg
