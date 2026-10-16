#include <stdio.h>

int real_impl(int x) { return x * 3 + 1; }

extern int alias_impl(int x) __attribute__((alias("real_impl")));

int main(void) {
  printf("%d %d\n", alias_impl(5), real_impl(2));
  return 0;
}
