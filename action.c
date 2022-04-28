#include "action.h"

void action_queue_init(ActionQueue* aq)
{
    for (int i = 0; i < NUM_ACTION; i++) {
        aq->actions[i].type = NoOp;
    }
    aq->head = 0;
    aq->tail = 0;
}

bool action_queue_has(ActionQueue* aq)
{
    return aq->head != aq->tail && aq->actions[aq->head].type != NoOp;
}

Action action_queue_pop(ActionQueue* aq)
{
    int tmp = atomic_fetch_add(&aq->head, 1);
    Action a = aq->actions[tmp];
    aq->actions[tmp].type = NoOp;

    return a;
}

void action_queue_push(ActionQueue* aq, Action a)
{
    int tmp = atomic_fetch_add(&aq->tail, 1);

    aq->actions[tmp].optint = a.optint;
    aq->actions[tmp].optptr = a.optptr;
    asm volatile("": : :"memory"); // memory fence
    aq->actions[tmp].type = a.type;
}