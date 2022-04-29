#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "server.h"
#include "game.h"

int max(int a, int b);

int main(void)
{
    struct timeval stop, start;
    const int frame_time = (1.0f / 60) * 1000 * 1000;
    int dt = 0;

    game_init();

    start_server();

    while (true) {
        gettimeofday(&start, NULL);
        {
            float time = (float)max(dt, frame_time) / 1000 / 1000;
            update_game(time);
            update_environment();
            
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

int max(int a, int b)
{
    if (a > b) return a;
    return b;
}