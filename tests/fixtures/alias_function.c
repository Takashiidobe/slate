// COMMON-LABEL: {{^}}fn alias_impl(_0: i32) -> i32 {
// COMMON-DAG: real_impl(_0)
// COMMON: {{^}}}

#include <stdio.h>

int real_impl(int x) { return x * 3 + 1; }

extern int alias_impl(int x) __attribute__((alias("real_impl")));

int call_alias(int x) { return alias_impl(x) + real_impl(x); }

int main(void) {
  printf("%d %d\n", alias_impl(5), call_alias(2));
  return 0;
}
