#include <stdio.h>

struct Pair {
    int left;
    int right;
};

static int sum_pair(int a, int b) {
    struct Pair p;
    p.left = a;
    p.right = b;
    return p.left + p.right;
}

static int overwrite_left(int a, int b) {
    struct Pair p;
    p.left = a;
    p.right = b;
    p.left = p.right + 2;
    return p.left;
}

int main(void) {
    printf("%d\n", sum_pair(4, 5));
    printf("%d\n", overwrite_left(3, 8));
    return 0;
}
