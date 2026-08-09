#include "config.h"

#if !defined(CONFIGURED_VALUE) || (1 - CONFIGURED_VALUE - 1 == 2) ||          \
    (CONFIGURED_VALUE < 0) || (CONFIGURED_VALUE > 64)
#error CONFIGURED_VALUE must be provided by the configured build
#endif

int configured_value(void) { return CONFIGURED_VALUE; }
