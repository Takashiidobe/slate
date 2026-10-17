#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char path[] = "slate-XXXXXX";
  int  fd     = mkstemp(path);
  printf("%d\n", fd >= 0);
  if (fd >= 0) {
    close(fd);
    unlink(path);
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn mkstemp(__template: *mut i8) -> i32;
// CHECK-NEXT:     fn close(__fd: i32) -> i32;
// CHECK-NEXT:     fn unlink(__name: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut path: [i8; 13] = [115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0];
// CHECK-NEXT:     let fd: i32 = unsafe { mkstemp(path.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     println!("{}", (fd >= 0) as i32);
// CHECK-NEXT:     if fd >= 0 {
// CHECK-NEXT:         unsafe { close(fd) };
// CHECK-NEXT:         unsafe { unlink(path.as_mut_ptr() as *const i8) };
// CHECK-NEXT:     }
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
