#include <stdio.h>

int add(int a, int b) { return a + b; }
int side_effect(void) {
  printf("effect\n");
  return 1;
}

// @rewrite-fn-begin
// @lowering-fn-begin
int safe_forward(int a, int b, int c, int d) {
  int r = add(a, b);
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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn safe_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = add({{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn blocked_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = add({{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = side_effect();
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: return;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn safe_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG: unsafe { printf(c"%d\n".as_ptr(), add({{arg[0-9]+}}, {{arg[0-9]+}})) };
// REWRITES-DAG: return {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: fn blocked_forward({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// REWRITES-DAG: unsafe { printf(c"%d %d\n".as_ptr(), add({{arg[0-9]+}}, {{arg[0-9]+}}), side_effect()) };
// REWRITES-DAG: return;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
