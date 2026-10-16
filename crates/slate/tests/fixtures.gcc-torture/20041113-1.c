#include <stdarg.h>

void abort(void);
void exit(int);

void test(int x, ...) {
  va_list ap;
  int     i;
  va_start(ap, x);
  if (va_arg(ap, int) != 1)
    abort();
  if (va_arg(ap, int) != 2)
    abort();
  if (va_arg(ap, int) != 3)
    abort();
  if (va_arg(ap, int) != 4)
    abort();
}

double a = 40.0;

int main(int argc, char *argv[]) {
  test(0, 1, 2, 3, (int)(a / 10.0));
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn test(mut x: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: f64 = 40.0f64;
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(0 as i32, 1 as i32, 2 as i32, 3 as i32, ((unsafe { a }) / 10.0f64) as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
