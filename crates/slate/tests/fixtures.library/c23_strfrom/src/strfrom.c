#include <stddef.h>
#include <stdlib.h>

int format_pi(char *out, size_t n) { return strfroml(out, n, "%.1f", 3.5L); }
