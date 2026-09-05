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
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 6;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = square({{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = cube({{__v[0-9]+}});
// REWRITES-DAG:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: fn main() {
// REWRITES-X86_64-GNU-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), square(6), cube(4)) };
// REWRITES-X86_64-GNU-DAG:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: fn main() {
// REWRITES-AARCH64-GNU-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), square(6), cube(4)) };
// REWRITES-AARCH64-GNU-DAG:     std::process::exit(0 as i32);
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
