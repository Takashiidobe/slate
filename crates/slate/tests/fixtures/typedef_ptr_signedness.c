#include <stdio.h>

typedef unsigned char my_char_t;

struct buffer {
    my_char_t *start;
    my_char_t *end;
};

int main(void) {
    my_char_t data[4] = {65, 66, 67, 0};
    struct buffer b;
    b.start = data;
    b.end = data + 3;
    printf("%c%c%c\n", b.start[0], b.start[1], b.start[2]);
    printf("%d\n", (int)(b.end - b.start));
    return 0;
}
