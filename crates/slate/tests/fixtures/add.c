#include <stdio.h>

// @check-fn-begin
int add(int a, int b) {
  int c = a + b;
  return c;
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  printf("%d\n", add(2, 3));
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let c: i32 = a + b;
// CHECK-NEXT:     c
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", add(2i32, 3i32));
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
