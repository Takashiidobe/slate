/* { dg-options "-maes -mpclmul -mrdrnd -msha -madx" } */
#include <stdio.h>

int main(void) {
#if defined(__AES__) && defined(__PCLMUL__) && defined(__RDRND__) && defined(__SHA__) && defined(__ADX__)
  int all = 1;
#else
  int all = 0;
#endif
#ifdef __AVX__
  int avx = 1;
#else
  int avx = 0;
#endif
  printf("%d %d\n", all, avx);
  return 0;
}
