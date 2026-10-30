#include "config.h"
#include "value.h"

#ifndef TARGET_EXTRA
#define TARGET_EXTRA 0
#endif
#ifndef LEVEL
#define LEVEL 0
#endif

int project_value(void) {
    int value = PROJECT_BASE + CONFIG_VALUE + TARGET_EXTRA + LEVEL * 10;
#ifdef FAST
    value += 1000;
#endif
#ifdef SMALL
    value += 2000;
#endif
    return value;
}
