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
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: return {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
