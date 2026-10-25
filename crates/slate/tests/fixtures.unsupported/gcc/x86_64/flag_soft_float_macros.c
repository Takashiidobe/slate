/* { dg-options "-msoft-float" } */
#include <stdio.h>

int main(void) {
#ifdef _SOFT_FLOAT
  int soft = 1;
#else
  int soft = 0;
#endif
#ifdef __SSE__
  int sse = 1;
#else
  int sse = 0;
#endif
  printf("%d %d\n", soft, sse);
  return 0;
}
