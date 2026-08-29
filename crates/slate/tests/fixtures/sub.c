#include <stdio.h>

static int isub(int a, int b) { return a - b; }

int main(void) {
  printf("%d\n", isub(10, 25));
  unsigned int u = 3u;
  unsigned int w = 10u;
  printf("%u\n", u - w);
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
// LOWERING-NEXT: fn isub(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 - _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut w: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 10;
// LOWERING-NEXT:     let _v3: i32 = 25;
// LOWERING-NEXT:     let _v4: i32 = isub(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: u32 = 3;
// LOWERING-NEXT:     u = _v6;
// LOWERING-NEXT:     let _v7: u32 = 10;
// LOWERING-NEXT:     w = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: u32 = u;
// LOWERING-NEXT:     let _v10: u32 = w;
// LOWERING-NEXT:     let _v11: u32 = _v9 - _v10;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v8 as *const i8, _v11) };
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
// REWRITES-NEXT: fn isub(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a - b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut w: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 10;
// REWRITES-NEXT: let _v3: i32 = 25;
// REWRITES-NEXT: let _v4: i32 = isub(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: u = 3;
// REWRITES-NEXT: w = 10;
// REWRITES-NEXT: let _v8: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v8 as *const i8, u - w) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
