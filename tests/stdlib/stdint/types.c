#include <stdint.h>
#include <stdio.h>
int main(void) {
  int32_t a = 100000;
  uint8_t b = 250;
  int16_t c = -3000;
  int64_t d = 5000000000;
  printf("%d %d %d\n", (int)a, (int)b, (int)c);
  printf("%lld\n", (long long)d);
  return 0;
}
