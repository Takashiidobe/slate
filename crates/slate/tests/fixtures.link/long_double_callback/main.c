#include <stdio.h>
#include <string.h>

typedef long double (*ld_cb)(long double, long double);

extern long double invoke_cb(ld_cb cb, long double a, long double b);

static long double local_cb(long double a, long double b) { return a + b; }

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  long double a = 0x1.0000000000000001p0L;
  long double b = 0x1.0000000000000002p0L;
  dump80("result", invoke_cb(local_cb, a, b));
  return 0;
}
