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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn add(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut c: i32 = a + b;
// CHECK-NEXT:     c
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr() as *const i8, add(2 as i32, 3 as i32)) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
