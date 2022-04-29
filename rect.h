#ifndef _RECT_H_
#define _RECT_H_

#include <stdbool.h>

typedef struct {
    float x, y, w, h;
} Rect;

bool rect_collision(Rect* a, Rect* b);
void calculate_collision(Rect* a, Rect* b);
bool rect_is_on(Rect* a, Rect* b);

#endif