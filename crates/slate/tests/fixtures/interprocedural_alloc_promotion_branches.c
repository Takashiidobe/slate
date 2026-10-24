#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int *a(void) { return alloc(); }

int *b() {
  if (1)
    return a();
  return alloc();
}

int *c(void) { return b(); }

int main(void) {
  int *x = NULL;
  x      = c();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn alloc() -> *mut i32 {
// CHECK-NEXT:     (unsafe { malloc(40u64) }) as *mut i32
// CHECK-NEXT: }
// CHECK-NEXT: fn a() -> *mut i32 {
// CHECK-NEXT:     alloc()
// CHECK-NEXT: }
// CHECK-NEXT: fn b() -> *mut i32 {
// CHECK-NEXT:     if 1i32 != 0i32 {
// CHECK-NEXT:         return a();
// CHECK-NEXT:     }
// CHECK-NEXT:     alloc()
// CHECK-NEXT: }
// CHECK-NEXT: fn c() -> *mut i32 {
// CHECK-NEXT:     b()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     x = c();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x.add(0) = 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { * x.add(0) });
// CHECK-NEXT:     unsafe { free(x as *mut ()) };
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
