#include <stdio.h>

int main(void) {
  int a    = 5;
  int post = a--;
  int pre  = --a;
  int sum  = a-- + --pre;
  printf("%d %d %d %d\n", a, post, pre, sum);

  unsigned char c = 0;
  c--;
  printf("%u\n", (unsigned)c);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 5;
// CHECK-NEXT:     let mut post: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     a = [[V_0]] - 1;
// CHECK-NEXT:     post = [[V_0]];
// CHECK-NEXT:     let mut pre: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = a - 1;
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     pre = [[V_1]];
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     a = [[V_2]] - 1;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = pre - 1;
// CHECK-NEXT:     pre = [[V_3]];
// CHECK-NEXT:     sum = [[V_2]] + [[V_3]];
// CHECK-NEXT:     println!("{} {} {} {}", a, post, pre, sum);
// CHECK-NEXT:     let mut c: u8 = 0;
// CHECK-NEXT:     c = ((((c as u32) as i32) - 1) as i8) as u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), c as u32) };
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
