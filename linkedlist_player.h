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