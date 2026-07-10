#include <stdio.h>

int main(void) {
    int a[5] = {1, 2, 3, 4, 5};
    int partial[4] = {7, 8};
    char s[6] = "hello";
    char padded[8] = "hi";

    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += a[i];
    }
    printf("%d\n", sum);
    printf("%d %d\n", partial[1], partial[3]);
    printf("%s\n", s);
    printf("%s %d\n", padded, padded[4]);
    return 0;
}
