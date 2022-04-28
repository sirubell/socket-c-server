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

typedef struct NodePlatform NodePlatform;
struct NodePlatform{
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
};

struct LinkedListPlatform{
    NodePlatform* head;
} LinkedListPlatform;

#endif