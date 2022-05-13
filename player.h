#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "rect.h"
#include "str.h"

typedef enum {
    NoDir,
    Left,
    Right,
    Up,
    Down,
} Dir;

typedef struct {
    Rect rect;
    float heart;
    Str name;
    Dir dir;
    int fd;
} Player;

typedef struct NodePlayer NodePlayer;
struct NodePlayer{
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
};

typedef struct LinkedListPlayer{
    NodePlayer* head;
} LinkedListPlayer;

Dir get_dir(char c);
NodePlayer* _query_has(LinkedListPlayer* ll_player, int fd);
void create_player(LinkedListPlayer* ll_player, Player player);
void delete_player(LinkedListPlayer* ll_player, NodePlayer* node);
void change_player_dir(LinkedListPlayer* ll_player, NodePlayer* node, Dir dir);
void player_revive(Player* player);

Rect player_origin_rect(void);

Str to_str_player(Player* player);

#endif