long double __ld_scale(long double x, int n);
long double __ld_twice(long double x);

int scaled_tenths(int n) { return (int)(__ld_twice(__ld_scale(0.1L, n)) * 10); }
