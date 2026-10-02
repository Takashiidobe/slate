#include <stdio.h>

static int calls;

static unsigned int input(void) {
  ++calls;
  return 0x89abcdefu;
}

int main(void) {
  unsigned short small = __builtin_bswap16(0x89abu);
  unsigned int medium = __builtin_bswap32(input());
  unsigned long long large = __builtin_bswap64(0x89abcdef01234567ull);
  printf("%x %x %llx %d\n", small, medium, large, calls);
  return 0;
}
