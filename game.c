#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "game.h"

static void player_down();
static void platform_up();
static void platform_remove_overflow();
static void player_go_direction();
static void adjust_player_position();
static void calculate_damage();
static void check_player_position();
static int count_player_alive();
static void set_winner();
static void new_game();

static Game game;

void para_init(Parameter* para) {
    para->player_counter = 0;
    para->platform_counter = 0;
    para->tick = 0;
    para->platform_generation_tick = 200;
    para->current_time = 0.0f;
    para->scalar = 1.0f;
    str_init(&para->winner);
}

void game_init(void) {
    action_queue_init(&game.aq);
    game.ll_player.head = NULL;
    game.ll_platform.head = NULL;
    game.state = Starting;
    str_with_mutex_init(&game.environment);
    para_init(&game.para);

    Action a = {
        .type = NewGame,
    };
    action_push(a);
}

void handle_actions(void) {
    // printf("action queue: %d %d\n", game.aq.head, game.aq.tail);
    while (action_queue_has(&game.aq)) {
        Action a = action_queue_pop(&game.aq);

        switch (a.type) {
            case NoOp: {
                assert(false && "not reachable");
            }

            case CreatePlayer: {
                // printf("CreatePlayer\n");
                game.para.player_counter++;

                Player player = (Player) {
                    .name = to_str_int(game.para.player_counter),
                    .fd = a.optint,
                };
                player_revive(&player);

                create_player(&game.ll_player, player);
            } break;
            case DeletePlayer: {
                // printf("DeletePlayer\n");
                delete_player(&game.ll_player, a.optptr);
            } break;
            case ChangePlayerDir: {
                // printf("ChangePlayerDir\n");
                change_player_dir(&game.ll_player, a.optptr, get_dir(a.optint));
            } break;
            case CreatePlatform: {
                // printf("CreatePlatform\n");
                game.para.platform_counter++;

                Platform platform = {
                    .rect = platform_random_rect(),
                    .name = to_str_int(game.para.platform_counter),
                    .type = a.optint,
                };

                create_platform(&game.ll_platform, platform);
            } break;
            case CreateInitPlatform: {
                game.para.platform_counter++;

                Platform platform = {
                    .rect = {
                        .x = 200,
                        .y = 500,
                        .w = 200,
                        .h = 20,
                    },
                    .name = to_str_int(game.para.platform_counter),
                    .type = Normal,
                };

                create_platform(&game.ll_platform, platform);
            } break;
            case DeletePlatform: {
                // printf("DeletePlatform\n");
                delete_platform(&game.ll_platform, a.optptr);
            } break;
            case NewGame: {
                // printf("NewGame\n");
                new_game();
            } break;
        }
    }
}

