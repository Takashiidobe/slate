#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int sum3(int a, int b, int c) {
  return a + b + c;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n", sum3(2, 3, 4));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let mut a: i32 = 0;
// LOWERING-DAG: let mut b: i32 = 0;
// LOWERING-DAG: let mut c: i32 = 0;
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: a = {{arg[0-9]+}};
// LOWERING-DAG: b = {{arg[0-9]+}};
// LOWERING-DAG: c = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut a: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut b: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut c: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = a + b + c;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
