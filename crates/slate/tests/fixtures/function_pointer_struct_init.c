#include <stdio.h>

struct Callback {
  int (*handler)(int);
};

static int add_one(int value) { return value + 1; }

int main(void) {
  struct Callback callback = {add_one};
  printf("%d\n", callback.handler(41));
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Callback {
// CHECK-NEXT:     handler: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn add_one(value: i32) -> i32 {
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut callback: Callback = Callback { handler: Some(add_one) };
// CHECK-NEXT:     println!("{}", unsafe { callback.handler.unwrap() (41 as i32) });
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
