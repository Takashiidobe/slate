/* { dg-options "-O2 -fno-inline" } */
int printf(const char *, ...);

int main(void) {
#ifdef __NO_INLINE__
  int no_inline = 1;
#else
  int no_inline = 0;
#endif
  printf("%d\n", no_inline);
  return 0;
}
