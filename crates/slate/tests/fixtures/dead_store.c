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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let kept: i32 = noisy(5 as i32);
// CHECK-NEXT:     noisy(9 as i32);
// CHECK-NEXT:     println!("{}", kept);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
