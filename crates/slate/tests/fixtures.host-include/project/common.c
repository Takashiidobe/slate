#include <greet/greet.h>
#include <greet/scale.h>

int greet_value(int seed) { return seed * GREET_SCALE + 1; }
