#include <stdio.h>

int main(void) {
  int x = 0;
  x     = 42;

  int y = 0;
  int z = getchar();
  y     = z + 1;

  int fused;
  fused = z + 2;

  printf("%d %d %d %d\n", x, y, z, fused);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::getchar;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0;
// CHECK-NEXT:     x = 42;
// CHECK-NEXT:     let mut y: i32 = 0;
// CHECK-NEXT:     let z: i32 = unsafe { getchar() as i32 };
// CHECK-NEXT:     y = z + 1;
// CHECK-NEXT:     let fused: i32 = z + 2;
// CHECK-NEXT:     println!("{} {} {} {}", x, y, z, fused);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
