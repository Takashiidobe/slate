#include <math.h>
#include <stdio.h>
#include <string.h>

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  volatile long double x = 1.0L;
  dump80("sinl", sinl(x));
  return 0;
}
