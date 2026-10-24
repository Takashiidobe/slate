/* { dg-options "-fsjlj-exceptions" } */
int printf(const char *, ...);

int main(void) {
#ifdef __USING_SJLJ_EXCEPTIONS__
  int sjlj = __USING_SJLJ_EXCEPTIONS__;
#else
  int sjlj = 0;
#endif
  printf("%d\n", sjlj);
  return 0;
}
