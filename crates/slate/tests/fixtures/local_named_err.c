#include <stdio.h>

static const char *describe(int code) {
  const char *Err;
  if (code == 0) {
    Err = "ok";
  } else {
    Err = "bad";
  }
  return Err;
}

static int pick(int Ok, int None) {
  int Some = Ok + None;
  return Some * 2;
}

int main(void) {
  printf("%s\n", describe(0));
  printf("%s\n", describe(1));
  printf("%d\n", pick(3, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn describe(code: i32) -> *const i8 {
// CHECK-NEXT:     let mut [[ERR_0:Err_[0-9]+]]: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if code == 0 {
// CHECK-NEXT:         [[ERR_0]] = c"ok".as_ptr();
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[ERR_0]] = c"bad".as_ptr();
// CHECK-NEXT:     }
// CHECK-NEXT:     [[ERR_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn pick([[OK_0:Ok_[0-9]+]]: i32, [[NONE_0:None_[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let [[SOME_0:Some_[0-9]+]]: i32 = [[OK_0]] + [[NONE_0]];
// CHECK-NEXT:     [[SOME_0]] * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr(), describe(0i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr(), describe(1i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", pick(3i32, 4i32));
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
