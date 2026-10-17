#include <stdio.h>

// @check-fn-begin
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
// @check-fn-end

int main() {
  printf("%d\n", classify(2, 5));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn classify(mut a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     while a < b {
// CHECK-NEXT:         a += 1 as i32;
// CHECK-NEXT:         r += 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if a == b {
// CHECK-NEXT:         r += 10 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let t: i32 = if a > b { a - b } else { b - a };
// CHECK-NEXT:     let m: i32 = (a & b) + (a << (1 as i32));
// CHECK-NEXT:     r + t + m
// CHECK: {{^}}}
// SLATE-FILECHECK-END
