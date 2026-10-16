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
// CHECK-NEXT:     let mut buf: [i8; 4] = [104 as i8, 101 as i8, 121 as i8, 0 as i8];
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr() as *const i8, buf.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
