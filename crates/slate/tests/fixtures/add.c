#include <stdio.h>

static int add(int a, int b) {
  int c = a + b;
  return c;
}

int main(void) {
  printf("%d\n", add(2, 3));
  printf("%d\n", add(-10, 4));
  printf("%d\n", add(0, 0));
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}", add(2, 3));
// REWRITES-DAG: println!("{}", add(-10, 4));
// REWRITES-DAG: println!("{}", add(0, 0));
// REWRITES-LABEL: {{^}}fn add(a: i32, b: i32) -> i32 {
// REWRITES-DAG: let c: i32 = a + b;
// REWRITES-NOT: fn add(mut a: i32, mut b: i32)
// REWRITES-NOT: let mut c: i32
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: add(2, 3)
// REWRITES-DAG: add(-10, 4)
// REWRITES-DAG: add(0, 0)
// REWRITES-NOT: add(_v
// REWRITES-NOT: let _v1: i32 = 2;
// REWRITES-NOT: std::process::exit(0 as i32);
// REWRITES-NOT: __retval
// REWRITES: {{^}}}
