#include <stdio.h>

// @check-fn-begin
int main(void) {
  char buf[4] = "hey";
  printf("%s\n", buf);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 4] = [104, 101, 121, 0];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr(), buf.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
