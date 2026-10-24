#include <stdio.h>

struct one {
  int v;
};

static int __attribute__((fastcall)) wide_then_int(long long a, int b) {
  return (int)a + b;
}

static int __attribute__((fastcall)) record_then_ints(struct one r, int a, int b) {
  return r.v + a * 10 + b * 100;
}

int main(void) {
  struct one r = {1};
  printf("%d\n", wide_then_int(40, 2));
  printf("%d\n", record_then_ints(r, 2, 3));
  return 0;
}
