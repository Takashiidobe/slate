/* { dg-options "-std=gnu17" } */
#include <stdio.h>

static int counter;
int later();

static int bump() { return ++counter; }

int twice() {
  bump();
  return bump();
}

int main() {
  printf("%d %d\n", twice(), later());
  return counter - 3;
}

int later() { return bump() * 10; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu17" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bump() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { counter }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn twice() -> i32 {
// CHECK-NEXT:     bump();
// CHECK-NEXT:     bump()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", twice(), later());
// CHECK-NEXT:     (unsafe { counter }) - 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn later() -> i32 {
// CHECK-NEXT:     bump() * 10
// CHECK-NEXT: }
// CHECK-EMPTY:
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
