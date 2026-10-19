#include <stdio.h>

union Word {
  int           value;
  unsigned char bytes[4];
};

int main(void) {
  union Word word = {0};
  word.bytes[0]   = 65;
  word.bytes[1]   = 66;
  printf("%d %d\n", word.bytes[0], word.bytes[1]);
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
// CHECK-NEXT: union Word {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     bytes: [u8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut word: Word = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Word = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         word.bytes[0] = 65;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         word.bytes[1] = 66;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", ((unsafe { word.bytes[0] }) as u32) as i32, ((unsafe { word.bytes[1] })
// CHECK-NEXT:         as u32) as i32
// CHECK-NEXT:     );
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
