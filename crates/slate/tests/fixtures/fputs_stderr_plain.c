#include <stdio.h>

// @check-fn-begin
int main(void) {
  fputs("bye\n", stderr);
  return 0;
}
// @check-fn-end

// X86_64-UNKNOWN-LINUX-GNU: {{^}}struct _IO_FILE {

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { fputs(c"bye\n".as_ptr() as *const i8, unsafe { stderr }) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
