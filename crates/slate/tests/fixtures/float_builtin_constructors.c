#include <math.h>
#include <stdio.h>
static int calls;
static const char *payload(void) { ++calls; return "0x23"; }
int main(void) {
    volatile float infinity = INFINITY;
    volatile double huge = __builtin_inf();
    volatile float nan = NAN;
    union { float value; unsigned int bits; } f = {__builtin_nanf("0x123")};
    union { double value; unsigned long long bits; } d = {__builtin_nan(payload())};
    printf("%d %d %d %08x %016llx %d\n", !!isinf(infinity), !!isinf(huge), !!isnan(nan), f.bits, d.bits, calls);
    return calls != 1;
}
