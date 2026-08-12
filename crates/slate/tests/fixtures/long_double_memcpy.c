#include <stdio.h>
#include <string.h>

union ld_union {
  long double   ld;
  unsigned char bytes[sizeof(long double)];
};

int main(void) {
  union ld_union src;
  union ld_union dst;
  int i;

  memset(&src, 0, sizeof(src));
  for (i = 0; i < 10; ++i)
    src.bytes[i] = (unsigned char)(i * 17 + 3);

  memcpy(&dst, &src, sizeof(dst));
  for (i = 0; i < 10; ++i) {
    if (dst.bytes[i] != src.bytes[i])
      return 1;
  }

  puts("ok");
  return 0;
}
