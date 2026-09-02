#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int choose(int value) {
  if (value < 0)
    return -1;
  return value + 2;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d\n", choose(-4), choose(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn choose({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut value: i32 = 0;
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     value = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn choose(mut value: i32) -> i32 {
// REWRITES-DAG:     if value < 0 {
// REWRITES-DAG:         return -1;
// REWRITES-DAG:     }
// REWRITES-DAG:     value + 2
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
