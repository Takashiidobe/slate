#include <math.h>
#include <float.h>
#include <stdio.h>

static int calls;
static double once(double x) { ++calls; return x; }
int main(void) {
    union { unsigned long long bits; double value; } inf = {0x7ff0000000000000ULL}, nan = {0x7ff8000000000001ULL};
    union { unsigned int bits; float value; } finf = {0x7f800000U}, fnan = {0x7fc00001U};
    volatile double values[] = {0.0, -0.0, 1.0, -2.0, DBL_MIN / 2, inf.value, -inf.value, nan.value, -nan.value};
    for (int i = 0; i < 9; ++i) {
        double x = values[i];
        printf("%d%d%d%d%d %d\n", !!isnan(x), !!isinf(x), !!isfinite(x), !!isnormal(x), !!signbit(x), fpclassify(x));
    }
    volatile float small[] = {0.0f, -0.0f, 1.0f, FLT_MIN / 2, finf.value, fnan.value};
    for (int i = 0; i < 6; ++i) {
        float x = small[i];
        printf("%d%d%d%d%d %d\n", !!isnan(x), !!isinf(x), !!isfinite(x), !!isnormal(x), !!signbit(x), fpclassify(x));
    }
    int result = isnan(once(nan.value));
    printf("%d %d\n", !!result, calls);
    return calls != 1;
}
