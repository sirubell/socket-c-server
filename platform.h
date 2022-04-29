#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "rect.h"
#include "str.h"

typedef enum {
    Normal,
    Spike,
} PlatformType;

typedef struct {
    Rect rect;
    Str name;
    PlatformType type;
} Platform;

typedef struct NodePlatform NodePlatform;
struct NodePlatform{
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
};

typedef struct {
    NodePlatform* head;
} LinkedListPlatform;

void create_platform(LinkedListPlatform* ll_platform, Platform platform);
void delete_platform(LinkedListPlatform* ll_platform, NodePlatform* node);

Rect platform_random_rect(void);

Str to_str_platform(Platform* platform);

#endif