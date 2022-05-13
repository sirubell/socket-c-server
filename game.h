#ifndef _GAME_H_
#define _GAME_H_

#include "action.h"
#include "platform.h"
#include "player.h"
#include "str.h"

typedef enum {
  Starting,
  Gaming,
  End,
} GameState;

typedef struct {
  int player_counter;
  int platform_counter;
  int tick;
  int platform_generation_tick;
  float current_time;
  float scalar;
  Str winner;
} Parameter;

typedef struct {
  ActionQueue aq;
  LinkedListPlayer ll_player;
  LinkedListPlatform ll_platform;
  GameState state;
  StrWithMutex environment;
  Parameter para;
} Game;

void para_init(Parameter *para);
void game_init(void);
void handle_actions(void);
void update_game(float time);
void action_push(Action a);
NodePlayer *query_has(int fd);

Str get_environment(void);
void update_environment(void);

#endif
