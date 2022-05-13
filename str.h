#ifndef _STR_H_
#define _STR_H_

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

void str_init(Str* str);
void str_with_mutex_init(StrWithMutex* x);

void str_cat(Str* str1, Str* str2);
void str_cat_cstr(Str* str1, const char* str2);
void str_cat_char(Str* str, const char c);

Str to_str_int(int x);

void str_reverse(Str* str);

#endif