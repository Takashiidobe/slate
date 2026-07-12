#include <stdio.h>

int classify(int a, int b) {
    int r = 0;
    while (a < b) {
        a = a + 1;
        r = r + 1;
    }
    if (a == b) {
        r = r + 10;
    }
    int t = (a > b) ? (a - b) : (b - a);
    int m = (a & b) + (a << 1);
    return r + t + m;
}

int main() {
    printf("%d\n", classify(2, 5));
    return 0;
}
