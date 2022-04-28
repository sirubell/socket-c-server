#include "action.h"
#include "linkedlist_player.h"
#include "linkedlist_platform.h"

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