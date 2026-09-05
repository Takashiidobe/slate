#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int sum_while(int n) {
  int total = 0;
  int i     = 1;
  while (i <= n) {
    total += i;
    i++;
  }
  return total;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n", sum_while(5));
  printf("%d\n", sum_while(1));
  printf("%d\n", sum_while(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn sum_while({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut n: i32 = 0;
// COMMON-LOWERING-DAG:     let mut total: i32 = 0;
// COMMON-LOWERING-DAG:     let mut i: i32 = 0;
// COMMON-LOWERING-DAG:     n = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:                 i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn sum_while(mut n: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut total: i32 = 0;
// COMMON-REWRITES-DAG:     let mut i: i32 = 1;
// COMMON-REWRITES-DAG:     while i <= n {
// COMMON-REWRITES-DAG:         total += i;
// COMMON-REWRITES-DAG:         i += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     total
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
