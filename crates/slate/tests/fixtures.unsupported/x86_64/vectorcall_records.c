#include <stdio.h>

#define VC __attribute__((vectorcall))

struct r3 {
  long long a;
  int b;
};

static long long VC sum(struct r3 r) { return r.a + r.b; }

typedef long long(VC *sum_fn)(struct r3);

int main(void) {
  struct r3 r = {40, 2};
  sum_fn fp = sum;
  printf("%lld\n", sum(r));
  printf("%lld\n", fp(r));
  return 0;
}
