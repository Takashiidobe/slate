#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

union FnBox {
  Callback fn;
  void    *ptr;
};

int main(void) {
  union FnBox box;
  box.fn        = add_one;
  void    *slot = box.ptr;
  Callback cb   = (Callback)slot;
  Callback none = (Callback)0;
  printf("%d %d\n", cb(41), none != NULL);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn add_one(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union FnBox {
// CHECK-NEXT:     r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     ptr: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r#box: FnBox = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         r#box.r#fn = Some(add_one);
// CHECK-NEXT:     }
// CHECK-NEXT:     let slot: *mut () = unsafe { r#box.ptr };
// CHECK-NEXT:     let cb: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(slot)
// CHECK-NEXT:     };
// CHECK-NEXT:     let none: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// CHECK-NEXT:     println!("{} {}", unsafe { cb.unwrap() (41 as i32) }, (none != None) as i32);
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
