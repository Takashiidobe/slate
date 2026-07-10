#include <stdio.h>

typedef int my_int;
typedef unsigned char byte;
typedef long long wide;

struct Box {
    my_int value;
    byte tag;
};

static my_int add_alias(my_int a, my_int b) {
    my_int c = a + b;
    return c;
}

int main(void) {
    my_int x = 40;
    byte y = 200;
    wide z = 9000000000LL;
    struct Box bx;
    bx.value = add_alias(x, 2);
    bx.tag = y;
    printf("%d\n", bx.value);
    printf("%d\n", bx.tag);
    printf("%lld\n", z);
    printf("%d\n", (int)sizeof(my_int));
    return 0;
}
