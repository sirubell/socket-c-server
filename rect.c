#include "rect.h"

bool rect_collision(Rect* a, Rect* b) 
{
    if (a->x + a->w > b->x && a->x < b->x + b->w && a->y + a->h > b->y && a->y < b->y + b->h) {
        return true;
    }
    return false;
}

void calculate_collision(Rect* a, Rect* b)
{
    if (rect_collision(a, b) && a->y < b->y && a->y + a->h > b->y && a->y + a->h < b->y + b->h) a->y = b->y - a->h;
    if (rect_collision(a, b) && a->y > b->y && a->y < b->y + b->h) a->y = b->y + b->h;
    if (rect_collision(a, b) && a->x < b->x && a->x + a->w > b->x) a->x = b->x - a->w;
    if (rect_collision(a, b) && a->x > b->x && a->x < b->x + b->w) a->x = b->x + b->w;
}

bool rect_is_on(Rect* a, Rect* b)
{
    if (!(a->x + a->w < b->x || a->x > b->x + b->w) && a->y + a->h == b->y) {
        return true;
    }
    return false;
}