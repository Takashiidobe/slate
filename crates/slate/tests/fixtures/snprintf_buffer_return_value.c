#include <stdio.h>

int main(void) {
  char buf[64];
  int  n = snprintf(buf, sizeof(buf), "%d-%d", 3, 4);
  printf("n=%d buf=%s\n", n, buf);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn snprintf(__s: *mut i8, __maxlen: u64, __format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     let n: i32 = unsafe {
// CHECK-NEXT:         snprintf(
// CHECK-NEXT:             buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:             c"%d-%d".as_ptr(),
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"n=%d buf=%s\n".as_ptr(), n, buf.0.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
