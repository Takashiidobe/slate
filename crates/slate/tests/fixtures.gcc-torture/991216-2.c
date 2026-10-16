#include <stdarg.h>

void abort(void);
void exit(int);

#define VALUE 0x123456789abcdefLL
#define AFTER 0x55

void test(int n, ...) {
  va_list ap;
  int     i;

  va_start(ap, n);
  for (i = 2; i <= n; i++) {
    if (va_arg(ap, int) != i)
      abort();
  }

  if (va_arg(ap, long long) != VALUE)
    abort();

  if (va_arg(ap, int) != AFTER)
    abort();

  va_end(ap);
}

int main() {
  test(1, VALUE, AFTER);
  test(2, 2, VALUE, AFTER);
  test(3, 2, 3, VALUE, AFTER);
  test(4, 2, 3, 4, VALUE, AFTER);
  test(5, 2, 3, 4, 5, VALUE, AFTER);
  test(6, 2, 3, 4, 5, 6, VALUE, AFTER);
  test(7, 2, 3, 4, 5, 6, 7, VALUE, AFTER);
  test(8, 2, 3, 4, 5, 6, 7, 8, VALUE, AFTER);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn test(mut n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 2 as i32;
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= n {
// CHECK-NEXT:         if (unsafe { ap.next_arg::<i32>() }) != i {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i64>() }) != (81985529216486895 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (85 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { test(1 as i32, 81985529216486895 as i64, 85 as i32) };
// CHECK-NEXT:     unsafe { test(2 as i32, 2 as i32, 81985529216486895 as i64, 85 as i32) };
// CHECK-NEXT:     unsafe { test(3 as i32, 2 as i32, 3 as i32, 81985529216486895 as i64, 85 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(4 as i32, 2 as i32, 3 as i32, 4 as i32, 81985529216486895 as i64, 85 as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             81985529216486895 as i64,
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             81985529216486895 as i64,
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             81985529216486895 as i64,
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test(
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             81985529216486895 as i64,
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:         )
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
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
