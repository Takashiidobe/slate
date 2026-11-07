#include <stdio.h>

#include "pair.h"

int main(void) {
    struct pair pair = {3, 14};
    printf("%d\n", run_lib(&pair));
    return 0;
}
