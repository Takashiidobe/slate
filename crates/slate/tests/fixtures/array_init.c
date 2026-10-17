#include <stdio.h>

int main(void) {
  int  a[5]       = {1, 2, 3, 4, 5};
  int  partial[4] = {7, 8};
  char s[6]       = "hello";
  char padded[8]  = "hi";

  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += a[i];
  }
  printf("%d\n", sum);
  printf("%d %d\n", partial[1], partial[3]);
  printf("%s\n", s);
  printf("%s %d\n", padded, padded[4]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let partial: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 7 as i32;
// CHECK-NEXT:         [[T_0]][1] = 8 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut s: [i8; 6] = [104, 101, 108, 108, 111, 0];
// CHECK-NEXT:     let mut padded: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in a.0.iter().copied() {
// CHECK-NEXT:         sum += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", sum);
// CHECK-NEXT:     println!("{} {}", partial.0[1], partial.0[3]);
// CHECK-NEXT:     unsafe { printf(c"%s\n".as_ptr(), s.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %d\n".as_ptr(), padded.as_mut_ptr() as *mut i8, padded[4] as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
