#include <stdio.h>

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

static int calls;

static v8hi input(void) {
  ++calls;
  return (v8hi){-1, -32768, 1000, 1, 30000, 400, 200, 3};
}

int main(void) {
  v8hi right = {-1, 2, 1000, 1, 30000, 400, 200, 3};
  v8hi product = __builtin_ia32_pmulhuw128(input(), right);
  v4si packed = (v4si)product;
  v4si sum = packed + 2147483647;
  v4si counts = {1, 0, 0, 0};
  v4si shifted = __builtin_ia32_pslld128(packed, counts);
  printf("%d %d %d %d %d %d\n", product[0], product[1], product[2], calls,
         shifted[0] == (packed[0] << 1), sum[0] < 0);
  return 0;
}
