#include <stdbool.h>
#include <assert.h>

#include "game.h"

static Game game;

void para_init(Parameter* para) {
    para->player_count = 0;
    para->tick = 0;
    para->current_time = 0.0f;
}

void game_init(void) {
    action_queue_init(&game.aq);
    game.ll_player.head = NULL;
    game.ll_platform.head = NULL;
    game.state = Starting;
    str_with_mutex_init(&game.environment);
    para_init(&game.para);
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
    game.para.tick++;
    game.para.current_time += time;

    if (game.para.current_time >= 20.0f)
    {
        game.state = Gaming;
        // winner = String.Empty;
    }

    player_down();
    if (game.state == Gaming) {
        platform_up();
    }
    platform_remove_overflow();

    adjust_player_position();

    player_go_direction();
    adjust_player_position();

    calculate_damage();

    check_player_position();

    int alive = count_player_alive();

    if (game.state == Gaming && alive == 1)
    {
        found_winner();
    }
    if (alive == 0)
    {
        new_game();
    }

    if (game.state == Gaming && game.para.tick % 40 == 0)
    {
        generate_platform();
    }
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