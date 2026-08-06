#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
int main(void) {
  int32_t  x = 42;
  int64_t  y = 5000000000;
  uint32_t z = 4000000000;
  printf("%" PRId32 "\n", x);
  printf("%" PRId64 "\n", y);
  printf("%" PRIu32 "\n", z);
  return 0;
}
