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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn sum_while({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut n: i32 = 0;
// LOWERING-DAG:     let mut total: i32 = 0;
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     n = {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     total = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     i = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = n;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 total = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                 i = {{_v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn sum_while(mut n: i32) -> i32 {
// REWRITES-DAG:     let mut total: i32 = 0;
// REWRITES-DAG:     let mut i: i32 = 1;
// REWRITES-DAG:     while i <= n {
// REWRITES-DAG:         total += i;
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     total
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
