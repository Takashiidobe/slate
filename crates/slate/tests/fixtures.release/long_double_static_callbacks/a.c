typedef long double (*unary)(long double);

static long double step(long double x) { return x * 3 + 0.5L; }

unary a_step(void) { return step; }
