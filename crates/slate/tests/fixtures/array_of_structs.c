#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point ps[2];
  ps[0].x = 1;
  ps[0].y = 2;
  ps[1].x = 3;
  ps[1].y = 4;
  printf("%d\n", ps[0].x + ps[1].y);

  struct Point init[2] = {{10, 20}, {30, 40}};
  printf("%d\n", init[0].y + init[1].x);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ps: __SlateAlign16<[Point; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     ps.0[0].x = 1;
// CHECK-NEXT:     ps.0[0].y = 2;
// CHECK-NEXT:     ps.0[1].x = 3;
// CHECK-NEXT:     ps.0[1].y = 4;
// CHECK-NEXT:     println!("{}", ps.0[0].x + ps.0[1].y);
// CHECK-NEXT:     let init: __SlateAlign16<[Point; 2]> = __SlateAlign16([
// CHECK-NEXT:         Point { x: 10, y: 20 },
// CHECK-NEXT:         Point { x: 30, y: 40 },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     println!("{}", init.0[0].y + init.0[1].x);
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
