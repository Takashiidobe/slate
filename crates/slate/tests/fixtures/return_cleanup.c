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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn choose({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut value: i32 = 0;
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     value = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn choose(mut value: i32) -> i32 {
// COMMON-REWRITES-DAG:     if value < 0 {
// COMMON-REWRITES-DAG:         return -1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     value + 2
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
