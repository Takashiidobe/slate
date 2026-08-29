#include <stdio.h>

int main(void) {
  double        d  = 7.75;
  float         f  = (float)d;
  int           i  = (int)d;
  unsigned int  u  = (unsigned int)-1;
  long          l  = (long)i;
  unsigned char c  = (unsigned char)260;
  _Bool         b0 = (_Bool)0.0;
  _Bool         b1 = (_Bool)-3;
  printf("%f\n", f);
  printf("%d\n", i);
  printf("%u\n", u);
  printf("%ld\n", l);
  printf("%u\n", c);
  printf("%d %d\n", b0, b1);
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
// LOWERING-NEXT:     let mut d: f64 = 0.0;
// LOWERING-NEXT:     let mut f: f32 = 0.0;
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut l: i64 = 0;
// LOWERING-NEXT:     let mut c: u8 = 0;
// LOWERING-NEXT:     let mut b0: bool = false;
// LOWERING-NEXT:     let mut b1: bool = false;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 7.75;
// LOWERING-NEXT:     d = _v1;
// LOWERING-NEXT:     let _v2: f64 = d;
// LOWERING-NEXT:     let _v3: f32 = _v2 as f32;
// LOWERING-NEXT:     f = _v3;
// LOWERING-NEXT:     let _v4: f64 = d;
// LOWERING-NEXT:     let _v5: i32 = _v4 as i32;
// LOWERING-NEXT:     i = _v5;
// LOWERING-NEXT:     let _v6: u32 = 4294967295u32;
// LOWERING-NEXT:     u = _v6;
// LOWERING-NEXT:     let _v7: i32 = i;
// LOWERING-NEXT:     let _v8: i64 = _v7 as i64;
// LOWERING-NEXT:     l = _v8;
// LOWERING-NEXT:     let _v9: u8 = 4;
// LOWERING-NEXT:     c = _v9;
// LOWERING-NEXT:     let _v10: f64 = 0.0;
// LOWERING-NEXT:     let _v11: bool = _v10 != 0.0;
// LOWERING-NEXT:     b0 = _v11;
// LOWERING-NEXT:     let _v12: i32 = -3;
// LOWERING-NEXT:     let _v13: bool = _v12 != 0;
// LOWERING-NEXT:     b1 = _v13;
// LOWERING-NEXT:     let _v14: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: f32 = f;
// LOWERING-NEXT:     let _v16: f64 = _v15 as f64;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v14 as *const i8, _v16) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i32 = i;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v18 as *const i8, _v19) };
// LOWERING-NEXT:     let _v21: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: u32 = u;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v21 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: i64 = l;
// LOWERING-NEXT:     let _v26: i32 = unsafe { printf(_v24 as *const i8, _v25) };
// LOWERING-NEXT:     let _v27: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v28: u8 = c;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = unsafe { printf(_v27 as *const i8, _v29) };
// LOWERING-NEXT:     let _v31: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v32: bool = b0;
// LOWERING-NEXT:     let _v33: i32 = _v32 as i32;
// LOWERING-NEXT:     let _v34: bool = b1;
// LOWERING-NEXT:     let _v35: i32 = _v34 as i32;
// LOWERING-NEXT:     let _v36: i32 = unsafe { printf(_v31 as *const i8, _v33, _v35) };
// LOWERING-NEXT:     let _v37: i32 = 0;
// LOWERING-NEXT:     __retval = _v37;
// LOWERING-NEXT:     let _v38: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v38 as i32);
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
// REWRITES-NEXT: let mut d: f64 = 0.0;
// REWRITES-NEXT: let mut f: f32 = 0.0;
// REWRITES-NEXT: let mut i: i32 = 0;
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut l: i64 = 0;
// REWRITES-NEXT: let mut c: u8 = 0;
// REWRITES-NEXT: let mut b0: bool = false;
// REWRITES-NEXT: let mut b1: bool = false;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: d = 7.75;
// REWRITES-NEXT: f = d as f32;
// REWRITES-NEXT: i = d as i32;
// REWRITES-NEXT: u = 4294967295u32;
// REWRITES-NEXT: l = i as i64;
// REWRITES-NEXT: c = 4;
// REWRITES-NEXT: let _v10: f64 = 0.0;
// REWRITES-NEXT: b0 = _v10 != 0.0;
// REWRITES-NEXT: let _v12: i32 = -3;
// REWRITES-NEXT: b1 = _v12 != 0;
// REWRITES-NEXT: let _v14: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v14 as *const i8, f as f64) };
// REWRITES-NEXT: let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v18 as *const i8, i) };
// REWRITES-NEXT: let _v21: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v21 as *const i8, u) };
// REWRITES-NEXT: let _v24: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v26: i32 = unsafe { printf(_v24 as *const i8, l) };
// REWRITES-NEXT: let _v27: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: i32 = unsafe { printf(_v27 as *const i8, c as i32) };
// REWRITES-NEXT: let _v31: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v36: i32 = unsafe { printf(_v31 as *const i8, b0 as i32, b1 as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
