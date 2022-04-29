#include <stdbool.h>
#include <assert.h>

#include "game.h"

static void player_down();
static void platform_up();
static void platform_remove_overflow();
static void player_go_direction();
static void adjust_player_position();
static void calculate_position();
static void calculate_damage();
static void check_player_position();
static int count_player_alive();
static void set_winner();
static void new_game();
static void generate_platform();

static Game game;

void para_init(Parameter* para) {
    para->player_count = 0;
    para->tick = 0;
    para->platform_generation_tick = 40;
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
                    .rect = player_origin_rect(),
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
                    .rect = platform_random_rect(),
                    .type = a.optint,
                };

                create_platform(&game.ll_platform, platform);
            } break;
            case DeletePlatform: {
                delete_platform(&game.ll_platform, a.optptr);
            } break;
            case NewGame: {
                new_game();
            } break;
        }
    }
}

void update_game(float time) {
    game.para.tick++;
    game.para.current_time += time;

    if (game.para.current_time >= 20.0f) {
        game.state = Gaming;
        str_init(&game.para.winner);
    }

    player_down();
    if (game.state == Gaming) {
        platform_up();
    }
    adjust_player_position();
    
    player_go_direction();
    adjust_player_position();

    calculate_damage();

    platform_remove_overflow();
    check_player_position();

    int alive = count_player_alive();

    if (game.state == Gaming && alive == 1) {
        set_winner();
    }
    
    if (game.state == Gaming && game.para.tick % game.para.platform_generation_tick == 0) {
        generate_platform();
    }

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
    Str str;
    str_init(&str);

    NodePlayer* player = game.ll_player.head;
    while (player) {
        
        player = player->next;
    }
}

static void player_down() {
    NodePlayer* tmp = game.ll_player.head;
    while (tmp) {
        tmp->p.rect.y += game.para.scalar * 1.0f;
        tmp = tmp->next;
    }
}
static void platform_up() {
    NodePlatform* tmp = game.ll_platform.head;
    while (tmp) {
        tmp->p.rect.y -= game.para.scalar * 1.0f;
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
        tmp = tmp->next;
    }
}
static void player_go_direction() {
    NodePlayer* tmp = game.ll_player.head;
    while (tmp) {
        if (tmp->p.dir == Left) {
            tmp->p.rect.x -= game.para.scalar * 1.0f;
        }
        if (tmp->p.dir == Right) {
            tmp->p.rect.x += game.para.scalar * 1.0f;
        }
        tmp = tmp->next;
    }
}
static void adjust_player_position() {
    NodePlayer* player = game.ll_player.head;
    while (player) {
        NodePlatform* platform = game.ll_platform.head;
        while (platform) {
            calculate_position(&player->p.rect, &platform->p.rect);

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
                player->p.heart -= game.para.scalar * 1.0f;
                damaged = true;
            }
            platform = platform->next;
        }

        if (!damaged && player->p.heart < 100 && player->p.heart > 0)
        {
            player->p.heart += game.para.scalar * 1.0f;
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
    NodePlatform* platform = game.ll_platform.head;
    while (platform) {
        Action a = {
            .type = DeletePlatform,
            .optptr = platform,
        };
        action_push(a);

        platform = platform->next;
    }
}

static void generate_platform() {
    Action a = {
        .type = CreatePlatform,
        .optint = (int)Normal,
    };
    action_push(a);
}