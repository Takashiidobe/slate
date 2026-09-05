#include <stdio.h>

// @rewrite-fn-begin
int classify(int a, int b) {
  int r = 0;
  while (a < b) {
    a = a + 1;
    r = r + 1;
  }
  if (a == b) {
    r = r + 10;
  }
  int t = (a > b) ? (a - b) : (b - a);
  int m = (a & b) + (a << 1);
  return r + t + m;
}
// @rewrite-fn-end

int main() {
  printf("%d\n", classify(2, 5));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn classify(mut a: i32, mut b: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut r: i32 = 0;
// COMMON-REWRITES-DAG:     while a < b {
// COMMON-REWRITES-DAG:         a += 1;
// COMMON-REWRITES-DAG:         r += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     if a == b {
// COMMON-REWRITES-DAG:         r += 10;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if a > b {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = a - b;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = b - a;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-DAG:     r + {{__v[0-9]+}} + ((a & b) + (a << {{__v[0-9]+}}))
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
