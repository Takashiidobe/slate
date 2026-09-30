#include <ieee754.h>

_Static_assert(__builtin_offsetof(union ieee754_float, f) == 0, "union ieee754_float.f offset differs from oracle");

typedef float slate_oracle_union_ieee754_float_f;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union ieee754_float *)0)->f), slate_oracle_union_ieee754_float_f), "union ieee754_float.f field type differs from oracle");

#ifndef IEEE754_DOUBLE_BIAS
#error "ieee754.h:IEEE754_DOUBLE_BIAS macro is missing from libc-shim"
#endif

#ifndef IEEE754_FLOAT_BIAS
#error "ieee754.h:IEEE754_FLOAT_BIAS macro is missing from libc-shim"
#endif

#ifndef IEEE854_LONG_DOUBLE_BIAS
#error "ieee754.h:IEEE854_LONG_DOUBLE_BIAS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
