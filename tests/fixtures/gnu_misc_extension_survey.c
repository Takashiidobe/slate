#include <stdio.h>

extern __thread int counter;
__thread int counter __attribute__((tls_model("global-dynamic"))) = 5;

#pragma GCC visibility push(hidden)
int f$oo(void) { return 1; }
#pragma GCC visibility pop

static const char *pretty(void) { return __PRETTY_FUNCTION__; }

int main(void) {
  if (0)
    goto done;
  printf("%d\n", counter);
  printf("%d\n", f$oo());
  printf("%s\n", pretty());
  printf("%d\n", (int)sizeof(void));
done:
  __attribute__((unused));
  return 0;
}
