#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 0;
    x++;
    printf("%d\n", x);

    return 0;
}
