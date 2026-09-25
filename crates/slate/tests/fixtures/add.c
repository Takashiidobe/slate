#include <stdio.h>

// @rewrite-fn-begin
// @lowering-fn-begin
// @slate-lowerer-fn-begin
int add(int a, int b) {
  int c = a + b;
  return c;
}
// @slate-lowerer-fn-end
// @lowering-fn-end
// @rewrite-fn-end

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  printf("%d\n", add(2, 3));
  // @lowering-end
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = add({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-DAG: }
// REWRITES-DAG: println!("{}", add(2, 3));
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// SLATE-FILECHECK-END rewrites

// SLATE-REWRITES-DAG: fn add(mut a: i32, mut b: i32) -> i32 {
// SLATE-REWRITES-DAG: let mut c: i32 = a + b;
// SLATE-REWRITES-DAG: printf(c"%d\n".as_ptr() as *const i8, add(2 as i32, 3 as i32))

// SLATE-FILECHECK-BEGIN slate-lowerer
// SLATE-LOWERER-DAG: fn add(mut a: i32, mut b: i32) -> i32 {
// SLATE-LOWERER-DAG:     let mut c: i32 = a + b;
// SLATE-LOWERER-DAG:     return c;
// SLATE-LOWERER-DAG: }
// SLATE-FILECHECK-END slate-lowerer
