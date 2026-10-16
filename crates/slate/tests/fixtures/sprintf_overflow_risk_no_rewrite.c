#include <stdio.h>

// @check-fn-begin
int main(void) {
  char buf[4];
  sprintf(buf, "%d", 7);
  puts(buf);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 4] = [0 as i8; 4];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sprintf(buf.as_mut_ptr() as *mut i8, c"%d".as_ptr() as *const i8, 7 as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(buf.as_mut_ptr() as *const i8) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
