#include "player.h"

Dir get_dir(char c)
{
    if (c == '1') return Left;
    if (c == '2') return Right;
    return NoDir;
}

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

void create_player(LinkedListPlayer* ll_player, Player player)
{
    NodePlayer* node = (NodePlayer*)malloc(sizeof(NodePlayer));
    node->p = player;

    node->next = ll_player->head;
    ll_player->head->prev = node;
    ll_player->head = node;
}

void delete_player(LinkedListPlayer* ll_player, NodePlayer* node)
{
    if (node == NULL) return;

    NodePlayer *front = node->prev;
    NodePlayer *back = node->next;
    if (front) front->next = back;
    if (back) back->prev = front;

    free(node);
}

void change_player_dir(LinkedListPlayer* ll_player, NodePlayer* node, Dir dir)
{
    if (node == NULL) return;

    node->p.dir = dir;
}

Rect player_origin_rect(void)
{
    return (Rect) {
        .x = 300,
        .y = 300,
        .w = 100,
        .h = 100,
    };
}