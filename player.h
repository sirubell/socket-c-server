#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "rect.h"
#include "str.h"

typedef struct {
    Rect rect;
    float heart;
    Str name;
    int fd;
} Player;

#endif