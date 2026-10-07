#include <stdio.h>

int late(int x);
int early(int x);
int counted(int x);

/* section A */
int early(int x) { return x + 1; }

/* counted doc */
int counted(int x) {
    static int calls = 0;
    calls++;
    return x + calls;
}

/* section B */
int late(int x) { return x * 2; }

int main(void) {
    printf("%d %d %d\n", early(1), counted(1), late(2));
    return 0;
}
