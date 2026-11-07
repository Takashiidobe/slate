long double __ld_scale(long double x, int n) {
  while (n-- > 0)
    x *= 2;
  return x;
}

long double __ld_twice(long double x) { return x + x; }
