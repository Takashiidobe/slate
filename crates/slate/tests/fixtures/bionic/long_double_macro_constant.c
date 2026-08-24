#include <float.h>

long double f(void) { return LDBL_TRUE_MIN; }

int main(void) { return f() == 0.0L; }
// REWRITES-BIONIC-X86_64-DAG: #![feature(f128)]
// REWRITES-BIONIC-X86_64-DAG: f128::from_bits
// REWRITES-BIONIC-X86_64-NOT: struct LongDouble
