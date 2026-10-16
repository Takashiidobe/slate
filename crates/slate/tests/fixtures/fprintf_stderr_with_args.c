#include <stdio.h>

// @check-fn-begin
int main(void) {
  fprintf(stderr, "error: %d\n", 42);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fprintf(unsafe { stderr }, c"error: %d\n".as_ptr() as *const i8, 42 as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
