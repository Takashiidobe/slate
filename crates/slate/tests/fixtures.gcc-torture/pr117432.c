/* PR ipa/117432 */

#include <stdarg.h>

long long r;

__attribute__((noipa)) void baz(int tag, ...) {
  va_list ap;
  va_start(ap, tag);
  if (!r)
    r = va_arg(ap, long long);
  else
    r = va_arg(ap, int);
  va_end(ap);
}

void foo(void) { baz(1, -1, 0); }

void bar(void) { baz(1, -1LL, 0); }

__attribute__((noipa)) void qux(...) {
  va_list ap;
  va_start(ap);
  if (!r)
    r = va_arg(ap, long long);
  else
    r = va_arg(ap, int);
  va_end(ap);
}

void corge(void) { qux(-2, 0); }

void fred(void) { qux(-2LL, 0); }

int main() {
  bar();
  if (r != -1LL)
    __builtin_abort();
  foo();
  if (r != -1)
    __builtin_abort();
  r = 0;
  fred();
  if (r != -2LL)
    __builtin_abort();
  corge();
  if (r != -2)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR ipa/117432
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn baz(mut tag: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     if !((unsafe { r }) != (0 as i64)) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             r = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             r = (unsafe { ap.next_arg::<i32>() }) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe { baz(1 as i32, -(1 as i32), 0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     unsafe { baz(1 as i32, -(1 as i64), 0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn qux(mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     if !((unsafe { r }) != (0 as i64)) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             r = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             r = (unsafe { ap.next_arg::<i32>() }) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn corge() {
// CHECK-NEXT:     unsafe { qux(-(2 as i32), 0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fred() {
// CHECK-NEXT:     unsafe { qux(-(2 as i64), 0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar();
// CHECK-NEXT:     if (unsafe { r }) != -(1 as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { r }) != (-(1 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         r = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     fred();
// CHECK-NEXT:     if (unsafe { r }) != -(2 as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     corge();
// CHECK-NEXT:     if (unsafe { r }) != (-(2 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
