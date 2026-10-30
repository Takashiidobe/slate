#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) void die(int code) {
  printf("dying with %d\n", code);
  exit(code);
}

int main(void) {
  if (0) {
    die(1);
  }
  printf("main\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn exit([[ARG_0]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn die(code: i32) {
// CHECK-NEXT:     println!("dying with {}", code);
// CHECK-NEXT:     unsafe { std::process::exit(code) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if false {
// CHECK-NEXT:         die(1i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("main");
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
