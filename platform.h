#include "rect.h"

typedef enum {
    Normal,
    Spike,
} PlatformType;

typedef struct {
    Rect rect;
    PlatformType type;
} Platform;