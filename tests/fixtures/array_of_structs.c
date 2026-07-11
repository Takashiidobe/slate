#include <stdio.h>

struct Point {
    int x;
    int y;
};

int main(void) {
    struct Point ps[2];
    ps[0].x = 1;
    ps[0].y = 2;
    ps[1].x = 3;
    ps[1].y = 4;
    printf("%d\n", ps[0].x + ps[1].y);

    struct Point init[2] = {{10, 20}, {30, 40}};
    printf("%d\n", init[0].y + init[1].x);
    return 0;
}
