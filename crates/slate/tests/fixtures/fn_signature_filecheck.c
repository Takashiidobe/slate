#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int sum3(int a, int b, int c) { return a + b + c; }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n", sum3(2, 3, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
