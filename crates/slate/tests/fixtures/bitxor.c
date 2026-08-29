#include <stdio.h>

static int ixor(int a, int b) { return a ^ b; }

int main(void) {
  printf("%d\n", ixor(12, 10));
  printf("%d\n", ixor(-1, 6));
  unsigned int u = 0xFFu;
  printf("%u\n", u ^ 0x0Fu);
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
// LOWERING-NEXT: fn ixor(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 ^ _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 12;
// LOWERING-NEXT:     let _v3: i32 = 10;
// LOWERING-NEXT:     let _v4: i32 = ixor(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = -1;
// LOWERING-NEXT:     let _v8: i32 = 6;
// LOWERING-NEXT:     let _v9: i32 = ixor(_v7, _v8);
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: u32 = 255;
// LOWERING-NEXT:     u = _v11;
// LOWERING-NEXT:     let _v12: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: u32 = u;
// LOWERING-NEXT:     let _v14: u32 = 15;
// LOWERING-NEXT:     let _v15: u32 = _v13 ^ _v14;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v12 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: i32 = 0;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v18 as i32);
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
// REWRITES-NEXT: fn ixor(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a ^ b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 12;
// REWRITES-NEXT: let _v3: i32 = 10;
// REWRITES-NEXT: let _v4: i32 = ixor(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = -1;
// REWRITES-NEXT: let _v8: i32 = 6;
// REWRITES-NEXT: let _v9: i32 = ixor(_v7, _v8);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// REWRITES-NEXT: u = 255;
// REWRITES-NEXT: let _v12: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: u32 = 15;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v12 as *const i8, u ^ _v14) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
