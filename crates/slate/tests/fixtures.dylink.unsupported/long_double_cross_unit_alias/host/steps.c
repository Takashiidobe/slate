long double step(long double value) { return value + 0x1p-63L; }

extern long double alias_step(long double) __attribute__((alias("step")));
extern long double weak_step(long double) __attribute__((weak, alias("step")));
