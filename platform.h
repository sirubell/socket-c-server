#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "rect.h"

typedef enum {
    Normal,
    Spike,
} PlatformType;

typedef struct {
    Rect rect;
    PlatformType type;
} Platform;

#endif