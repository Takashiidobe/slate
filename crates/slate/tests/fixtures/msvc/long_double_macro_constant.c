#include <float.h>

long double f(void) { return LDBL_TRUE_MIN; }

int main(void) { return f() == 0.0L; }
// REWRITES-MSVC-DAG: f64::from_bits
// REWRITES-MSVC-NOT: LongDouble
