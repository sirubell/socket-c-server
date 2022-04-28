#ifndef _GAME_H_
#define _GAME_H_

#include "action.h"
#include "player.h"
#include "platform.h"

typedef enum {
    Starting,
    Gaming,
    End,
} GameState;

typedef struct {
    ActionQueue aq;
    LinkedListPlayer ll_player;
    LinkedListPlatform ll_platform;
    GameState state;
    StrWithMutex environment;
} Game;

void game_init(void);
void handle_actions(void);
void update_game(float time);
NodePlayer* query_has(int fd);

#endif