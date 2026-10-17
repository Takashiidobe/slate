#include <string.h>

// @check-fn-begin
int main(void) {
  char  buf[8];
  char *p = buf;
  memset((void *)p, 0, 8);
  return p[0];
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [0 as i8; 8];
// CHECK-NEXT:     let p: *mut i8 = buf.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     unsafe { buf[0usize..8usize].fill(0 as i8) };
// CHECK-NEXT:     (unsafe { *unsafe { p.offset(0 as isize) } }) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
