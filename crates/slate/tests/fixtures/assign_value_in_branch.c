#include <stdio.h>

unsigned long bump(unsigned long *p, int c) {
  return c ? (*p += 2) : (*p += 1);
}

int main() {
  unsigned long x = 10;
  unsigned long a = bump(&x, 1);
  unsigned long b = bump(&x, 0);
  printf("%lu %lu %lu\n", a, b, x);
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
// CHECK-NEXT: fn bump(p: &mut u64, c: i32) -> u64 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = if c != 0 {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: u64 = unsafe { *p };
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: u64 = [[V_1]].wrapping_add(2);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_2]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: u64 = unsafe { *p };
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: u64 = [[V_3]].wrapping_add(1);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = 10;
// CHECK-NEXT:     let a: u64 = bump(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, 1i32);
// CHECK-NEXT:     let b: u64 = bump(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, 0i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lu %lu %lu\n".as_ptr(), a, b, x) };
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
