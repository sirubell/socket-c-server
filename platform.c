#include <stdlib.h>

#include "platform.h"

void create_platform(LinkedListPlatform* ll_platform, Platform platform)
{
    NodePlatform* node = (NodePlatform*)malloc(sizeof(NodePlatform));
    node->p = platform;

    node->next = ll_platform->head;
    node->prev = NULL;
    if (ll_platform->head != NULL)
        ll_platform->head->prev = node;
    ll_platform->head = node;
}

void delete_platform(LinkedListPlatform* ll_platform, NodePlatform* node)
{
    if (node == NULL) return;

    NodePlatform *front = node->prev;
    NodePlatform *back = node->next;
    if (front) front->next = back;
    if (back) back->prev = front;

    free(node);
}

Rect platform_random_rect(void)
{
    return (Rect) {
        .x = 300,
        .y = 900,
        .w = 100,
        .h = 20,
    };
}

Str to_str_platform(Platform* platform)
{
    Str str, tmp;
    str_init(&str);

    tmp = to_str_int((int)platform->rect.x);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)platform->rect.y);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)platform->rect.w);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)platform->rect.h);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    str_cat(&str, &platform->name);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)platform->type + 1);
    str_cat(&str, &tmp);

    return str;
}