#include <stdio.h>

int main(void) {
  unsigned int  value = 48879u;
  unsigned int  mask  = 255u;
  unsigned long wide  = 511ul;
  printf("%x %X %o\n", value, mask, wide);
  printf("%08x|%-4X|%5o\n", mask, mask, mask);
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
// LOWERING-NEXT:     let mut value: u32 = 0;
// LOWERING-NEXT:     let mut mask: u32 = 0;
// LOWERING-NEXT:     let mut wide: u64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u32 = 48879;
// LOWERING-NEXT:     value = _v1;
// LOWERING-NEXT:     let _v2: u32 = 255;
// LOWERING-NEXT:     mask = _v2;
// LOWERING-NEXT:     let _v3: u64 = 511;
// LOWERING-NEXT:     wide = _v3;
// LOWERING-NEXT:     let _v4: *mut i8 = b"%x %X %o\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: u32 = value;
// LOWERING-NEXT:     let _v6: u32 = mask;
// LOWERING-NEXT:     let _v7: u64 = wide;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v4 as *const i8, _v5, _v6, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%08x|%-4X|%5o\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: u32 = mask;
// LOWERING-NEXT:     let _v11: u32 = mask;
// LOWERING-NEXT:     let _v12: u32 = mask;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11, _v12) };
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v15 as i32);
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
// REWRITES-NEXT: let mut value: u32 = 0;
// REWRITES-NEXT: let mut mask: u32 = 0;
// REWRITES-NEXT: let mut wide: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: value = 48879;
// REWRITES-NEXT: mask = 255;
// REWRITES-NEXT: wide = 511;
// REWRITES-NEXT: let _v4: *mut i8 = b"%x %X %o\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v4 as *const i8, value, mask, wide) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%08x|%-4X|%5o\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v9 as *const i8, mask, mask, mask) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
