/* Test va_arg when the result is ignored and only the pointer increment
   side effect is used.  */
#include <stdarg.h>

void abort(void);
void exit(int);

static int foo(int a, ...) {
  va_list va;
  int     i, res;

  va_start(va, a);

  for (i = 0; i < 4; ++i)
    (void)va_arg(va, int);

  res = va_arg(va, int);

  va_end(va);

  return res;
}

int main(void) {
  if (foo(5, 4, 3, 2, 1, 0))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test va_arg when the result is ignored and only the pointer increment
// CHECK-NEXT: //! side effect is used.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn foo(mut a: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut va: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut res: i32 = 0 as i32;
// CHECK-NEXT:     va = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         unsafe { va.next_arg::<i32>() };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     res = unsafe { va.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     res
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { foo(5 as i32, 4 as i32, 3 as i32, 2 as i32, 1 as i32, 0 as i32) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
