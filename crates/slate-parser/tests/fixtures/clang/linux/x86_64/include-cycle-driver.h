#ifndef DRIVER
#define DRIVER() int value;
#include "include-cycle-driver.h"
#undef DRIVER
#else
DRIVER()
#endif
