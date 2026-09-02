#include <stdio.h>

int square(int x);
int cube(int x);

// @rewrite-fn-begin
int main(void) {
  printf("%d %d\n", square(6), cube(4));
  return 0;
}
// @rewrite-fn-end

// LOWERING-DAG: mod math;
// LOWERING-DAG: use crate::math::square;
// LOWERING-DAG: use crate::math::cube;
// LOWERING-NOT: fn square
// LOWERING-NOT: fn cube

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), square(6), cube(4)) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
