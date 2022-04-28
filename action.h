#include <stdatomic.h>

typedef enum {
    NoOp = 0,

    CreatePlayer,
    DeletePlayer,
    ChangePlayerDir,

    CreatePlatform,
    DeletePlatform,

} ActionType;

typedef struct {
    ActionType at;
    int optint;
    void* optptr;
} Action;

#define NUM_ACTION 1024

typedef struct {
    Action action[NUM_ACTION];
    atomic_int head;
    atomic_int tail;
} ActionQueue;

void action_queue_init(ActionQueue* aq);