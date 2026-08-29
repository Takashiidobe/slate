#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  short          s  = 300;
  unsigned short us = 60000;
  signed char    c  = -5;
  unsigned char  uc = 200;
  intmax_t       j  = 123456789L;
  uintmax_t      ju = 123456789UL;
  ptrdiff_t      t  = -7;
  printf("%hd %hu %hhd %hhu %jd %ju %td\n", s, us, c, uc, j, ju, t);

  int          full_int = 300;
  int          negative = -1;
  unsigned int wide     = 70000;
  printf("%hhd %hhd %hhu %hd %hu\n", full_int, uc, negative, wide, wide);
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
// LOWERING-NEXT:     let mut s: i16 = 0;
// LOWERING-NEXT:     let mut us: u16 = 0;
// LOWERING-NEXT:     let mut c: i8 = 0;
// LOWERING-NEXT:     let mut uc: u8 = 0;
// LOWERING-NEXT:     let mut j: i64 = 0;
// LOWERING-NEXT:     let mut ju: u64 = 0;
// LOWERING-NEXT:     let mut t: i64 = 0;
// LOWERING-NEXT:     let mut full_int: i32 = 0;
// LOWERING-NEXT:     let mut negative: i32 = 0;
// LOWERING-NEXT:     let mut wide: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i16 = 300;
// LOWERING-NEXT:     s = _v1;
// LOWERING-NEXT:     let _v2: u16 = 60000;
// LOWERING-NEXT:     us = _v2;
// LOWERING-NEXT:     let _v3: i8 = -5;
// LOWERING-NEXT:     c = _v3;
// LOWERING-NEXT:     let _v4: u8 = 200;
// LOWERING-NEXT:     uc = _v4;
// LOWERING-NEXT:     let _v5: i64 = 123456789;
// LOWERING-NEXT:     j = _v5;
// LOWERING-NEXT:     let _v6: u64 = 123456789;
// LOWERING-NEXT:     ju = _v6;
// LOWERING-NEXT:     let _v7: i64 = -7;
// LOWERING-NEXT:     t = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = b"%hd %hu %hhd %hhu %jd %ju %td\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: i16 = s;
// LOWERING-NEXT:     let _v10: i32 = _v9 as i32;
// LOWERING-NEXT:     let _v11: u16 = us;
// LOWERING-NEXT:     let _v12: i32 = _v11 as i32;
// LOWERING-NEXT:     let _v13: i8 = c;
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: u8 = uc;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i64 = j;
// LOWERING-NEXT:     let _v18: u64 = ju;
// LOWERING-NEXT:     let _v19: i64 = t;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v8 as *const i8, _v10, _v12, _v14, _v16, _v17, _v18, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 300;
// LOWERING-NEXT:     full_int = _v21;
// LOWERING-NEXT:     let _v22: i32 = -1;
// LOWERING-NEXT:     negative = _v22;
// LOWERING-NEXT:     let _v23: u32 = 70000;
// LOWERING-NEXT:     wide = _v23;
// LOWERING-NEXT:     let _v24: *mut i8 = b"%hhd %hhd %hhu %hd %hu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: i32 = full_int;
// LOWERING-NEXT:     let _v26: u8 = uc;
// LOWERING-NEXT:     let _v27: i32 = _v26 as i32;
// LOWERING-NEXT:     let _v28: i32 = negative;
// LOWERING-NEXT:     let _v29: u32 = wide;
// LOWERING-NEXT:     let _v30: u32 = wide;
// LOWERING-NEXT:     let _v31: i32 = unsafe { printf(_v24 as *const i8, _v25, _v27, _v28, _v29, _v30) };
// LOWERING-NEXT:     let _v32: i32 = 0;
// LOWERING-NEXT:     __retval = _v32;
// LOWERING-NEXT:     let _v33: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v33 as i32);
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
// REWRITES-NEXT: let mut s: i16 = 0;
// REWRITES-NEXT: let mut us: u16 = 0;
// REWRITES-NEXT: let mut c: i8 = 0;
// REWRITES-NEXT: let mut uc: u8 = 0;
// REWRITES-NEXT: let mut j: i64 = 0;
// REWRITES-NEXT: let mut ju: u64 = 0;
// REWRITES-NEXT: let mut t: i64 = 0;
// REWRITES-NEXT: let mut full_int: i32 = 0;
// REWRITES-NEXT: let mut negative: i32 = 0;
// REWRITES-NEXT: let mut wide: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: s = 300;
// REWRITES-NEXT: us = 60000;
// REWRITES-NEXT: c = -5;
// REWRITES-NEXT: uc = 200;
// REWRITES-NEXT: j = 123456789;
// REWRITES-NEXT: ju = 123456789;
// REWRITES-NEXT: t = -7;
// REWRITES-NEXT: let _v8: *mut i8 = b"%hd %hu %hhd %hhu %jd %ju %td\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v8 as *const i8, s as i32, us as i32, c as i32, uc as i32, j, ju, t) };
// REWRITES-NEXT: full_int = 300;
// REWRITES-NEXT: negative = -1;
// REWRITES-NEXT: wide = 70000;
// REWRITES-NEXT: let _v24: *mut i8 = b"%hhd %hhd %hhu %hd %hu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v31: i32 = unsafe { printf(_v24 as *const i8, full_int, uc as i32, negative, wide, wide) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
