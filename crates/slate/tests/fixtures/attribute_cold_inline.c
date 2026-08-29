#include <stdio.h>

__attribute__((cold)) static int cold_path(int x) { return x * 2; }

__attribute__((always_inline)) static int always_inlined(int x) { return x + 1; }

__attribute__((noinline)) static int never_inlined(int x) { return x - 1; }

int main(void) {
  int a = cold_path(3);
  int b = always_inlined(a);
  int c = never_inlined(b);
  printf("%d %d %d\n", a, b, c);
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
// LOWERING-NEXT: #[cold]
// LOWERING-NEXT: fn cold_path(arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg2;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 2;
// LOWERING-NEXT:     let _v2: i32 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn always_inlined(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn never_inlined(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = _v0 - _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 3;
// LOWERING-NEXT:     let _v2: i32 = cold_path(_v1);
// LOWERING-NEXT:     a = _v2;
// LOWERING-NEXT:     let _v3: i32 = a;
// LOWERING-NEXT:     let _v4: i32 = always_inlined(_v3);
// LOWERING-NEXT:     b = _v4;
// LOWERING-NEXT:     let _v5: i32 = b;
// LOWERING-NEXT:     let _v6: i32 = never_inlined(_v5);
// LOWERING-NEXT:     c = _v6;
// LOWERING-NEXT:     let _v7: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = a;
// LOWERING-NEXT:     let _v9: i32 = b;
// LOWERING-NEXT:     let _v10: i32 = c;
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v7 as *const i8, _v8, _v9, _v10) };
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v13 as i32);
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
// REWRITES-NEXT: #[cold]
// REWRITES-NEXT: fn cold_path(arg2: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 2;
// REWRITES-NEXT: __retval = x * _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn always_inlined(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: __retval = x + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn never_inlined(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: __retval = x - _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut b: i32 = 0;
// REWRITES-NEXT: let mut c: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 3;
// REWRITES-NEXT: a = cold_path(_v1);
// REWRITES-NEXT: b = always_inlined(a);
// REWRITES-NEXT: c = never_inlined(b);
// REWRITES-NEXT: let _v7: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v7 as *const i8, a, b, c) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
