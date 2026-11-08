#include "..\outer.h"
static int inner_value(void) { return outer_value() + 1; }
