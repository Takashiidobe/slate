#include <stdio.h>

int main(void) {
  int a    = 5;
  int post = a--;
  int pre  = --a;
  int sum  = a-- + --pre;
  printf("%d %d %d %d\n", a, post, pre, sum);

  unsigned char c = 0;
  c--;
  printf("%u\n", (unsigned)c);
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
// LOWERING-NEXT:     let mut post: i32 = 0;
// LOWERING-NEXT:     let mut pre: i32 = 0;
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let mut c: u8 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: i32 = a;
// LOWERING-NEXT:     let _v3: i32 = _v2 - 1;
// LOWERING-NEXT:     a = _v3;
// LOWERING-NEXT:     post = _v2;
// LOWERING-NEXT:     let _v4: i32 = a;
// LOWERING-NEXT:     let _v5: i32 = _v4 - 1;
// LOWERING-NEXT:     a = _v5;
// LOWERING-NEXT:     pre = _v5;
// LOWERING-NEXT:     let _v6: i32 = a;
// LOWERING-NEXT:     let _v7: i32 = _v6 - 1;
// LOWERING-NEXT:     a = _v7;
// LOWERING-NEXT:     let _v8: i32 = pre;
// LOWERING-NEXT:     let _v9: i32 = _v8 - 1;
// LOWERING-NEXT:     pre = _v9;
// LOWERING-NEXT:     let _v10: i32 = _v6 + _v9;
// LOWERING-NEXT:     sum = _v10;
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = a;
// LOWERING-NEXT:     let _v13: i32 = post;
// LOWERING-NEXT:     let _v14: i32 = pre;
// LOWERING-NEXT:     let _v15: i32 = sum;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v11 as *const i8, _v12, _v13, _v14, _v15) };
// LOWERING-NEXT:     let _v17: u8 = 0;
// LOWERING-NEXT:     c = _v17;
// LOWERING-NEXT:     let _v18: u8 = c;
// LOWERING-NEXT:     let _v19: u8 = _v18 - 1;
// LOWERING-NEXT:     c = _v19;
// LOWERING-NEXT:     let _v20: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: u8 = c;
// LOWERING-NEXT:     let _v22: u32 = _v21 as u32;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v20 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
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
// REWRITES-NEXT: let mut post: i32 = 0;
// REWRITES-NEXT: let mut pre: i32 = 0;
// REWRITES-NEXT: let mut sum: i32 = 0;
// REWRITES-NEXT: let mut c: u8 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 5;
// REWRITES-NEXT: let _v2: i32 = a;
// REWRITES-NEXT: a = _v2 - 1;
// REWRITES-NEXT: post = _v2;
// REWRITES-NEXT: let _v5: i32 = a - 1;
// REWRITES-NEXT: a = _v5;
// REWRITES-NEXT: pre = _v5;
// REWRITES-NEXT: let _v6: i32 = a;
// REWRITES-NEXT: a = _v6 - 1;
// REWRITES-NEXT: let _v9: i32 = pre - 1;
// REWRITES-NEXT: pre = _v9;
// REWRITES-NEXT: sum = _v6 + _v9;
// REWRITES-NEXT: let _v11: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v11 as *const i8, a, post, pre, sum) };
// REWRITES-NEXT: c = 0;
// REWRITES-NEXT: c = c - 1;
// REWRITES-NEXT: let _v20: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v20 as *const i8, c as u32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
