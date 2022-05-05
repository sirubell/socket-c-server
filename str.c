#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "str.h"

void str_init(Str* str)
{
    str->len = 0;
}

void str_with_mutex_init(StrWithMutex* x)
{
    str_init(&x->str);
    pthread_mutex_init(&x->mutex, NULL);
}

void str_cat(Str* str1, Str* str2)
{
    if (str1->len + str2->len > STR_CAPACITY) {
        perror("str len overflow");
        exit(1);
    }

    for (int i = 0; i < str2->len; i++) {
        str1->s[str1->len++] = str2->s[i];
    }
}

void str_cat_cstr(Str* str1, const char* str2)
{
    size_t len = strlen(str2);
    if (str1->len + len > STR_CAPACITY) {
        perror("str len overflow");
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        str1->s[str1->len++] = str2[i];
    }
}

void str_cat_char(Str* str, const char c)
{
    if (str->len + 1 > STR_CAPACITY) {
        perror("str len overflow");
        exit(1);
    }

    str->s[str->len++] = c;
}

Str to_str_int(int x)
{
    Str str;
    str_init(&str);

    if (x == 0) {
        str_cat_char(&str, '0');
        return str;
    }

    bool neg = (x < 0);
    if (neg) x *= -1;
    
    while (x) {
        str_cat_char(&str, '0' + x % 10);
        x /= 10;
    }
    if (neg) {
        str_cat_char(&str, '-');
    }

    str_reverse(&str);
    return str;
}

void str_reverse(Str* str)
{
    for (int i = 0; i < str->len / 2; i++) {
        char tmp = str->s[i];
        str->s[i] = str->s[str->len - 1 - i];
        str->s[str->len - 1 - i] = tmp;
    }
}
