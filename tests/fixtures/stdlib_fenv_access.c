#include <fenv.h>
#include <stdio.h>

int main(void) {
  volatile double x = 3.0;
  volatile double y = 7.0;
  double          before;
  double          contended;
  double          after;

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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn fesetround(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut x: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut y: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut contended: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 7.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fesetround({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = __slate_fenv_div_f64({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         contended = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fesetround({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = contended;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_add_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sub_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_mul_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_div_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_rem_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_lt_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_le_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_gt_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ge_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_eq_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ne_f32(__a: f32, __b: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sin_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_cos_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_exp_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_exp2_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log2_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log10_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ceil_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_floor_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_round_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_rint_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_nearbyint_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_roundeven_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_trunc_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sqrt_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fabs_f32(__a: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_pow_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fmax_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fmin_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_copysign_f32(__a: f32, __b: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fma_f32(__a: f32, __b: f32, __c: f32) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_i64_to_f32(__a: i64) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_u64_to_f32(__a: u64) -> f32;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f32_to_i64(__a: f32) -> i64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f32_to_u64(__a: f32) -> u64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f32_to_bool(__a: f32) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_add_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sub_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_mul_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_div_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_rem_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_lt_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_le_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_gt_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ge_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_eq_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ne_f64(__a: f64, __b: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sin_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_cos_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_exp_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_exp2_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log2_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_log10_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_ceil_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_floor_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_round_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_rint_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_nearbyint_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_roundeven_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_trunc_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_sqrt_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fabs_f64(__a: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_pow_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fmax_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fmin_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_copysign_f64(__a: f64, __b: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_fma_f64(__a: f64, __b: f64, __c: f64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_i64_to_f64(__a: i64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_u64_to_f64(__a: u64) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f64_to_i64(__a: f64) -> i64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f64_to_u64(__a: f64) -> u64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f64_to_bool(__a: f64) -> bool;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f32_to_f64(__a: f32) -> f64;
// COMMON-LOWERING-NEXT:     safe fn __slate_fenv_f64_to_f32(__a: f64) -> f32;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 1024;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.20e %.20e %.20e\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 8388608;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.20e %.20e %.20e\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn fesetround(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut x: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut y: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut contended: f64 = 0.0;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), 3.0 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), 7.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-REWRITES-NEXT:     contended = __slate_fenv_div_f64({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     unsafe { fesetround(0 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%.20e %.20e %.20e\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             contended,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_add_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sub_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_mul_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_div_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_rem_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_lt_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_le_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_gt_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ge_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_eq_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ne_f32(__a: f32, __b: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sin_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_cos_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_exp_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_exp2_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log2_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log10_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ceil_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_floor_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_round_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_rint_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_nearbyint_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_roundeven_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_trunc_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sqrt_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fabs_f32(__a: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_pow_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fmax_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fmin_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_copysign_f32(__a: f32, __b: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fma_f32(__a: f32, __b: f32, __c: f32) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_i64_to_f32(__a: i64) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_u64_to_f32(__a: u64) -> f32;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f32_to_i64(__a: f32) -> i64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f32_to_u64(__a: f32) -> u64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f32_to_bool(__a: f32) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_add_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sub_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_mul_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_div_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_rem_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_lt_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_le_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_gt_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ge_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_eq_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ne_f64(__a: f64, __b: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sin_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_cos_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_exp_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_exp2_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log2_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_log10_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_ceil_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_floor_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_round_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_rint_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_nearbyint_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_roundeven_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_trunc_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_sqrt_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fabs_f64(__a: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_pow_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fmax_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fmin_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_copysign_f64(__a: f64, __b: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_fma_f64(__a: f64, __b: f64, __c: f64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_i64_to_f64(__a: i64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_u64_to_f64(__a: u64) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f64_to_i64(__a: f64) -> i64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f64_to_u64(__a: f64) -> u64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f64_to_bool(__a: f64) -> bool;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f32_to_f64(__a: f32) -> f64;
// COMMON-REWRITES-NEXT:     safe fn __slate_fenv_f64_to_f32(__a: f64) -> f32;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     unsafe { fesetround(1024 as i32) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     unsafe { fesetround(8388608 as i32) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
