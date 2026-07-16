#include <stdio.h>

int main() {
    int x = 2;
    int r = 0;
    switch (x) {
    case 1:
        goto one;
    case 2:
        goto two;
    default:
        goto other;
    }
one:
    r = 10;
    goto done;
two:
    r = 20;
    goto done;
other:
    r = 30;
    goto done;
done:
    printf("%d\n", r);
    return 0;
}
