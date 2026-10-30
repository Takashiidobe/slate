#include <stdio.h>

typedef int *pointer_returning_fn(int *);

struct Callback {
  pointer_returning_fn *handler;
};

static int *add_one(int *value) {
  *value += 1;
  return value;
}

int main(void) {
  int             value    = 41;
  struct Callback callback = {0};
  callback.handler         = add_one;
  printf("%d\n", *callback.handler(&value));
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Callback {
// CHECK-NEXT:     handler: Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32>,
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn add_one(value: *mut i32) -> *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *value = *value + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 41;
// CHECK-NEXT:     let mut callback: Callback = Callback { handler: None };
// CHECK-NEXT:     callback.handler = Some(add_one);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { * callback.handler.unwrap() (std::ptr::addr_of_mut!(value)) }
// CHECK-NEXT:     );
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
