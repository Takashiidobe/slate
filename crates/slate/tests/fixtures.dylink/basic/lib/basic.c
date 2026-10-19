#include "dylink.h"

EXPORT int plug_add(int a, int b) { return a + b * 2; }

EXPORT long long plug_mul64(long long a, long long b) { return a * b; }

EXPORT unsigned char plug_not8(unsigned char v) { return (unsigned char)~v; }

EXPORT double plug_hypot2(double x, double y) { return x * x + y * y; }

int plug_internal(int v) { return v + 1; }

EXPORT int plug_use_internal(int v) { return plug_internal(v) * 2; }
