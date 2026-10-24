#include <stdio.h>

static int hits = 0;

static int mark(int value) {
  hits += 1;
  return value;
}

static int logical_and(int a, int b) { return a && mark(b); }

static int logical_or(int a, int b) { return a || mark(b); }

int main(void) {
  hits = 0;
  printf("%d %d\n", logical_and(0, 1), hits);
  hits = 0;
  printf("%d %d\n", logical_and(2, 3), hits);
  hits = 0;
  printf("%d %d\n", logical_or(5, 0), hits);
  hits = 0;
  printf("%d %d\n", logical_or(0, 7), hits);
  printf("%d %d %d\n", !0, !4, !!9);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut hits: i32 = 0;
// CHECK-NEXT: fn mark(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = hits + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn logical_and(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         [[V_0]] = mark(b) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn logical_or(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         [[V_1]] = true;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = mark(b) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]] as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", logical_and(0i32, 1i32), unsafe { hits });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", logical_and(2i32, 3i32), unsafe { hits });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", logical_or(5i32, 0i32), unsafe { hits });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", logical_or(0i32, 7i32), unsafe { hits });
// CHECK-NEXT:     println!("{} {} {}", true as i32, false as i32, true as i32);
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
