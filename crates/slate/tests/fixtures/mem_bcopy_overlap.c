#include <stdio.h>

int main(void) {
  char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  __builtin_bcopy(buf, buf + 2, 4);
  for (int i = 0; i < 8; i++)
    printf("%d ", buf[i]);
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "bcopy"]
// CHECK-NEXT:     fn __builtin_bcopy([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: u64);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_bcopy(
// CHECK-NEXT:             buf.as_mut_ptr() as *const (),
// CHECK-NEXT:             (unsafe { (buf.as_mut_ptr() as *mut i8).offset(2 as isize) }) as *mut (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0 as i32..8 as i32 {
// CHECK-NEXT:         print!("{} ", buf[i as usize] as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
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
