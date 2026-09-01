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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fesetround(_0: i32) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut y: f64 = 0.0;
// LOWERING-NEXT:     let mut contended: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 7.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1024;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fesetround({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = __slate_fenv_div_f64({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:         contended = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fesetround({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.20e %.20e %.20e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = contended;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fesetround(_0: i32) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut x: f64 = 0.0;
// REWRITES-NEXT:     let mut y: f64 = 0.0;
// REWRITES-NEXT:     let mut contended: f64 = 0.0;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), 3.0 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), 7.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe { fesetround(1024 as i32) };
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT:         contended = __slate_fenv_div_f64({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { fesetround(0 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-NEXT:     unsafe { printf(c"%.20e %.20e %.20e\n".as_ptr(), {{_v[0-9]+}}, contended, {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
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
