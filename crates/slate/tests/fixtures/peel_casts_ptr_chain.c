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
// CHECK-NEXT:     let mut buf: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     let p: *mut i8 = buf.as_mut_ptr();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_bytes(
// CHECK-NEXT:             ((p as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             (8u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { *p.add(0) as i32 }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
