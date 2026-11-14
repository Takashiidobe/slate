#include "platform.h"

static int unix_base = 21;

const char *platform_name(void) {
    return "unix";
}

int platform_value(void) {
    return doubled(unix_base);
}
