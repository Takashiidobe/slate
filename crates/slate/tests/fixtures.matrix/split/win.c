#include "platform.h"

const char *platform_name(void) {
    return "win";
}

int platform_value(void) {
    return doubled(50) + 1;
}
