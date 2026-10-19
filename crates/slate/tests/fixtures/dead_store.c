#include <stdio.h>

static int noisy(int x) {
  printf("noisy %d\n", x);
  return x;
}

// @check-fn-begin
int main(void) {
  int dead = 1 + 2;
  int kept = noisy(5);
  noisy(9);
  printf("%d\n", kept);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let kept: i32 = noisy(5i32);
// CHECK-NEXT:     noisy(9i32);
// CHECK-NEXT:     println!("{}", kept);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
