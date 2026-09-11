#include <float.h>

long double f(void) { return LDBL_TRUE_MIN; }

int main(void) { return f() == 0.0L; }

// LOWERING: #![feature(f128)]

// REWRITES-BIONIC-AARCH64: #![feature(f128)]
