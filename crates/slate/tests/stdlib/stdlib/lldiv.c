#include <stdio.h>
#include <stdlib.h>
int main(void) {
  lldiv_t d = lldiv(17LL, 5LL);
  printf("%lld %lld\n", d.quot, d.rem);
  return 0;
}
