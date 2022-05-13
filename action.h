#ifndef _ACTION_H_
#define _ACTION_H_

#include <stdatomic.h>
#include <stdbool.h>

typedef enum {
    NoOp = 0,

    CreatePlayer,
    DeletePlayer,
    ChangePlayerDir,

    CreatePlatform,
    CreateInitPlatform,
    DeletePlatform,

    NewGame,

} ActionType;

typedef struct {
    ActionType type;
    int optint;
    void* optptr;
} Action;

#define NUM_ACTION 1024

typedef struct {
    Action actions[NUM_ACTION];
    atomic_int head;
    atomic_int tail;
} ActionQueue;

void action_queue_init(ActionQueue* aq);
bool action_queue_has(ActionQueue* aq);
Action action_queue_pop(ActionQueue* aq);
void action_queue_push(ActionQueue* aq, Action a);

#endif