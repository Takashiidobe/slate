#include <stdio.h>

struct Inner {
  int x;
  int y;
};

struct Outer {
  struct Inner a;
  int          z;
};

int main(void) {
  struct Outer o;
  o.a.x = 3;
  o.a.y = 4;
  o.z   = 5;
  printf("%d\n", o.a.x + o.a.y + o.z);

  struct Outer init = {{1, 2}, 3};
  printf("%d\n", init.a.x + init.a.y + init.z);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Outer {
// CHECK-NEXT:     a: Inner,
// CHECK-NEXT:     z: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut o: Outer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     o.a.x = 3;
// CHECK-NEXT:     o.a.y = 4;
// CHECK-NEXT:     o.z = 5;
// CHECK-NEXT:     println!("{}", o.a.x + o.a.y + o.z);
// CHECK-NEXT:     let init: Outer = Outer {
// CHECK-NEXT:         a: Inner { x: 1, y: 2 },
// CHECK-NEXT:         z: 3,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", init.a.x + init.a.y + init.z);
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
