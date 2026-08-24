#include <stdio.h>

int main(void) {
  int a    = 5;
  int post = a--;
  int pre  = --a;
  int sum  = a-- + --pre;
  printf("%d %d %d %d\n", a, post, pre, sum);

  unsigned char c = 0;
  c--;
  printf("%u\n", (unsigned)c);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: let post: i32 = a;
// REWRITES-DAG: a -= 1;
// REWRITES-DAG: let mut pre: i32 = a;
// REWRITES-DAG: pre -= 1;
// REWRITES-DAG: let sum: i32 =
// REWRITES-NOT: a = _v
// REWRITES-NOT: pre = _v
// REWRITES: {{^}}}
