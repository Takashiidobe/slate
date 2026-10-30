#include <stdio.h>

int main(void) {
  char        buf[64];
  const char *fmt = "%d-%d";
  sprintf(buf, fmt, 3, 4);
  puts(buf);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf([[ARG_0:arg[0-9]+]]: *mut i8, [[ARG_1:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn puts([[ARG_0]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     let fmt: *const i8 = c"%d-%d".as_ptr();
// CHECK-NEXT:     unsafe { sprintf(buf.0.as_mut_ptr() as *mut i8, fmt, 3i32, 4i32) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(buf.0.as_mut_ptr() as *const i8) };
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
