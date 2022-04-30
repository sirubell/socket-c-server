#include <stdlib.h>

#include "player.h"

Dir get_dir(char c)
{
    if (c == '2') return Left;
    if (c == '3') return Right;
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

    printf("server: player: %.*s joined the game\n", (int)node->p.name.len, node->p.name.s);

    node->next = ll_player->head;
    node->prev = NULL;
    if (ll_player->head != NULL)
        ll_player->head->prev = node;
    ll_player->head = node;
}

void delete_player(LinkedListPlayer* ll_player, NodePlayer* node)
{
    if (node == NULL) return;

    printf("server: player: %.*s disconnected\n", (int)node->p.name.len, node->p.name.s);

    if (ll_player->head == node)
        ll_player->head = node->next;

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

void player_revive(Player* player)
{
    player->rect = player_origin_rect();
    player->heart = 100.0f;
    player->dir = NoDir;
}

Rect player_origin_rect(void)
{
    return (Rect) {
        .x = 300,
        .y = 100,
        .w = 50,
        .h = 50,
    };
}

Str to_str_player(Player* player)
{
    Str str, tmp;
    str_init(&str);

    tmp = to_str_int((int)player->rect.x);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)player->rect.y);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)player->rect.w);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)player->rect.h);
    str_cat(&str, &tmp);

    str_cat_char(&str, ',');

    str_cat(&str, &player->name);

    str_cat_char(&str, ',');

    tmp = to_str_int((int)player->heart);
    str_cat(&str, &tmp);

    return str;
}
