#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *end = 0;
    float f = strtof("-1.50tail", &end);
    printf("%.2f %c\n", (double)f, *end);
    return 0;
}
