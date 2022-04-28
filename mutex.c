#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <inttypes.h>

#define NUM_THREADS 10
#define NUM_THING 10
#define NUM_INCREMENT 10

typedef struct {
    int x;
    pthread_mutex_t mutex;
} Something;

void* do_something(void *arg)
{
    Something *thing = (Something*)arg;

    for (int i = 0; i < NUM_THING; i++) {
        // pthread_mutex_lock(&thing[i].mutex);
        {
            thing[i].x++;
        }
        // pthread_mutex_unlock(&thing[i].mutex);
    }
}

int main(void) 
{
    struct timeval start, stop;
    Something thing[NUM_THING];
    for (int i = 0; i < NUM_THING; i++) {
        thing[i].x = 0;
        // pthread_mutex_init(&thing[i].mutex, NULL);
    }

    gettimeofday(&start, NULL);
    {
        pthread_t threads[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&threads[i], NULL, do_something, &thing);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    gettimeofday(&stop, NULL);
    uint64_t execution_time = (stop.tv_sec - start.tv_sec) * 1000 * 1000 + (stop.tv_usec - start.tv_usec);

    printf("%d\n", thing[0].x);
    printf("took: %fms\n", (float) execution_time / 1000);
    return 0;
}
