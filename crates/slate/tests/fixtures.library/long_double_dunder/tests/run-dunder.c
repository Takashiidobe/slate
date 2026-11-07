extern long double __ld_scale(long double x, int n);
extern long double __ld_twice(long double x);
extern int         scaled_tenths(int n);

int main(void) {
  if (__ld_scale(1.5L, 3) != 12.0L)
    return 1;
  if (__ld_twice(0.25L) != 0.5L)
    return 2;
  return scaled_tenths(4) == 32 ? 0 : 3;
}
