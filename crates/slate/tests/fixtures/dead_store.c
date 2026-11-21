#include <stdio.h>

static int noisy(int x) {
  printf("noisy %d\n", x);
  return x;
}

// @check-fn-begin
int main(void) {
  int dead = 1 + 2;
  int __v99 = 3;
  int kept = noisy(5);
  noisy(9);
  noisy(1) ? noisy(6) : noisy(7);
  noisy(0) || noisy(8);
  printf("%d\n", kept);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let dead: i32 = 1 + 2;
// CHECK-NEXT:     let __v99: i32 = 3;
// CHECK-NEXT:     let kept: i32 = noisy(5i32);
// CHECK-NEXT:     noisy(9i32);
// CHECK-NEXT:     if noisy(1i32) != 0 {
// CHECK-NEXT:         noisy(6i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         noisy(7i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     noisy(0i32) != 0 || noisy(8i32) != 0;
// CHECK-NEXT:     println!("{}", kept);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
