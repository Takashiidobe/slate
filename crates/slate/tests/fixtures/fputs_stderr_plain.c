#include <stdio.h>

// @check-fn-begin
int main(void) {
  fputs("bye\n", stderr);
  return 0;
}
// @check-fn-end

// X86_64-UNKNOWN-LINUX-GNU: {{^}}struct _IO_FILE {

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { fputs(c"bye\n".as_ptr(), stderr) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
