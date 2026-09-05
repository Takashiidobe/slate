#include <stdio.h>

int main(void) {
  double fma_v    = __builtin_fma(2.0, 3.0, 1.0);
  double hypot_v  = __builtin_hypot(3.0, 4.0);
  double fdim_v   = __builtin_fdim(5.0, 2.0);
  double fdim_neg = __builtin_fdim(2.0, 5.0);
  double cbrt_v   = __builtin_cbrt(27.0);
  double ldexp_v  = __builtin_ldexp(1.0, 4);
  double scalbn_v = __builtin_scalbn(10.0, 3);
  double logb_v   = __builtin_logb(10.0);
  int    ilogb_v  = __builtin_ilogb(10.0);

  printf("%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n", fma_v, hypot_v, fdim_v,
         fdim_neg, cbrt_v, ldexp_v, scalbn_v, logb_v, ilogb_v);
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
// COMMON-LOWERING-NEXT:     fn hypot(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn fdim(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn cbrt(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn ldexp(_0: f64, _1: i32) -> f64;
// COMMON-LOWERING-NEXT:     fn scalbn(_0: f64, _1: i32) -> f64;
// COMMON-LOWERING-NEXT:     fn logb(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn ilogb(_0: f64) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.mul_add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { hypot({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fdim({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fdim({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 27.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cbrt({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { ldexp({{__v[0-9]+}} as f64, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { scalbn({{__v[0-9]+}} as f64, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { logb({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { ilogb({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     fn hypot(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn fdim(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn cbrt(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn ldexp(_0: f64, _1: i32) -> f64;
// COMMON-REWRITES-NEXT:     fn scalbn(_0: f64, _1: i32) -> f64;
// COMMON-REWRITES-NEXT:     fn logb(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn ilogb(_0: f64) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (2.0 as f64).mul_add(3.0 as f64, 1.0 as f64),
// COMMON-REWRITES-NEXT:             unsafe { hypot(3.0 as f64, 4.0 as f64) },
// COMMON-REWRITES-NEXT:             unsafe { fdim(5.0 as f64, 2.0 as f64) },
// COMMON-REWRITES-NEXT:             unsafe { fdim(2.0 as f64, 5.0 as f64) },
// COMMON-REWRITES-NEXT:             unsafe { cbrt(27.0 as f64) },
// COMMON-REWRITES-NEXT:             unsafe { ldexp(1.0 as f64, 4 as i32) },
// COMMON-REWRITES-NEXT:             unsafe { scalbn(10.0 as f64, 3 as i32) },
// COMMON-REWRITES-NEXT:             unsafe { logb(10.0 as f64) },
// COMMON-REWRITES-NEXT:             unsafe { ilogb(10.0 as f64) },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
