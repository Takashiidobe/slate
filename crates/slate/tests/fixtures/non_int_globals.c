#include <stdio.h>

static char          small = 12;
static unsigned char byte  = 200;
static float         ratio = 1.5f;
static double        total = 2.25;

static char add_char(char a, char b) { return a + b; }

static float scale(float value, float factor) { return value * factor; }

static double add_double(double a, double b) { return a + b; }

int main(void) {
  small = add_char(small, 3);
  byte  = byte + 1;
  ratio = scale(ratio, 2.0f);
  total = add_double(total, ratio);
  printf("%d\n", small);
  printf("%u\n", byte);
  printf("%f\n", ratio);
  printf("%f\n", total);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut byte: u8 = 200;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut ratio: f32 = 1.5;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut small: i8 = 12;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut total: f64 = 2.25;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_char(arg4: i8, arg5: i8) -> i8 {
// LOWERING-NEXT:     let mut a: i8 = 0;
// LOWERING-NEXT:     let mut b: i8 = 0;
// LOWERING-NEXT:     let mut __retval: i8 = 0;
// LOWERING-NEXT:     a = arg4;
// LOWERING-NEXT:     b = arg5;
// LOWERING-NEXT:     let _v0: i8 = a;
// LOWERING-NEXT:     let _v1: i32 = _v0 as i32;
// LOWERING-NEXT:     let _v2: i8 = b;
// LOWERING-NEXT:     let _v3: i32 = _v2 as i32;
// LOWERING-NEXT:     let _v4: i32 = _v1 + _v3;
// LOWERING-NEXT:     let _v5: i8 = _v4 as i8;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i8 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn scale(arg2: f32, arg3: f32) -> f32 {
// LOWERING-NEXT:     let mut value: f32 = 0.0;
// LOWERING-NEXT:     let mut factor: f32 = 0.0;
// LOWERING-NEXT:     let mut __retval: f32 = 0.0;
// LOWERING-NEXT:     value = arg2;
// LOWERING-NEXT:     factor = arg3;
// LOWERING-NEXT:     let _v0: f32 = value;
// LOWERING-NEXT:     let _v1: f32 = factor;
// LOWERING-NEXT:     let _v2: f32 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: f32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_double(arg0: f64, arg1: f64) -> f64 {
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: f64 = a;
// LOWERING-NEXT:     let _v1: f64 = b;
// LOWERING-NEXT:     let _v2: f64 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: f64 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i8 = unsafe { small };
// LOWERING-NEXT:     let _v2: i8 = 3;
// LOWERING-NEXT:     let _v3: i8 = add_char(_v1, _v2);
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         small = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: u8 = unsafe { byte };
// LOWERING-NEXT:     let _v5: i32 = _v4 as i32;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: i32 = _v5 + _v6;
// LOWERING-NEXT:     let _v8: u8 = _v7 as u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         byte = _v8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v9: f32 = unsafe { ratio };
// LOWERING-NEXT:     let _v10: f32 = 2.0;
// LOWERING-NEXT:     let _v11: f32 = scale(_v9, _v10);
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ratio = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: f64 = unsafe { total };
// LOWERING-NEXT:     let _v13: f32 = unsafe { ratio };
// LOWERING-NEXT:     let _v14: f64 = _v13 as f64;
// LOWERING-NEXT:     let _v15: f64 = add_double(_v12, _v14);
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         total = _v15;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: i8 = unsafe { small };
// LOWERING-NEXT:     let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v16 as *const i8, _v18) };
// LOWERING-NEXT:     let _v20: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: u8 = unsafe { byte };
// LOWERING-NEXT:     let _v22: i32 = _v21 as i32;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v20 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: f32 = unsafe { ratio };
// LOWERING-NEXT:     let _v26: f64 = _v25 as f64;
// LOWERING-NEXT:     let _v27: i32 = unsafe { printf(_v24 as *const i8, _v26) };
// LOWERING-NEXT:     let _v28: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: f64 = unsafe { total };
// LOWERING-NEXT:     let _v30: i32 = unsafe { printf(_v28 as *const i8, _v29) };
// LOWERING-NEXT:     let _v31: i32 = 0;
// LOWERING-NEXT:     __retval = _v31;
// LOWERING-NEXT:     let _v32: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v32 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut byte: u8 = 200;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut ratio: f32 = 1.5;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut small: i8 = 12;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut total: f64 = 2.25;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_char(arg4: i8, arg5: i8) -> i8 {
// REWRITES-NEXT: let mut a: i8 = arg4;
// REWRITES-NEXT: let mut b: i8 = arg5;
// REWRITES-NEXT: let mut __retval: i8 = 0;
// REWRITES-NEXT: __retval = ((a as i32) + (b as i32)) as i8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn scale(arg2: f32, arg3: f32) -> f32 {
// REWRITES-NEXT: let mut value: f32 = 0.0;
// REWRITES-NEXT: let mut factor: f32 = 0.0;
// REWRITES-NEXT: let mut __retval: f32 = 0.0;
// REWRITES-NEXT: value = arg2;
// REWRITES-NEXT: factor = arg3;
// REWRITES-NEXT: __retval = value * factor;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_double(arg0: f64, arg1: f64) -> f64 {
// REWRITES-NEXT: let mut a: f64 = 0.0;
// REWRITES-NEXT: let mut b: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: a = arg0;
// REWRITES-NEXT: b = arg1;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v2: i8 = 3;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         small = add_char(unsafe { small }, _v2);
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         byte = (((unsafe { byte }) as i32) + _v6) as u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v10: f32 = 2.0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         ratio = scale(unsafe { ratio }, _v10);
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         total = add_double(unsafe { total }, (unsafe { ratio }) as f64);
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v16 as *const i8, (unsafe { small }) as i32) };
// REWRITES-NEXT: let _v20: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v20 as *const i8, (unsafe { byte }) as i32) };
// REWRITES-NEXT: let _v24: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v27: i32 = unsafe { printf(_v24 as *const i8, (unsafe { ratio }) as f64) };
// REWRITES-NEXT: let _v28: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: i32 = unsafe { printf(_v28 as *const i8, unsafe { total }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
