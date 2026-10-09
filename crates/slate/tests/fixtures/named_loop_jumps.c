/* { dg-options "-std=c2y" } */
#include <stdio.h>

static int grid(int limit) {
    int hits = 0;
rows:
    for (int i = 0; i < 6; ++i) {
cols:
        for (int j = 0; j < 6; ++j) {
            if (j > i)
                continue rows;
            if (i * j > limit)
                break rows;
            if ((i + j) % 3 == 0)
                continue cols;
            hits += i * 10 + j;
        }
    }
    return hits;
}

static int dispatch(int n) {
    int total = 0;
    int k = 0;
outer:
    while (k < 8) {
        ++k;
choice:
        switch (n + k) {
        case 3:
            total += 1;
            break choice;
        case 5:
            total += 100;
            continue outer;
        case 9:
            total += 1000;
            break outer;
        default:
            break;
        }
        total += 10;
    }
    return total * 16 + k;
}

static int countdown(int n) {
    int steps = 0;
again:
    do {
        --n;
        ++steps;
        if (n % 2)
            continue again;
        if (n < 3)
            break again;
        steps += 5;
    } while (n > 0);
    return steps * 100 + n;
}

int main(void) {
    for (int limit = 0; limit < 30; limit += 7)
        printf("grid %d = %d\n", limit, grid(limit));
    for (int n = 0; n < 6; ++n)
        printf("dispatch %d = %d\n", n, dispatch(n));
    for (int n = 1; n < 12; n += 3)
        printf("countdown %d = %d\n", n, countdown(n));
    return 0;
}
