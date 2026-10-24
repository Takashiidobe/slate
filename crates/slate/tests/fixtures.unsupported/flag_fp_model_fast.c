/* { dg-options "-ffp-model=fast" } */
int printf(const char *, ...);

int main(void) {
#ifdef __FAST_MATH__
  int fast = 1;
#else
  int fast = 0;
#endif
#ifdef __NO_MATH_ERRNO__
  int no_errno = 1;
#else
  int no_errno = 0;
#endif
#ifdef __FINITE_MATH_ONLY__
  int finite = __FINITE_MATH_ONLY__;
#else
  int finite = -1;
#endif
  printf("%d %d %d\n", fast, no_errno, finite);
  return 0;
}
