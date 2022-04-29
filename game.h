#ifndef _GAME_H_
#define _GAME_H_

#include "action.h"
#include "player.h"
#include "platform.h"
#include "str.h"

typedef enum {
    Starting,
    Gaming,
    End,
} GameState;

typedef struct {
    int player_count;
    
} Parameter;

typedef struct {
    ActionQueue aq;
    LinkedListPlayer ll_player;
    LinkedListPlatform ll_platform;
    GameState state;
    StrWithMutex environment;
    Parameter para;
} Game;

void game_init(void);
void handle_actions(void);
void update_game(float time);
void action_push(Action a);
NodePlayer* query_has(int fd);
Str get_environment(void);

#endif