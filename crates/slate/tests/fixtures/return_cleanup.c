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
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = value;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = value;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     __retval = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn choose(mut value: i32) -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = value;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:         __retval = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:         return {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = value;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     __retval = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
