#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double y = x;
  double z = -(x * x);
  double contracted;
  double uncontracted;

  {
#pragma STDC FP_CONTRACT ON
    contracted = x * y + z;
  }

  {
#pragma STDC FP_CONTRACT OFF
    uncontracted = x * y + z;
  }

  printf("%.20e %.20e\n", contracted, uncontracted);
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
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut y: f64 = 0.0;
// LOWERING-NEXT:     let mut z: f64 = 0.0;
// LOWERING-NEXT:     let mut contracted: f64 = 0.0;
// LOWERING-NEXT:     let mut uncontracted: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 1.9999999999999998;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), _v1) };
// LOWERING-NEXT:     let _v2: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let _v5: f64 = _v3 * _v4;
// LOWERING-NEXT:     let _v6: f64 = -_v5;
// LOWERING-NEXT:     z = _v6;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let _v8: f64 = y;
// LOWERING-NEXT:         let _v9: f64 = z;
// LOWERING-NEXT:         let _v10: f64 = _v7 * _v8 + _v9;
// LOWERING-NEXT:         contracted = _v10;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v11: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let _v12: f64 = y;
// LOWERING-NEXT:         let _v13: f64 = _v11 * _v12;
// LOWERING-NEXT:         let _v14: f64 = z;
// LOWERING-NEXT:         let _v15: f64 = _v13 + _v14;
// LOWERING-NEXT:         uncontracted = _v15;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: *mut i8 = b"%.20e %.20e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: f64 = contracted;
// LOWERING-NEXT:     let _v18: f64 = uncontracted;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v16 as *const i8, _v17, _v18) };
// LOWERING-NEXT:     let _v20: i32 = 0;
// LOWERING-NEXT:     __retval = _v20;
// LOWERING-NEXT:     let _v21: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v21 as i32);
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
// REWRITES-NEXT: let mut x: f64 = 0.0;
// REWRITES-NEXT: let mut y: f64 = 0.0;
// REWRITES-NEXT: let mut z: f64 = 0.0;
// REWRITES-NEXT: let mut contracted: f64 = 0.0;
// REWRITES-NEXT: let mut uncontracted: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: f64 = 1.9999999999999998;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), _v1) };
// REWRITES-NEXT: y = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT: let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT: let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT: z = -(_v3 * _v4);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         contracted = _v7 * y + z;
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v11: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         uncontracted = _v11 * y + z;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v16: *mut i8 = b"%.20e %.20e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v16 as *const i8, contracted, uncontracted) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
