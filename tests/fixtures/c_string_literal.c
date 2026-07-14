#include <stdio.h>

static int first_plus_last(char *s) {
    return s[0] + s[10];
}

int main(void) {
    char *message = "write error";
    printf("%d\n", first_plus_last(message));
    return 0;
}
