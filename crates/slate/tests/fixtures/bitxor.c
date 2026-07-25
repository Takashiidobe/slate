#include <stdio.h>

static int ixor(int a, int b) { return a ^ b; }

int main(void) {
  printf("%d\n", ixor(12, 10));
  printf("%d\n", ixor(-1, 6));
  unsigned int u = 0xFFu;
  printf("%u\n", u ^ 0x0Fu);
  return 0;
}
