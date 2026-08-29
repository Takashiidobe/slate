#include <stdio.h>
#include <string.h>

static int first_byte(const unsigned char *bytes, int len) {
  (void)strlen((const char *)bytes);
  int value = 0;
  for (int i = 0; i < len; ++i)
    value += bytes[i];
  return value;
}

int main(void) {
  const unsigned char bytes[] = {0xc3, 0xa9, 0};
  int value = first_byte(bytes, 1);
  printf("%d\n", value);
  return 0;
}

// REWRITES: fn first_byte(arg{{[0-9]+}}: *mut u8, arg{{[0-9]+}}: i32) -> i32
// REWRITES-NOT: std::str::from_utf8_unchecked(
