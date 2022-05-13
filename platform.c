#include <stdlib.h>

#include "platform.h"

void create_platform(LinkedListPlatform *ll_platform, Platform platform) {
  NodePlatform *node = (NodePlatform *)malloc(sizeof(NodePlatform));
  node->p = platform;

  node->next = ll_platform->head;
  node->prev = NULL;
  if (ll_platform->head != NULL)
    ll_platform->head->prev = node;
  ll_platform->head = node;
}

void delete_platform(LinkedListPlatform *ll_platform, NodePlatform *node) {
  if (node == NULL)
    return;

  if (ll_platform->head == node)
    ll_platform->head = node->next;

  NodePlatform *front = node->prev;
  NodePlatform *back = node->next;
  if (front)
    front->next = back;
  if (back)
    back->prev = front;

  free(node);
}

Rect platform_random_rect(void) {
  int w = 100 + rand() % 150;
  int h = 20;
  int x = 100 + rand() % 400;
  int y = 900;

  return (Rect){
      .x = x - w / 2,
      .y = y,
      .w = w,
      .h = h,
  };
}

Str to_str_platform(Platform *platform) {
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
