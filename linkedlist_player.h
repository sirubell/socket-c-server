#ifndef _LINKEDLIST_PLAYER_H_
#define _LINKEDLIST_PLAYER_H_

#include "player.h"

typedef struct {
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
} NodePlayer;

typedef struct {
    NodePlayer* head;
} LinkedListPlayer;

NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd);

#endif