void update_game(float time) {
    // printf("tick: %d\n", game.para.tick);
    game.para.tick++;
    game.para.current_time += time;

    if (game.para.current_time >= 20.0f) {
        game.state = Gaming;
        str_init(&game.para.winner);
    }

    // printf("update game: 1\n");
    player_down();
    // printf("update game: 2\n");
    if (game.state == Gaming) {
        platform_up();
    }
    // printf("update game: 3\n");
    adjust_player_position();
    // printf("update game: 4\n");
    
    player_go_direction();
    // printf("update game: 5\n");
    adjust_player_position();
    // printf("update game: 6\n");


    calculate_damage();
    // printf("update game: 7\n");

    platform_remove_overflow();
    // printf("update game: 8\n");
    check_player_position();
    // printf("update game: 9\n");

    int alive = count_player_alive();
    // printf("update game: 10\n");

    if (game.state == Gaming && alive == 1) {
        set_winner();
    }
    // printf("update game: 11\n");
    
    if (game.state == Gaming && game.para.tick % game.para.platform_generation_tick == 0) {
        int platform_type;
        int random_val = rand() % 10;
        if (random_val < 3) {
            platform_type = (int)Spike;
        } else {
            platform_type = (int)Normal;
        }

        Action a = {
            .type = CreatePlatform,
            .optint = platform_type;
        };
        action_push(a);
    }
    // printf("update game: 12\n");

    if (alive == 0) {
        Action a = {
            .type = NewGame,
        };
        action_push(a);
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

void update_environment(void) {
    bool first;
    Str str, tmp;
    str_init(&str);

    first = true;
    NodePlayer* player = game.ll_player.head;
    while (player) {
        if (first) {
            first = false;
        } else {
            str_cat_char(&str, '|');
        }
        tmp = to_str_player(&player->p);
        str_cat(&str, &tmp);

        player = player->next;
    }
    str_cat_char(&str, '\n');

    first = true;
    NodePlatform* platform = game.ll_platform.head;
    while (platform) {
        if (first) {
            first = false;
        } else {
            str_cat_char(&str, '|');
        }
        tmp = to_str_platform(&platform->p);
        str_cat(&str, &tmp);

        platform = platform->next;
    }
    str_cat_char(&str, '\n');

    tmp = to_str_int((int)(game.para.current_time * 1000));
    str_cat(&str, &tmp);
    str_cat_char(&str, '\n');

    if (game.state == Starting) {
        str_cat_cstr(&str, "Game Start in ");
        tmp = to_str_int((int)(20.0f - game.para.current_time));
        str_cat(&str, &tmp);
        str_cat_char(&str, '\n');
    }

    str_cat(&str, &game.para.winner);

    pthread_mutex_lock(&game.environment.mutex);
    game.environment.str = str;
    pthread_mutex_unlock(&game.environment.mutex);
}

static void player_down() {
    NodePlayer* tmp = game.ll_player.head;
    while (tmp) {
        tmp->p.rect.y += game.para.scalar * 3.0f;
        tmp = tmp->next;
    }
}
static void platform_up() {
    NodePlatform* tmp = game.ll_platform.head;
    while (tmp) {
        tmp->p.rect.y -= game.para.scalar * 3.0f;
        tmp = tmp->next;
    }
}
static void platform_remove_overflow() {
    NodePlatform* tmp = game.ll_platform.head;
    while (tmp) {
        if (tmp->p.rect.y < 0) {
            Action a = {
                .type = DeletePlatform,
                .optptr = tmp,
            };
            action_push(a);
        }

        assert(tmp != tmp->next);
        // printf("platform name: %.*s\n", (int)tmp->p.name.len, tmp->p.name.s);

        tmp = tmp->next;
    }
}
static void player_go_direction() {
    NodePlayer* tmp = game.ll_player.head;
    while (tmp) {
        if (tmp->p.dir == Left) {
            tmp->p.rect.x -= game.para.scalar * 3.0f;
        }
        if (tmp->p.dir == Right) {
            tmp->p.rect.x += game.para.scalar * 3.0f;
        }
        tmp = tmp->next;
    }
}
static void adjust_player_position() {
    NodePlayer* player = game.ll_player.head;
    while (player) {
        NodePlatform* platform = game.ll_platform.head;
        while (platform) {
            calculate_collision(&player->p.rect, &platform->p.rect);

            platform = platform->next;
        }
        player = player->next;
    }
}

static void calculate_damage() {
    NodePlayer* player = game.ll_player.head;
    while (player) {
        bool damaged = false;

        NodePlatform* platform = game.ll_platform.head;
        while (platform) {
            if (platform->p.type == Spike && rect_is_on(&player->p.rect, &platform->p.rect)) {
                player->p.heart -= game.para.scalar * 0.05f;
                damaged = true;
            }
            platform = platform->next;
        }

        if (!damaged && player->p.heart < 100 && player->p.heart > 0)
        {
            player->p.heart += game.para.scalar * 0.02f;
        }

        player = player->next;
    }
}

static void check_player_position() {
    NodePlayer* player = game.ll_player.head;
    while (player) {
        if (player->p.rect.y >= 900) {
            player->p.heart = 0;
        }
        player = player->next;
    }
}
static int count_player_alive() {
    int count = 0;
    NodePlayer* player = game.ll_player.head;
    while (player) {
        if (player->p.heart > 0) count++;
        player = player->next;
    }
    return count;
}

static void set_winner() {
    NodePlayer* player = game.ll_player.head;
    while (player) {
        if (player->p.heart > 0) {
            game.para.winner = player->p.name;
            break;
        }
        player = player->next;
    }
}

static void new_game() {
    game.state = Starting;
    game.para.current_time = 0;
    game.para.tick = 0;

    NodePlayer* player = game.ll_player.head;
    while (player) {
        player_revive(&player->p);
        player = player->next;
    }
    
    NodePlatform* platform = game.ll_platform.head;
    while (platform) {
        Action a = {
            .type = DeletePlatform,
            .optptr = platform,
        };
        action_push(a);

        platform = platform->next;
    }

    Action a = {
        .type = CreateInitPlatform,
    };
    action_push(a);
}
