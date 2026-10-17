#include <stdio.h>

int sum3(int a, int b, int c) { return a + b + c; }

// @check-fn-begin
int main(void) {
  printf("%d\n", sum3(2, 3, 4));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum3(2 as i32, 3 as i32, 4 as i32));
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
