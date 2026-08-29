#include <fenv.h>
#include <stdio.h>

int main(void) {
  volatile double x = 3.0;
  volatile double y = 7.0;
  double before;
  double contended;
  double after;

  before = x + y;

  fesetround(FE_DOWNWARD);
  {
#pragma STDC FENV_ACCESS ON
    contended = x / y;
  }
  fesetround(FE_TONEAREST);

  after = x + y;

  printf("%.20e %.20e %.20e\n", before, contended, after);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fesetround(_0: i32) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut y: f64 = 0.0;
// LOWERING-NEXT:     let mut before: f64 = 0.0;
// LOWERING-NEXT:     let mut contended: f64 = 0.0;
// LOWERING-NEXT:     let mut after: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 3.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), _v1) };
// LOWERING-NEXT:     let _v2: f64 = 7.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), _v2) };
// LOWERING-NEXT:     let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:     let _v5: f64 = _v3 + _v4;
// LOWERING-NEXT:     before = _v5;
// LOWERING-NEXT:     let _v6: i32 = 1024;
// LOWERING-NEXT:     let _v7: i32 = unsafe { fesetround(_v6 as i32) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v8: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let _v9: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:         let _v10: f64 = __slate_fenv_div_f64(_v8, _v9);
// LOWERING-NEXT:         contended = _v10;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     let _v12: i32 = unsafe { fesetround(_v11 as i32) };
// LOWERING-NEXT:     let _v13: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let _v14: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:     let _v15: f64 = _v13 + _v14;
// LOWERING-NEXT:     after = _v15;
// LOWERING-NEXT:     let _v16: *mut i8 = b"%.20e %.20e %.20e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: f64 = before;
// LOWERING-NEXT:     let _v18: f64 = contended;
// LOWERING-NEXT:     let _v19: f64 = after;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v16 as *const i8, _v17, _v18, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v22: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v22 as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     safe fn __slate_fenv_add_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_sub_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_mul_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_div_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_rem_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_lt_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_le_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_gt_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_ge_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_eq_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_ne_f32(a: f32, b: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_sin_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_cos_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_exp_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_exp2_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_log_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_log2_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_log10_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_ceil_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_floor_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_round_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_rint_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_nearbyint_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_roundeven_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_trunc_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_sqrt_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_fabs_f32(a: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_pow_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_fmax_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_fmin_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_copysign_f32(a: f32, b: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_fma_f32(a: f32, b: f32, c: f32) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_i64_to_f32(a: i64) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_u64_to_f32(a: u64) -> f32;
// LOWERING-NEXT:     safe fn __slate_fenv_f32_to_i64(a: f32) -> i64;
// LOWERING-NEXT:     safe fn __slate_fenv_f32_to_u64(a: f32) -> u64;
// LOWERING-NEXT:     safe fn __slate_fenv_f32_to_bool(a: f32) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_add_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_sub_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_mul_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_div_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_rem_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_lt_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_le_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_gt_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_ge_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_eq_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_ne_f64(a: f64, b: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_sin_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_cos_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_exp_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_exp2_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_log_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_log2_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_log10_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_ceil_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_floor_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_round_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_rint_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_nearbyint_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_roundeven_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_trunc_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_sqrt_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_fabs_f64(a: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_pow_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_fmax_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_fmin_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_copysign_f64(a: f64, b: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_fma_f64(a: f64, b: f64, c: f64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_i64_to_f64(a: i64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_u64_to_f64(a: u64) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_f64_to_i64(a: f64) -> i64;
// LOWERING-NEXT:     safe fn __slate_fenv_f64_to_u64(a: f64) -> u64;
// LOWERING-NEXT:     safe fn __slate_fenv_f64_to_bool(a: f64) -> bool;
// LOWERING-NEXT:     safe fn __slate_fenv_f32_to_f64(a: f32) -> f64;
// LOWERING-NEXT:     safe fn __slate_fenv_f64_to_f32(a: f64) -> f32;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fesetround(_0: i32) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: f64 = 0.0;
// REWRITES-NEXT: let mut y: f64 = 0.0;
// REWRITES-NEXT: let mut before: f64 = 0.0;
// REWRITES-NEXT: let mut contended: f64 = 0.0;
// REWRITES-NEXT: let mut after: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: f64 = 3.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), _v1) };
// REWRITES-NEXT: let _v2: f64 = 7.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), _v2) };
// REWRITES-NEXT: let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT: let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT: before = _v3 + _v4;
// REWRITES-NEXT: let _v6: i32 = 1024;
// REWRITES-NEXT: let _v7: i32 = unsafe { fesetround(_v6 as i32) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v8: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         let _v9: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT:         contended = __slate_fenv_div_f64(_v8, _v9);
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: i32 = 0;
// REWRITES-NEXT: let _v12: i32 = unsafe { fesetround(_v11 as i32) };
// REWRITES-NEXT: let _v13: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT: let _v14: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT: after = _v13 + _v14;
// REWRITES-NEXT: let _v16: *mut i8 = b"%.20e %.20e %.20e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v16 as *const i8, before, contended, after) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     safe fn __slate_fenv_add_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_sub_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_mul_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_div_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_rem_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_lt_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_le_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_gt_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_ge_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_eq_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_ne_f32(a: f32, b: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_sin_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_cos_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_exp_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_exp2_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_log_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_log2_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_log10_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_ceil_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_floor_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_round_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_rint_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_nearbyint_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_roundeven_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_trunc_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_sqrt_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_fabs_f32(a: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_pow_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_fmax_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_fmin_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_copysign_f32(a: f32, b: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_fma_f32(a: f32, b: f32, c: f32) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_i64_to_f32(a: i64) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_u64_to_f32(a: u64) -> f32;
// REWRITES-NEXT:     safe fn __slate_fenv_f32_to_i64(a: f32) -> i64;
// REWRITES-NEXT:     safe fn __slate_fenv_f32_to_u64(a: f32) -> u64;
// REWRITES-NEXT:     safe fn __slate_fenv_f32_to_bool(a: f32) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_add_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_sub_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_mul_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_div_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_rem_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_lt_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_le_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_gt_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_ge_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_eq_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_ne_f64(a: f64, b: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_sin_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_cos_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_exp_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_exp2_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_log_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_log2_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_log10_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_ceil_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_floor_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_round_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_rint_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_nearbyint_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_roundeven_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_trunc_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_sqrt_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_fabs_f64(a: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_pow_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_fmax_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_fmin_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_copysign_f64(a: f64, b: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_fma_f64(a: f64, b: f64, c: f64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_i64_to_f64(a: i64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_u64_to_f64(a: u64) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_f64_to_i64(a: f64) -> i64;
// REWRITES-NEXT:     safe fn __slate_fenv_f64_to_u64(a: f64) -> u64;
// REWRITES-NEXT:     safe fn __slate_fenv_f64_to_bool(a: f64) -> bool;
// REWRITES-NEXT:     safe fn __slate_fenv_f32_to_f64(a: f32) -> f64;
// REWRITES-NEXT:     safe fn __slate_fenv_f64_to_f32(a: f64) -> f32;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
