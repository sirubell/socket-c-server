#include "action.h"
#include "player.h"
#include "platform.h"

typedef struct {
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
} NodePlayer;

typedef struct {
    NodePlayer* head;
} LinkedListPlayer;

typedef struct {
    Platform p;
    NodePlatform* next;
    NodePlatform* prev;
} NodePlatform;

typedef struct {
    NodePlatform* head;
} LinkedListPlatform;

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

#ifdef _GAME_H_IMPLEMENTATION_

static Game game;
void game_init(void) {
    action_queue_init(&game.aq);
    game.ll_player.head = NULL;
    game.ll_platform.head = NULL;
    game.state = Starting;
    str_with_mutex_init(&game.environment);
}

void handle_actions(void) {
    assert(false && "handle_actions is not implemented yet");
}

void update_game(float time) {
    assert(false && "update_game is not implemented yet");
}

#endif