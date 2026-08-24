#include <stdio.h>

int main(void) {
  int a  = 20;
  a     -= 5;
  printf("%d\n", a);
  a *= 3;
  printf("%d\n", a);
  a /= 5;
  printf("%d\n", a);
  a %= 7;
  printf("%d\n", a);
  a <<= 3;
  printf("%d\n", a);
  a >>= 2;
  printf("%d\n", a);
  a &= 6;
  printf("%d\n", a);
  a ^= 3;
  printf("%d\n", a);
  a |= 8;
  printf("%d\n", a);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: a -= 5;
// REWRITES-DAG: a *= 3;
// REWRITES-DAG: a /= 5;
// REWRITES-DAG: a %= 7;
// REWRITES-DAG: a <<= 3;
// REWRITES-DAG: a >>= 2;
// REWRITES-DAG: a &= 6;
// REWRITES-DAG: a ^= 3;
// REWRITES-DAG: a |= 8;
// REWRITES-DAG: println!("{}", a);
// REWRITES-DAG: let mut a: i32 = 20;
// REWRITES-NOT: println!("{}", _v
// REWRITES-NOT: : i32 = println!(
// REWRITES-NOT: let _v1: i32 = 20;
// REWRITES-NOT: let _v4: i32 = (a - 5);
// REWRITES-NOT: let mut a: i32 = 0;
// REWRITES: {{^}}}
