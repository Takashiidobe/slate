#include <stdio.h>
#include <string.h>

struct ld_pair {
  long double a;
  long double b;
};

extern struct ld_pair ext_pair_roundtrip(struct ld_pair p);

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  struct ld_pair p = {0x1.0000000000000001p0L, 0x1.0000000000000002p0L};
  struct ld_pair r = ext_pair_roundtrip(p);
  dump80("a", r.a);
  dump80("b", r.b);
  return 0;
}
