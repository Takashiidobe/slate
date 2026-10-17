#include <stdio.h>

// @check-fn-begin
static int nested(int k) {
  int x = k + 1;
  {
    int x = k * 2;
    printf("%d\n", x);
  }
  return x;
}
// @check-fn-end

// @check-fn-begin
static int mutated_inner(int k) {
  int x = k;
  {
    int x = 3;
    x += k;
    printf("%d\n", x);
  }
  return x * 2;
}
// @check-fn-end

// @check-fn-begin
static int sibling_sums(int n) {
  int total = 0;
  for (int i = 0; i < n; i++)
    total += i;
  for (int j = 0; j < 3; j++) {
    int i = j * 2;
    total += i;
  }
  int i = 7;
  return total + i;
}
// @check-fn-end

// @check-fn-begin
static int shadows_param(int v) {
  int acc = v;
  for (int k = 0; k < 2; k++) {
    int v = k + 10;
    acc += v;
  }
  return acc + v;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d\n", nested(4), mutated_inner(5), sibling_sums(5), shadows_param(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn nested(k: i32) -> i32 {
// CHECK-NEXT:     let x: i32 = k + 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         let x: i32 = k * 2;
// CHECK-NEXT:         println!("{}", x);
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn mutated_inner(k: i32) -> i32 {
// CHECK-NEXT:     let x: i32 = k;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut x: i32 = 3;
// CHECK-NEXT:         x += k;
// CHECK-NEXT:         println!("{}", x);
// CHECK-NEXT:     }
// CHECK-NEXT:     x * 2
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sibling_sums(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     for j in 0 as i32..3 as i32 {
// CHECK-NEXT:         let i: i32 = j * 2;
// CHECK-NEXT:         total += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let i: i32 = 7;
// CHECK-NEXT:     total + i
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shadows_param(v: i32) -> i32 {
// CHECK-NEXT:     let mut acc: i32 = v;
// CHECK-NEXT:     for k in 0 as i32..2 as i32 {
// CHECK-NEXT:         let v: i32 = k + 10;
// CHECK-NEXT:         acc += v;
// CHECK-NEXT:     }
// CHECK-NEXT:     acc + v
// CHECK: {{^}}}
// SLATE-FILECHECK-END
