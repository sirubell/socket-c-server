#include <stdbool.h>
#include <assert.h>

#include "game.h"

static Game game;

void game_init(void) {
    action_queue_init(&game.aq);
    game.ll_player.head = NULL;
    game.ll_platform.head = NULL;
    game.state = Starting;
    str_with_mutex_init(&game.environment);
}

void handle_actions(void) {
    while (action_queue_has(&game.aq)) {
        Action a = action_queue_pop(&game.aq);

        switch (a.type) {
            case NoOp: {
                assert(false && "not reachable");
            }

            case CreatePlayer: {
                
            } break;
            case DeletePlayer: {

            } break;
            case ChangePlayerDir: {

            } break;
            case CreatePlatform: {

            } break;
            case DeletePlatform: {

            } break;
        }
    }
}

void update_game(float time) {
    assert(false && "update_game is not implemented yet");
}

NodePlayer* query_has(int fd) {
    return _query_has(&game.ll_player, fd);
}