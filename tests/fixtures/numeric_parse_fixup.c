#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char whole[] = "42";
    char leading[] = "  -17tail";
    char empty[] = "";
    char large[] = "999999999999999999999999999999";
    char flt[] = "  -3.5e2rest";
    char end_source[] = "12tail";
    char *end = 0;

    printf("%d %ld %ld %lu %.1f\n",
           atoi(whole),
           atol(leading),
           strtol(large, 0, 10),
           strtoul(empty, 0, 10),
           strtod(flt, 0));

    long raw = strtol(end_source, &end, 10);
    printf("%ld %c\n", raw, *end);
    return 0;
}
