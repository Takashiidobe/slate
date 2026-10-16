#include <stdio.h>

// @check-fn-begin
int main(void) {
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%5s|%-5s|%.1s|%6.1s\n".as_ptr() as *const i8,
// CHECK-NEXT:             c"hi".as_ptr() as *mut i8,
// CHECK-NEXT:             c"hi".as_ptr() as *mut i8,
// CHECK-NEXT:             c"hi".as_ptr() as *mut i8,
// CHECK-NEXT:             c"hi".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
