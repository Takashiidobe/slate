/* PR target/34281 */

#include <stdarg.h>

extern void abort(void);

void h(int x, va_list ap) {
  switch (x) {
  case 1:
    if (va_arg(ap, int) != 3 || va_arg(ap, int) != 4)
      abort();
    return;
  case 5:
    if (va_arg(ap, int) != 9 || va_arg(ap, int) != 10)
      abort();
    return;
  default:
    abort();
  }
}

void f1(int i, long long int j, ...) {
  va_list ap;
  va_start(ap, j);
  h(i, ap);
  if (i != 1 || j != 2)
    abort();
  va_end(ap);
}

void f2(int i, int j, int k, long long int l, ...) {
  va_list ap;
  va_start(ap, l);
  h(i, ap);
  if (i != 5 || j != 6 || k != 7 || l != 8)
    abort();
  va_end(ap);
}

int main() {
  f1(1, 2, 3, 4);
  f2(5, 6, 7, 8, 9, 10);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/34281
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn h(mut x: i32, mut ap: core::ffi::VaList<'_>) {
// CHECK-NEXT:     match x {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:             if (unsafe { ap.next_arg::<i32>() }) != (3 as i32) {
// CHECK-NEXT:                 [[V_0]] = true as bool;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_0]] = (unsafe { ap.next_arg::<i32>() }) != (4 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             if [[V_0]] {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             return;
// CHECK-NEXT:         }
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:             if (unsafe { ap.next_arg::<i32>() }) != (9 as i32) {
// CHECK-NEXT:                 [[V_1]] = true as bool;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_1]] = (unsafe { ap.next_arg::<i32>() }) != (10 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             if [[V_1]] {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             return;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f1(mut i: i32, mut j: i64, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     h(i, ap.clone());
// CHECK-NEXT:     if i != (1 as i32) || j != ((2 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f2(mut i: i32, mut j: i32, mut k: i32, mut l: i64, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     h(i, ap.clone());
// CHECK-NEXT:     if i != (5 as i32) || j != (6 as i32) || k != (7 as i32) || l != ((8 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { f1(1 as i32, (2 as i32) as i64, 3 as i32, 4 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f2(
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             (8 as i32) as i64,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
