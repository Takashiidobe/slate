#include <stdio.h>

// @slate-lowerer-fn-begin
int add(int a, int b) {
  int c = a + b;
  return c;
}
// @slate-lowerer-fn-end

int main(void) {
  printf("%d\n", add(2, 3));
}

// SLATE-REWRITES-DAG: fn add(mut a: i32, mut b: i32) -> i32 {
// SLATE-REWRITES-DAG: let mut c: i32 = a + b;
// SLATE-REWRITES-DAG: printf(c"%d\n".as_ptr() as *const i8, add(2 as i32, 3 as i32))
