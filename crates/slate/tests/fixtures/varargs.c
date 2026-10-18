

#include <stdarg.h>
#include <stdio.h>

static int sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  int total = 0;
  for (int i = 0; i < n; i++) {
    total += va_arg(ap, int);
  }
  va_end(ap);
  return total;
}

static int pick_second(int marker, ...) {
  va_list ap;
  va_start(ap, marker);
  int first  = va_arg(ap, int);
  int second = va_arg(ap, int);
  va_end(ap);
  return marker + first * 10 + second;
}

int main(void) {
  printf("%d\n", sum(4, 10, 20, 30, 40));
  printf("%d\n", pick_second(5, 7, 9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(n: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn pick_second(marker: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let first: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     let second: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     marker + first * 10 + second
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", unsafe { sum(4 as i32, 10 as i32, 20 as i32, 30 as i32, 40 as i32) });
// CHECK-NEXT:     println!("{}", unsafe { pick_second(5 as i32, 7 as i32, 9 as i32) });
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
