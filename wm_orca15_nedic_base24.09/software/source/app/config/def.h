#ifndef DEF_H
#define DEF_H

#include "stdint.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define NO_RETURN __attribute__((noreturn))

typedef enum
{
    RET_OK = 0,
    RET_NOK = -1
} ret_t;

typedef enum
{
    FALSE = 0,
    TRUE = 1
} bool_t;

#endif
