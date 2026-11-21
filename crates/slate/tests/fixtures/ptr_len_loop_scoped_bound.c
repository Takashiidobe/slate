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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_both(a: &[i32], b: &[i32]) -> i32 {
// CHECK-NEXT:     let alen: i32 = a.len() as i32;
// CHECK-NEXT:     let blen: i32 = b.len() as i32;
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..alen {
// CHECK-NEXT:         sa += a[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..blen {
// CHECK-NEXT:         sb += b[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sa * 100 + sb
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_twice(a: &[i32], first: i32, second: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0..first {
// CHECK-NEXT:         s += a[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..second {
// CHECK-NEXT:         s += a[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[i32; 5]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32, 5i32]);
// CHECK-NEXT:     let mut y: [i32; 2] = [10, 20];
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         sum_both(x.0.as_slice(), y.as_slice()),
// CHECK-NEXT:         sum_both(y.as_slice(), x.0.as_slice())
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         sum_twice(x.0.as_slice(), 5i32, 2i32),
// CHECK-NEXT:         sum_twice(x.0.as_slice(), 2i32, 5i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
