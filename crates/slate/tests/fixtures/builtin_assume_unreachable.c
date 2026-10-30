#include <stdio.h>

static int classify(int x) {
  switch (x & 3) {
  case 0:
    return 10;
  case 1:
    return 11;
  case 2:
    return 12;
  case 3:
    return 13;
  }
  __builtin_unreachable();
}

static int positive_half(int x) {
  __builtin_assume(x > 0);
  return x / 2;
}

static int bump(int *counter) {
  return ++*counter;
}

static unsigned checked(unsigned x) {
  if (x < 100)
    return x * 3;
  __builtin_unreachable();
}

int main(void) {
  int counter = 0;
  int x = 5;
  __builtin_assume(x++ > 0);
  __builtin_assume(bump(&counter) > 0);
  printf("%d %d %d\n", x, counter, positive_half(9));
  for (int i = 0; i < 6; i++)
    printf("%d ", classify(i));
  printf("\n%u\n", checked(7));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT:     fn __builtin_assume([[ARG_0]]: bool);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(x: i32) -> i32 {
// CHECK-NEXT:     match x & 3 {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return 10;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 11;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return 12;
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             return 13;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn positive_half(x: i32) -> i32 {
// CHECK-NEXT:     unsafe { std::hint::assert_unchecked(x > 0) };
// CHECK-NEXT:     x / 2
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(counter: &mut i32) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { *counter }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *counter = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn checked(x: u32) -> u32 {
// CHECK-NEXT:     if x < 100 {
// CHECK-NEXT:         return x.wrapping_mul(3);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let counter: i32 = 0;
// CHECK-NEXT:     let x: i32 = 5;
// CHECK-NEXT:     unsafe { std::hint::assert_unchecked(true) };
// CHECK-NEXT:     unsafe { std::hint::assert_unchecked(true) };
// CHECK-NEXT:     println!("{} {} {}", x, counter, positive_half(9i32));
// CHECK-NEXT:     for i in 0i32..6i32 {
// CHECK-NEXT:         print!("{} ", classify(i));
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"\n%u\n".as_ptr(), checked(7u32)) };
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
