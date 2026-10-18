#include <stdio.h>

static void fill(int *p, int n, int val) {
  for (int i = 0; i < n; i++) {
    *(p + i) = val;
  }
}

int main(void) {
  int buf[4] = {0};
  fill(buf, 4, 7);
  printf("%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
  return buf[0] + buf[1] + buf[2] + buf[3];
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn fill(p: *mut i32, n: i32, val: i32) {
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p.offset(i as isize) = val;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     fill(buf.0.as_mut_ptr() as *mut i32, 4 as i32, 7 as i32);
// CHECK-NEXT:     println!("{} {} {} {}", buf.0[0], buf.0[1], buf.0[2], buf.0[3]);
// CHECK-NEXT:     buf.0[0] + buf.0[1] + buf.0[2] + buf.0[3]
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
