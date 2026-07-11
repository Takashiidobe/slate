#include <stdint.h>
#include <stdio.h>
int main(void) {
  printf("%d %d\n", INT8_MAX, INT8_MIN);
  printf("%d %d\n", INT16_MAX, INT16_MIN);
  printf("%d\n", UINT8_MAX);
  printf("%d %d\n", INT32_MAX == 2147483647 ? 1 : 0, UINT8_MAX == 255 ? 1 : 0);
  return 0;
}
