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
  printf("%d\n", add(2, 3));
  printf("%d\n", add(-10, 4));
  printf("%d\n", add(0, 0));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: return {{arg[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
