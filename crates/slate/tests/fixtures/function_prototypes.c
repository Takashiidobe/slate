#include <stdio.h>

int is_even(int n);

int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n - 1);
}

int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int square(int x);

int main(void) {
    printf("%d %d\n", is_odd(7), is_even(10));
    printf("%d\n", square(5));
    return 0;
}

int square(int x) {
    return x * x;
}
