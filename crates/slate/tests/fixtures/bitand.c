#include <stdio.h>

static int iand(int a, int b) { return a & b; }

int main(void) {
  printf("%d\n", iand(12, 10));
  printf("%d\n", iand(-1, 6));
  unsigned int u = 0xF0u;
  printf("%u\n", u & 0x3Cu);
  return 0;
}
