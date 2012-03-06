/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ALLOCATORS_GLOBALALLOCATOR_H_
#define ELG_ALLOCATORS_GLOBALALLOCATOR_H_

#include <string>
#include "Elg/Utilities/Types.h"

// Malloc-Free
void* Malloc(uint32 size, const std::string &file, uint32 line);
void  Free(void* ptr);

// New operators
void *operator new[](uint32 size, const std::string &file, uint32 line) throw();
void *operator new(uint32 size, const std::string &file, uint32 line) throw();

// Delete operators
void operator delete[](void* ptr, const std::string &file, uint32 line) throw();
void operator delete(void* ptr, const std::string &file, uint32 line) throw();

// Macros
#define ElgMalloc(SIZE) Malloc(SIZE, __FILE__, __LINE__)
#define ElgNew new(__FILE__, __LINE__)
#define ElgDelete delete

#endif  // ELG_ALLOCATORS_GLOBALALLOCATOR_H_
