#include <stdio.h>

int main(void) {
  unsigned int  value = 48879u;
  unsigned int  mask  = 255u;
  unsigned long wide  = 4095ul;
  unsigned int  zero  = 0u;
  printf("%#x %#X %#o\n", value, mask, mask);
  printf("%#08x|%-#10X|%#12lo\n", mask, mask, wide);
  printf("%#x %#X %#o %#08x\n", zero, zero, zero, zero);
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
// LOWERING-NEXT:     let mut zero: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u32 = 48879;
// LOWERING-NEXT:     value = _v1;
// LOWERING-NEXT:     let _v2: u32 = 255;
// LOWERING-NEXT:     mask = _v2;
// LOWERING-NEXT:     let _v3: u64 = 4095;
// LOWERING-NEXT:     wide = _v3;
// LOWERING-NEXT:     let _v4: u32 = 0;
// LOWERING-NEXT:     zero = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%#x %#X %#o\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: u32 = value;
// LOWERING-NEXT:     let _v7: u32 = mask;
// LOWERING-NEXT:     let _v8: u32 = mask;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v5 as *const i8, _v6, _v7, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"%#08x|%-#10X|%#12lo\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: u32 = mask;
// LOWERING-NEXT:     let _v12: u32 = mask;
// LOWERING-NEXT:     let _v13: u64 = wide;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v10 as *const i8, _v11, _v12, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%#x %#X %#o %#08x\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: u32 = zero;
// LOWERING-NEXT:     let _v17: u32 = zero;
// LOWERING-NEXT:     let _v18: u32 = zero;
// LOWERING-NEXT:     let _v19: u32 = zero;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v15 as *const i8, _v16, _v17, _v18, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v22: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v22 as i32);
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
// REWRITES-NEXT: let mut zero: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: value = 48879;
// REWRITES-NEXT: mask = 255;
// REWRITES-NEXT: wide = 4095;
// REWRITES-NEXT: zero = 0;
// REWRITES-NEXT: let _v5: *mut i8 = b"%#x %#X %#o\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v5 as *const i8, value, mask, mask) };
// REWRITES-NEXT: let _v10: *mut i8 = b"%#08x|%-#10X|%#12lo\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v10 as *const i8, mask, mask, wide) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%#x %#X %#o %#08x\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v15 as *const i8, zero, zero, zero, zero) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
