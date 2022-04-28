#ifndef _LINKEDLIST_PLAYER_H_
#define _LINKEDLIST_PLAYER_H_

#include "player.h"

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