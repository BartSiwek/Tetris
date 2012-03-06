/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ENTITYMANAGER_IDMACROS_H_
#define ELG_ENTITYMANAGER_IDMACROS_H_

#include "Elg/Utilities/Types.h"

#define ELG_ENTITY_MANAGER_WRAP_ID(ID, GENERATION)                                                                    \
(((ID) << 16) | (GENERATION))

// We use 15 bits for valid generation values (make is so that it overflows nicely)
#define ELG_ENTITY_MANAGER_INC_GENERATION(GENERATION)                                                                  \
(((GENERATION) + 0x00000001) & 0x00007FFF)

#define ELG_ENTITY_MANAGER_UNWRAP_ID(WRAPPED_ID)                                                                      \
(((WRAPPED_ID) & 0xFFFF0000) >> 16)

#define ELG_ENTITY_MANAGER_UNWRAP_GENERATION(WRAPPED_ID)                                                              \
((WRAPPED_ID) & 0x0000FFFF)

// We use 15 bits for valid generation values, so 0xFFFF is an invalid value
#define ELG_ENTITY_MANAGER_INVALID_ID static_cast<uint32>(0xFFFF)

// We use 16 bits for valid id values, however we reserve the max one as invalid marker
#define ELG_ENTITY_MANAGER_INVALID_GENERATION static_cast<uint32>(0xFFFF)

// Composition of the two above
#define ELG_ENTITY_MANAGER_INVALID_WRAPPED_ID static_cast<uint32>(0xFFFFFFFF)

#endif  // ELG_ENTITYMANAGER_IDMACROS_H_
