#include <stdio.h>
#include <string.h>

struct ld_box {
  long double v;
};

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  struct ld_box b = {0x1.123456789abcdef1p+60L};
  dump80("before", b.v);
  ++b.v;
  dump80("after", b.v);
  return 0;
}
