#include <stdio.h>
#include <stdlib.h>

static void bump(int *p) { *p = *p + 1; }

static int peek(int *p) { return *p + 1; }

static int use_and_free(int *y) {
  *y    = *y + 1;
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int a = 1;
  bump(&a);

  int b      = 10;
  int peeked = peek(&b);

  int *c = malloc(sizeof(int));
  *c     = 100;
  int v  = use_and_free(c);

  printf("%d %d %d\n", a, peeked, v);
  return a + peeked + v;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(p: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut i32) = (unsafe { *(p as *mut i32) }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn peek(p: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(p as *const i32) }) + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn use_and_free(y: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = (unsafe { *y }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let v: i32 = unsafe { *y };
// CHECK-NEXT:     unsafe { free(y as *mut ()) };
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 1;
// CHECK-NEXT:     bump(unsafe { &mut (*std::ptr::addr_of_mut!(a)) });
// CHECK-NEXT:     let mut b: i32 = 10;
// CHECK-NEXT:     let peeked: i32 = peek(unsafe { &(*std::ptr::addr_of_mut!(b)) });
// CHECK-NEXT:     let c: *mut i32 = (unsafe { malloc(4 as u64) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *c = 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     let v: i32 = use_and_free(c);
// CHECK-NEXT:     println!("{} {} {}", a, peeked, v);
// CHECK-NEXT:     a + peeked + v
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
