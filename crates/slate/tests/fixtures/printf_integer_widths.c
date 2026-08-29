#include <stdio.h>

int main(void) {
  int          a = 42;
  int          b = 7;
  int          c = -42;
  unsigned int u = 9u;
  long         l = 123L;
  printf("%05d|%-4d|%+d|%5u|%+06ld\n", a, b, c, u, l);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut l: i64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 42;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: i32 = 7;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: i32 = -42;
// LOWERING-NEXT:     c = _v3;
// LOWERING-NEXT:     let _v4: u32 = 9;
// LOWERING-NEXT:     u = _v4;
// LOWERING-NEXT:     let _v5: i64 = 123;
// LOWERING-NEXT:     l = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%05d|%-4d|%+d|%5u|%+06ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = a;
// LOWERING-NEXT:     let _v8: i32 = b;
// LOWERING-NEXT:     let _v9: i32 = c;
// LOWERING-NEXT:     let _v10: u32 = u;
// LOWERING-NEXT:     let _v11: i64 = l;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8, _v9, _v10, _v11) };
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v14 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut b: i32 = 0;
// REWRITES-NEXT: let mut c: i32 = 0;
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut l: i64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 42;
// REWRITES-NEXT: b = 7;
// REWRITES-NEXT: c = -42;
// REWRITES-NEXT: u = 9;
// REWRITES-NEXT: l = 123;
// REWRITES-NEXT: let _v6: *mut i8 = b"%05d|%-4d|%+d|%5u|%+06ld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v6 as *const i8, a, b, c, u, l) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
