#include <stdio.h>

struct Buf {
  int data[3];
  int len;
};

int main(void) {
  struct Buf b;
  b.data[0] = 10;
  b.data[1] = 20;
  b.data[2] = 30;
  b.len     = 3;
  printf("%d\n", b.data[1] + b.len);

  struct Buf init = {{1, 2, 3}, 3};
  int        sum  = 0;
  for (int i = 0; i < init.len; i++) {
    sum += init.data[i];
  }
  printf("%d\n", sum);
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
// CHECK-NEXT: struct Buf {
// CHECK-NEXT:     data: [i32; 3],
// CHECK-NEXT:     len: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: Buf = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     b.data[0] = 10;
// CHECK-NEXT:     b.data[1] = 20;
// CHECK-NEXT:     b.data[2] = 30;
// CHECK-NEXT:     b.len = 3;
// CHECK-NEXT:     println!("{}", b.data[1] + b.len);
// CHECK-NEXT:     let init: Buf = Buf { data: [1, 2, 3], len: 3 };
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < init.len {
// CHECK-NEXT:         sum += init.data[i as usize];
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", sum);
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
