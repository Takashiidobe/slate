#include <stdio.h>

struct Triple {
  int x;
  int y;
  int z;
};

int main(void) {
  struct Triple full       = {1, 2, 3};
  struct Triple partial    = {4, 5};
  struct Triple designated = {.z = 9, .x = 7};
  printf("%d %d %d\n", full.x, full.y, full.z);
  printf("%d %d %d\n", partial.x, partial.y, partial.z);
  printf("%d %d %d\n", designated.x, designated.y, designated.z);

  struct Triple copy = full;
  copy.x             = 42;
  printf("%d %d\n", full.x, copy.x);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Triple {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     z: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let full: Triple = Triple { x: 1, y: 2, z: 3 };
// CHECK-NEXT:     let partial: Triple = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Triple = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].x = 4i32;
// CHECK-NEXT:         [[T_0]].y = 5i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let designated: Triple = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Triple = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].x = 7i32;
// CHECK-NEXT:         [[T_1]].z = 9i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {}", full.x, full.y, full.z);
// CHECK-NEXT:     println!("{} {} {}", partial.x, partial.y, partial.z);
// CHECK-NEXT:     println!("{} {} {}", designated.x, designated.y, designated.z);
// CHECK-NEXT:     let mut copy: Triple = full;
// CHECK-NEXT:     copy.x = 42;
// CHECK-NEXT:     println!("{} {}", full.x, copy.x);
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
