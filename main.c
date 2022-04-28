#include <stdio.h>
#include <stdbool.h>
#include "server.h"
#include "game.h"

typedef enum {
    NoOp = 0,

    CreatePlayer,
    DeletePlayer,
    ChangePlayerDir,

    CreatePlatform,
    DeletePlatform,

} ActionType;

typedef struct {
    ActionType at;
    int optint;
    void* optptr;
} Action;

typedef struct {
    Action action[NUM_ACTION];
    atomic_int head;
    atomic_int tail;
} ActionQueue;

typedef struct {
    Rect rect;
    float heart;
    String name;
} Player;

typedef struct {
    Player p;
    NodePlayer* next;
    NodePlayer* prev;
} NodePlayer;

typedef struct {
    NodePlayer head;
} LinkedListPlayer;

typedef struct {
    Rect rect;
    PlatformType type;
} Platform;

typedef struct {
    Platform p;
    NodePlatform* next;
    NodePlayform* prev;
} NodePlatform;

typedef struct {
    NodePlatform* head;
} LinkedListPlatform;

typedef struct {
    ActionQueue;
    LinkedListPlayer;
    LinkedListPlatform;
    GameState;
} Game;

static Game game;

int main(void)
{
    game_init();

    start_listening();

    while (true) {
        handle_action();
        update_game(float time);
        sleep(u time);
    }

	return 0;
}
