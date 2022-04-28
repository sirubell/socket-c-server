#include "player.h"

typedef struct {
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
} NodePlayer*;

typedef struct {
    NodePlayer* head;
} LinkedListPlayer;

NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd);

#ifdef _LINKEDLIST_PLAYER_H_IMPLEMENTATION_

NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd)
{
    if (ll_player == NULL)
        return NULL;

    LinkedListPlayer* tmp = ll_player->head;
    while (tmp != NULL) {
        if (tmp->fd == fd) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

#endif