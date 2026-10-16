#include <stdarg.h>

void abort(void);
void exit(int);

void foo(va_list v) {
  unsigned long long x = va_arg(v, unsigned long long);
  if (x != 16LL)
    abort();
}

void bar(char c, char d, ...) {
  va_list v;
  va_start(v, d);
  foo(v);
  va_end(v);
}

int main(void) {
  bar(0, 0, 16LL);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut v: core::ffi::VaList<'_>) {
// CHECK-NEXT:     let mut x: u64 = unsafe { v.next_arg::<u64>() };
// CHECK-NEXT:     if x != ((16 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn bar(mut c: i8, mut d: i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut v: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     v = __va_args.clone();
// CHECK-NEXT:     foo(v.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { bar((0 as i32) as i8, (0 as i32) as i8, 16 as i64) };
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
