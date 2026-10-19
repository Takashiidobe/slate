#include <stdio.h>

static int sum_both(const int *a, int alen, const int *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  return sa * 100 + sb;
}

static int sum_twice(const int *a, int first, int second) {
  int s = 0;
  for (int i = 0; i < first; i++)
    s += a[i];
  for (int i = 0; i < second; i++)
    s += a[i];
  return s;
}

int main(void) {
  int x[] = {1, 2, 3, 4, 5};
  int y[] = {10, 20};
  printf("%d %d\n", sum_both(x, 5, y, 2), sum_both(y, 2, x, 5));
  printf("%d %d\n", sum_twice(x, 5, 2), sum_twice(x, 2, 5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum_both(a: &[i32], b: &[i32]) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         while i < (a.len() as i32) {
// CHECK-NEXT:             sa += unsafe { a[i as usize] };
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < (b.len() as i32) {
// CHECK-NEXT:         sb += unsafe { b[i as usize] };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sa * 100 + sb
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_twice(a: *const i32, first: i32, second: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0..first {
// CHECK-NEXT:         s += unsafe { *a.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..second {
// CHECK-NEXT:         s += unsafe { *a.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let y: [i32; 2] = [10, 20];
// CHECK-NEXT:     println!("{} {}", sum_both(& x.0, & y), sum_both(& y, & x.0));
// CHECK-NEXT:     let __slate_printf_arg0 = sum_twice(
// CHECK-NEXT:         x.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg1 = sum_twice(
// CHECK-NEXT:         x.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
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
