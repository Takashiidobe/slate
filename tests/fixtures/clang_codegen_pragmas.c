#include <stdio.h>

#pragma clang optimize off
static int add_noopt(int a, int b) { return a + b; }
#pragma clang optimize on

#pragma clang attribute push(__attribute__((visibility("hidden"))), apply_to = function)
int hidden_fn(void) { return 7; }
#pragma clang attribute pop

static int loop_sum(void) {
  int s = 0;
#pragma unroll(4)
#pragma clang loop vectorize(enable) interleave(enable)
  for (int i = 0; i < 8; i++) s += i;
  return s;
}

double fma_like(double a, double b, double c) {
#pragma clang fp contract(on)
  return a * b + c;
}

int main(void) {
  printf("%d %d %d %f\n", add_noopt(1, 2), hidden_fn(), loop_sum(),
         fma_like(2.0, 3.0, 4.0));
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
// LOWERING-NEXT: fn hidden_fn() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 7;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fma_like(arg2: f64, arg3: f64, arg4: f64) -> f64 {
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let mut c: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     a = arg2;
// LOWERING-NEXT:     b = arg3;
// LOWERING-NEXT:     c = arg4;
// LOWERING-NEXT:     let _v0: f64 = a;
// LOWERING-NEXT:     let _v1: f64 = b;
// LOWERING-NEXT:     let _v2: f64 = c;
// LOWERING-NEXT:     let _v3: f64 = _v0 * _v1 + _v2;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: f64 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(never)]
// LOWERING-NEXT: fn add_noopt(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn loop_sum() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut s: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     s = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 8;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v5: i32 = i;
// LOWERING-NEXT:             let _v6: i32 = s;
// LOWERING-NEXT:             let _v7: i32 = _v6 + _v5;
// LOWERING-NEXT:             s = _v7;
// LOWERING-NEXT:             let _v8: i32 = i;
// LOWERING-NEXT:             let _v9: i32 = _v8 + 1;
// LOWERING-NEXT:             i = _v9;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: i32 = s;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     return _v11;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: i32 = 2;
// LOWERING-NEXT:     let _v4: i32 = add_noopt(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = hidden_fn();
// LOWERING-NEXT:     let _v6: i32 = loop_sum();
// LOWERING-NEXT:     let _v7: f64 = 2.0;
// LOWERING-NEXT:     let _v8: f64 = 3.0;
// LOWERING-NEXT:     let _v9: f64 = 4.0;
// LOWERING-NEXT:     let _v10: f64 = fma_like(_v7, _v8, _v9);
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v1 as *const i8, _v4, _v5, _v6, _v10) };
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
// REWRITES-NEXT: fn hidden_fn() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 7;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fma_like(arg2: f64, arg3: f64, arg4: f64) -> f64 {
// REWRITES-NEXT: let mut a: f64 = 0.0;
// REWRITES-NEXT: let mut b: f64 = 0.0;
// REWRITES-NEXT: let mut c: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: a = arg2;
// REWRITES-NEXT: b = arg3;
// REWRITES-NEXT: c = arg4;
// REWRITES-NEXT: __retval = a * b + c;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(never)]
// REWRITES-NEXT: fn add_noopt(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn loop_sum() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut s: i32 = 0;
// REWRITES-NEXT: s = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 8;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     s = s + i;
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = s;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 1;
// REWRITES-NEXT: let _v3: i32 = 2;
// REWRITES-NEXT: let _v4: i32 = add_noopt(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = hidden_fn();
// REWRITES-NEXT: let _v6: i32 = loop_sum();
// REWRITES-NEXT: let _v7: f64 = 2.0;
// REWRITES-NEXT: let _v8: f64 = 3.0;
// REWRITES-NEXT: let _v9: f64 = 4.0;
// REWRITES-NEXT: let _v10: f64 = fma_like(_v7, _v8, _v9);
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v1 as *const i8, _v4, _v5, _v6, _v10) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
