#include <stdio.h>
#include <stdlib.h>
int main(void) {
  div_t d = div(17, 5);
  printf("%d %d\n", d.quot, d.rem);
  return 0;
}
