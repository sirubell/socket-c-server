#include <stdlib.h>

#include "platform.h"

void create_platform(LinkedListPlatform* ll_platform, Platform platform)
{
    NodePlatform* node = (NodePlatform*)malloc(sizeof(NodePlatform));
    node->p = platform;

    node->next = ll_platform->head;
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