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
  printf("%d\n", add(-10, 4));
  printf("%d\n", add(0, 0));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: return {{arg[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const i8, add({{_v[0-9]+}}, {{_v[0-9]+}})) };
// SLATE-FILECHECK-END rewrites
