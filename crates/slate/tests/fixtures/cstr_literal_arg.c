#include <stdio.h>

// @check-fn-begin
int main(void) {
  puts("hello");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { puts(c"hello".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
