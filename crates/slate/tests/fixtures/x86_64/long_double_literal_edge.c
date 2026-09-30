#include <stdio.h>
#include <string.h>

static long double global_edge = 0x1.0000000000000001p0L;

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  long double local_edge = 0x1.0000000000000001p0L;
  dump80("local", local_edge);
  dump80("global", global_edge);
  return 0;
}
