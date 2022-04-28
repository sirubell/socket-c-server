#include "linkedlist_player.h"

NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd)
{
    if (ll_player == NULL)
        return NULL;

    NodePlayer* tmp = ll_player->head;
    while (tmp != NULL) {
        if (tmp->p.fd == fd) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}