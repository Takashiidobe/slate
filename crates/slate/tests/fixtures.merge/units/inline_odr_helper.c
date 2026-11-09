#include <stdio.h>
#include "inline_odr.h"

int other_unit(void) {
    printf("other twice %d\n", twice_counted(1));
    printf("other thrice %d\n", thrice_counted(1));
    return next_ticket();
}
