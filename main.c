#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "game.h"
#include "server.h"

int max(int a, int b);

int main(void) {
  srand(time(NULL));

  struct timeval stop, start;
  const int frame_time = (1.0f / 60) * 1000 * 1000;
  int dt = 0;

  game_init();
  handle_actions();

  sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
  start_server();

  while (true) {
    // printf("main: 1\n");
    gettimeofday(&start, NULL);
    {
      float time = (float)max(dt, frame_time) / 1000 / 1000;
      update_game(time);
      // printf("main: 2\n");
      update_environment();
      // printf("main: 3\n");

      handle_actions();
      // printf("main: 4\n");
    }
    gettimeofday(&stop, NULL);

    // dt is above computing time
    dt = (stop.tv_sec - stop.tv_sec) * 1000 * 1000 +
         (stop.tv_usec - start.tv_usec);
    if (frame_time > dt) {
      // printf("sleep %d usec\n", frame_time - dt);
      usleep(frame_time - dt);
    }
  }

  return 0;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}
