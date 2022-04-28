#ifndef _LINKEDLIST_PLATFORM_H_
#define _LINKEDLIST_PLATFORM_H_

#include "platform.h"

typedef struct NodePlatform NodePlatform;
struct NodePlatform{
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
};

typedef struct LinkedListPlatform LinkedListPlatform;
struct LinkedListPlatform{
    NodePlatform* head;
};


#endif