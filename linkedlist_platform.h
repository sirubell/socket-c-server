#include "platform.h"

typedef struct {
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
} NodePlatform;

typedef struct {
    NodePlatform* head;
} LinkedListPlatform;