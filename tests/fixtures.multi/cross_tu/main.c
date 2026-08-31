#include <stdio.h>

int square(int x);
int cube(int x);

// @rewrite-fn-begin
int main(void) {
  printf("%d %d\n", square(6), cube(4));
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = square({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = cube({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: mod math;
// LOWERING-DAG: use crate::math::square;
// LOWERING-DAG: use crate::math::cube;
// LOWERING-NOT: fn square
// LOWERING-NOT: fn cube
