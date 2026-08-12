#include <stdint.h>
#include <stdio.h>
#include <string.h>

static long double load80(const unsigned char *bytes) {
  long double value;
  memcpy(&value, bytes, 10);
  return value;
}

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  unsigned char one_bits[10] = {0, 0, 0, 0, 0, 0, 0, 0x80, 0xff, 0x3f};
  unsigned char two_bits[10] = {0, 0, 0, 0, 0, 0, 0, 0x80, 0x00, 0x40};
  unsigned char near_bits[10] = {1, 0, 0, 0, 0, 0, 0, 0x80, 0xff, 0x3f};
  long double one = load80(one_bits);
  long double two = load80(two_bits);
  long double near = load80(near_bits);

  dump80("add", one + two);
  dump80("sub", near - one);
  dump80("mul", one * two);
  dump80("div", two / one);
  dump80("neg", -near);

  printf("%d %d %d %d\n", one < two, near > one, one == one, near != one);
  printf("%d\n", (int)(one + two));
  dump80("from_i64", (long double)1234567890123LL);
  printf("%d\n", (int)(long double)123456789);
  return 0;
}
