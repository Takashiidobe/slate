/* PR target/64979 */

#include <stdarg.h>

void __attribute__((noinline, noclone)) bar(int x, va_list *ap) {
  if (ap) {
    int i;
    for (i = 0; i < 10; i++)
      if (i != va_arg(*ap, int))
        __builtin_abort();
    if (va_arg(*ap, double) != 0.5)
      __builtin_abort();
  }
}

void __attribute__((noinline, noclone)) foo(int x, ...) {
  va_list ap;
  int     n;

  va_start(ap, x);
  n = va_arg(ap, int);
  bar(x, (va_list *)((n == 0) ? ((void *)0) : &ap));
  va_end(ap);
}

int main() {
  foo(100, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0.5);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/64979
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32, mut ap: *mut core::ffi::VaList<'_>) {
// CHECK-NEXT:     if ap != std::ptr::null_mut::<core::ffi::VaList<'_>>() {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:             if i != unsafe { (*ap).next_arg::<i32>() } {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { (*ap).next_arg::<f64>() }) != 0.5f64 {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn foo(mut x: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n: i32 = 0 as i32;
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     n = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     bar(
// CHECK-NEXT:         x,
// CHECK-NEXT:         if n == (0 as i32) {
// CHECK-NEXT:             std::ptr::null_mut::<core::ffi::VaList<'_>>()
// CHECK-NEXT:         } else {
// CHECK-NEXT:             std::ptr::addr_of_mut!(ap)
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         foo(
// CHECK-NEXT:             100 as i32, 1 as i32, 0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32,
// CHECK-NEXT:             6 as i32, 7 as i32, 8 as i32, 9 as i32, 0.5f64,
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
