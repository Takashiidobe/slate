#include <stdarg.h>

extern void abort(void);
long        x, y;

inline void __attribute__((always_inline)) f1i(va_list ap) {
  x  = va_arg(ap, double);
  x += va_arg(ap, long);
  x += va_arg(ap, double);
}

void f1(int i, ...) {
  va_list ap;
  va_start(ap, i);
  f1i(ap);
  va_end(ap);
}

inline void __attribute__((always_inline)) f2i(va_list ap) {
  y  = va_arg(ap, int);
  y += va_arg(ap, long);
  y += va_arg(ap, double);
  f1i(ap);
}

void f2(int i, ...) {
  va_list ap;
  va_start(ap, i);
  f2i(ap);
  va_end(ap);
}

long f3h(int i, long arg0, long arg1, long arg2, long arg3) {
  return i + arg0 + arg1 + arg2 + arg3;
}

long f3(int i, ...) {
  long    t, arg0, arg1, arg2, arg3;
  va_list ap;

  va_start(ap, i);
  switch (i) {
  case 0:
    t = f3h(i, 0, 0, 0, 0);
    break;
  case 1:
    arg0 = va_arg(ap, long);
    t    = f3h(i, arg0, 0, 0, 0);
    break;
  case 2:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, 0, 0);
    break;
  case 3:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    arg2 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, arg2, 0);
    break;
  case 4:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    arg2 = va_arg(ap, long);
    arg3 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, arg2, arg3);
    break;
  default:
    abort();
  }
  va_end(ap);

  return t;
}

void f4(int i, ...) {
  va_list ap;

  va_start(ap, i);
  switch (i) {
  case 4:
    y = va_arg(ap, double);
    break;
  case 5:
    y  = va_arg(ap, double);
    y += va_arg(ap, double);
    break;
  default:
    abort();
  }
  f1i(ap);
  va_end(ap);
}

int main(void) {
  f1(3, 16.0, 128L, 32.0);
  if (x != 176L)
    abort();
  f2(6, 5, 7L, 18.0, 19.0, 17L, 64.0);
  if (x != 100L || y != 30L)
    abort();
  if (f3(0) != 0)
    abort();
  if (f3(1, 18L) != 19L)
    abort();
  if (f3(2, 18L, 100L) != 120L)
    abort();
  if (f3(3, 18L, 100L, 300L) != 421L)
    abort();
  if (f3(4, 18L, 71L, 64L, 86L) != 243L)
    abort();
  f4(4, 6.0, 9.0, 16L, 18.0);
  if (x != 43L || y != 6L)
    abort();
  f4(5, 7.0, 21.0, 1.0, 17L, 126.0);
  if (x != 144L || y != 28L)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut y: i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f1i(mut ap: core::ffi::VaList<'_>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = (unsafe { ap.next_arg::<f64>() }) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64 = unsafe { x };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] + unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = unsafe { x };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = (([[V_2]] as f64) + unsafe { ap.next_arg::<f64>() }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f1(mut i: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     f1i(ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn f2i(mut ap: core::ffi::VaList<'_>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = (unsafe { ap.next_arg::<i32>() }) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = unsafe { y };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i64 = [[V_4]] + unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i64 = unsafe { y };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i64 = (([[V_6]] as f64) + unsafe { ap.next_arg::<f64>() }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     f1i(ap.clone());
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f2(mut i: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     f2i(ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn f3h(mut i: i32, mut arg0: i64, mut arg1: i64, mut arg2: i64, mut arg3: i64) -> i64 {
// CHECK-NEXT:     (i as i64) + arg0 + arg1 + arg2 + arg3
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f3(mut i: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut t: i64 = 0 as i64;
// CHECK-NEXT:     let mut arg0: i64 = 0 as i64;
// CHECK-NEXT:     let mut arg1: i64 = 0 as i64;
// CHECK-NEXT:     let mut arg2: i64 = 0 as i64;
// CHECK-NEXT:     let mut arg3: i64 = 0 as i64;
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match i {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 t = f3h(
// CHECK-NEXT:                     i,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 arg0 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 t = f3h(
// CHECK-NEXT:                     i,
// CHECK-NEXT:                     arg0,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                     (0 as i32) as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 arg0 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg1 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 t = f3h(i, arg0, arg1, (0 as i32) as i64, (0 as i32) as i64);
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 arg0 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg1 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg2 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 t = f3h(i, arg0, arg1, arg2, (0 as i32) as i64);
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 arg0 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg1 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg2 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 arg3 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:                 t = f3h(i, arg0, arg1, arg2, arg3);
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f4(mut i: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match i {
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     y = (unsafe { ap.next_arg::<f64>() }) as i64;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     y = (unsafe { ap.next_arg::<f64>() }) as i64;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_8:__v[0-9]+]]: i64 = unsafe { y };
// CHECK-NEXT:                 let mut [[V_9:__v[0-9]+]]: i64 = (([[V_8]] as f64) + unsafe { ap.next_arg::<f64>() })
// CHECK-NEXT:                     as i64;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     y = [[V_9]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     f1i(ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { f1(3 as i32, 16.0f64, 128 as i64, 32.0f64) };
// CHECK-NEXT:     if (unsafe { x }) != (176 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f2(6 as i32, 5 as i32, 7 as i64, 18.0f64, 19.0f64, 17 as i64, 64.0f64) };
// CHECK-NEXT:     if (unsafe { x }) != (100 as i64) || (unsafe { y }) != (30 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { f3(0 as i32) }) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { f3(1 as i32, 18 as i64) }) != (19 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { f3(2 as i32, 18 as i64, 100 as i64) }) != (120 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { f3(3 as i32, 18 as i64, 100 as i64, 300 as i64) }) != (421 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { f3(4 as i32, 18 as i64, 71 as i64, 64 as i64, 86 as i64) })
// CHECK-NEXT:         != (243 as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f4(4 as i32, 6.0f64, 9.0f64, 16 as i64, 18.0f64) };
// CHECK-NEXT:     if (unsafe { x }) != (43 as i64) || (unsafe { y }) != (6 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f4(5 as i32, 7.0f64, 21.0f64, 1.0f64, 17 as i64, 126.0f64) };
// CHECK-NEXT:     if (unsafe { x }) != (144 as i64) || (unsafe { y }) != (28 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
