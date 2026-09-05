#include <stdio.h>

int add(int a, int b) { return a + b; }
int side_effect(void) {
  printf("effect\n");
  return 1;
}

// @rewrite-fn-begin
// @lowering-fn-begin
int safe_forward(int a, int b, int c, int d) {
  int r    = add(a, b);
  int kept = c + d;
  printf("%d\n", r);
  return kept + kept;
}
// @lowering-fn-end
// @rewrite-fn-end

// @rewrite-fn-begin
// @lowering-fn-begin
void blocked_forward(int a, int b) {
  int r = add(a, b);
  printf("%d %d\n", r, side_effect());
}
// @lowering-fn-end
// @rewrite-fn-end

int main(void) {
  printf("%d\n", safe_forward(2, 3, 4, 5));
  blocked_forward(6, 7);
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn safe_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = add({{arg[0-9]+}}, {{arg[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn blocked_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = add({{arg[0-9]+}}, {{arg[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = side_effect();
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:     return;
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn safe_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), add({{arg[0-9]+}}, {{arg[0-9]+}})) };
// COMMON-REWRITES-DAG:     {{__v[0-9]+}} + {{__v[0-9]+}}
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn blocked_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), add({{arg[0-9]+}}, {{arg[0-9]+}}), side_effect()) };
// COMMON-REWRITES-DAG:     return;
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
