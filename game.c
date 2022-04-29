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
    game.para.player_count = 0;
}

void handle_actions(void) {
    while (action_queue_has(&game.aq)) {
        Action a = action_queue_pop(&game.aq);

        switch (a.type) {
            case NoOp: {
                assert(false && "not reachable");
            }

            case CreatePlayer: {
                game.para.player_count++;

                Player player = (Player) {
                    .rect = rect_origin(),
                    .heart = 100.0f,
                    .name = to_str_int(game.para.player_count),
                    .dir = NoDir,
                    .fd = a.optint,
                };

                create_player(&game.ll_player, player);
            } break;
            case DeletePlayer: {
                delete_player(&game.ll_player, a.optptr);
            } break;
            case ChangePlayerDir: {
                change_player_dir(&game.ll_player, a.optptr, get_dir(a.optint));
            } break;
            case CreatePlatform: {
                Platform platform = {
                    .rect = platform_random(),
                    .type = a.optint,
                };

                create_platform(&game.ll_platform, platform);
            } break;
            case DeletePlatform: {
                delete_platform(&game.ll_platform, a.optptr);
            } break;
        }
    }
}

void update_game(float time) {
    assert(false && "update_game is not implemented yet");
}

void action_push(Action a) {
    action_queue_push(&game.aq, a);
}

NodePlayer* query_has(int fd) {
    return _query_has(&game.ll_player, fd);
}

Str get_environment(void) {
    pthread_mutex_lock(&game.environment.mutex);
    Str str = game.environment.str;
    pthread_mutex_unlock(&game.environment.mutex);
    return str;
}