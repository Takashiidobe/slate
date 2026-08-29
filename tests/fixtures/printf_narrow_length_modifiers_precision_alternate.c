#include <stdio.h>

int main(void) {
  int          a = 300;
  int          b = -5;
  unsigned int c = 400;
  printf("%.2hhd %.3hd %#hhx %08hhx\n", a, b, a, a);
  printf("%.4hho %.2hhx %hhu\n", a, a, c);
  printf("%.5hd %#hx\n", b, c);
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
// LOWERING-NEXT:     let mut c: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 300;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: i32 = -5;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: u32 = 400;
// LOWERING-NEXT:     c = _v3;
// LOWERING-NEXT:     let _v4: *mut i8 = b"%.2hhd %.3hd %#hhx %08hhx\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = a;
// LOWERING-NEXT:     let _v6: i32 = b;
// LOWERING-NEXT:     let _v7: i32 = a;
// LOWERING-NEXT:     let _v8: i32 = a;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v4 as *const i8, _v5, _v6, _v7, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"%.4hho %.2hhx %hhu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = a;
// LOWERING-NEXT:     let _v12: i32 = a;
// LOWERING-NEXT:     let _v13: u32 = c;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v10 as *const i8, _v11, _v12, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%.5hd %#hx\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i32 = b;
// LOWERING-NEXT:     let _v17: u32 = c;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v15 as *const i8, _v16, _v17) };
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
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
// REWRITES-NEXT: let mut c: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 300;
// REWRITES-NEXT: b = -5;
// REWRITES-NEXT: c = 400;
// REWRITES-NEXT: let _v4: *mut i8 = b"%.2hhd %.3hd %#hhx %08hhx\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v4 as *const i8, a, b, a, a) };
// REWRITES-NEXT: let _v10: *mut i8 = b"%.4hho %.2hhx %hhu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v10 as *const i8, a, a, c) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%.5hd %#hx\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v15 as *const i8, b, c) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
