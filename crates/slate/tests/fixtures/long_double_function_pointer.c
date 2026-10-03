#include <math.h>
#include <stdio.h>

typedef long double (*unary)(long double);
typedef long double (*binary)(long double, int);

static long double twice(long double x) { return x * 2; }
long double halve(long double x) { return x / 2; }
static long double shift(long double x, int n) { return ldexpl(x, n) + 0.25L; }

static long double apply(unary f, long double x) { return f(x); }

int main(void) {
    unary table[] = {twice, halve, cbrtl, fabsl, twice};
    long double x = -27.0L;
    for (int i = 0; i < 5; i++) {
        x = apply(table[i], x);
        printf("%d %.6Lf\n", i, x);
    }
    binary b[] = {shift, ldexpl};
    for (int i = 0; i < 2; i++)
        printf("b%d %.6Lf\n", i, b[i](1.5L, 3));
    printf("eq %d %d\n", table[0] == table[4], table[0] == (unary)twice);
    return 0;
}
