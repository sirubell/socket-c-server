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

typedef struct NodePlayer NodePlayer;
struct NodePlayer{
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
};

typedef struct LinkedListPlayer LinkedListPlayer;
struct LinkedListPlayer{
    NodePlayer* head;
};

NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd);

#endif