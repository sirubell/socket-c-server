#ifndef _LINKEDLIST_PLATFORM_H_
#define _LINKEDLIST_PLATFORM_H_

#include "platform.h"

typedef struct {
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
} NodePlatform;

typedef struct {
    NodePlatform* head;
} LinkedListPlatform;

#endif