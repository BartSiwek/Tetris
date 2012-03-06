/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ALLOCATORS_MALLOCALLOCATOR_H_
#define ELG_ALLOCATORS_MALLOCALLOCATOR_H_

#include "Elg/Utilities/Types.h"

namespace Elg {
namespace Allocators {

class MallocAllocator {
 public:
  void* Malloc(uint32 size);
  void  Free(void* ptr);
};

}  // namespace Allocators
}  // namespace Elg

#endif  // ELG_ALLOCATORS_MALLOCALLOCATOR_H_
