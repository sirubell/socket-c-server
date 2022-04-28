#include <stdint.h>
#include <pthread.h>

#define STR_CAPACITY 1024

typedef struct {
    char s[STR_CAPACITY];
    size_t len;
} Str;

typedef struct {
    Str str;
    pthread_mutex_t mutex;
} StrWithMutex;

void str_with_mutex_init(StrWithMutex* x);