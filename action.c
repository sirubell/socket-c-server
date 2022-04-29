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
    return (aq->head % NUM_ACTION != aq->tail % NUM_ACTION) && (aq->actions[aq->head].type != NoOp);
}

Action action_queue_pop(ActionQueue* aq)
{
    int tmp = atomic_fetch_add(&aq->head, 1);
    Action a = aq->actions[tmp % NUM_ACTION];
    aq->actions[tmp % NUM_ACTION].type = NoOp;

    return a;
}

void action_queue_push(ActionQueue* aq, Action a)
{
    int tmp = atomic_fetch_add(&aq->tail, 1);

    aq->actions[tmp % NUM_ACTION].optint = a.optint;
    aq->actions[tmp % NUM_ACTION].optptr = a.optptr;
    asm volatile("": : :"memory"); // memory fence
    aq->actions[tmp % NUM_ACTION].type = a.type;
}