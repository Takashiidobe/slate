#include <stdio.h>

int main(void) {
  __auto_type x = 5;
  __typeof__(x) y = x + 2;
  printf("%d\n", y);
  return y;
}
