#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "server.h"
#include "game.h"

int main(void)
{
    struct timeval stop, start;
    const useconds_t frame_time = (1.0f / 60) * 1000 * 1000;
    useconds_t dt = 0;

    game_init();

    start_server();

    while (true) {
        gettimeofday(&start, NULL);
        {
            float time = (float)max(dt, frame_time) / 1000 / 1000;
            update_game(time);
            handle_actions();
        }
        gettimeofday(&stop, NULL);
        
        dt = (stop.tv_sec - stop.tv_sec) * 1000 * 1000 + (stop.tv_usec - start.tv_usec);
        if (frame_time > dt) {
            usleep(frame_time - dt);
        }
        
    }

	return 0;
}
