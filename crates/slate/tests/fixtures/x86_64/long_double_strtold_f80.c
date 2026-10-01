#include <stdio.h>
#include <stdlib.h>
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
  char       *end;
  long double value = strtold("0x1.0000000000000002p0", &end);
  dump80("strtold", value);
  printf("%d\n", *end == '\0');
  return 0;
}
