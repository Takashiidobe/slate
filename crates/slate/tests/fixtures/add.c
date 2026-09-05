#include <stdio.h>

// @rewrite-fn-begin
// @lowering-fn-begin
int add(int a, int b) {
  int c = a + b;
  return c;
}
// @lowering-fn-end
// @rewrite-fn-end

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  printf("%d\n", add(2, 3));
  // @lowering-end
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-REWRITES-DAG:     {{__v[0-9]+}}
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = 2;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
