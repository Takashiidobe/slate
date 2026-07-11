#include <stdio.h>

extern int counter;
void bump(int by);

int main(void) {
    bump(3);
    bump(4);
    counter += 1;
    printf("%d\n", counter);
    return 0;
}
