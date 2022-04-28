#include "str.h"

void str_init(Str* str)
{
    memset(str->s, 0, sizeof(str->s));
    str->len = 0;
}

void str_with_mutex_init(StrWithMutex* x)
{
    str_init(&x->str);
    pthread_mutex_init(&x->mutex, NULL);
}