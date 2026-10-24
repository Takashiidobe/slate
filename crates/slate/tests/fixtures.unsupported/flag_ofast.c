/* { dg-options "--flavor=gcc -Ofast" } */
#include <stdio.h>

int main(void) {
#ifdef __FAST_MATH__
  int fast = 1;
#else
  int fast = 0;
#endif
#ifdef __OPTIMIZE__
  int optimize = 1;
#else
  int optimize = 0;
#endif
  printf("%d %d\n", fast, optimize);
  return 0;
}
