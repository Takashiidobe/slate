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

  printf("%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n", fma_v, hypot_v,
         fdim_v, fdim_neg, cbrt_v, ldexp_v, scalbn_v, logb_v, ilogb_v);
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
// LOWERING-NEXT:     fn hypot(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn fdim(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn cbrt(_0: f64) -> f64;
// LOWERING-NEXT:     fn ldexp(_0: f64, _1: i32) -> f64;
// LOWERING-NEXT:     fn scalbn(_0: f64, _1: i32) -> f64;
// LOWERING-NEXT:     fn logb(_0: f64) -> f64;
// LOWERING-NEXT:     fn ilogb(_0: f64) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.mul_add({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { hypot({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fdim({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fdim({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 27.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cbrt({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { ldexp({{__v[0-9]+}} as f64, {{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { scalbn({{__v[0-9]+}} as f64, {{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { logb({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 10.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { ilogb({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT:     fn hypot(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn fdim(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn cbrt(_0: f64) -> f64;
// REWRITES-NEXT:     fn ldexp(_0: f64, _1: i32) -> f64;
// REWRITES-NEXT:     fn scalbn(_0: f64, _1: i32) -> f64;
// REWRITES-NEXT:     fn logb(_0: f64) -> f64;
// REWRITES-NEXT:     fn ilogb(_0: f64) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n".as_ptr(),
// REWRITES-NEXT:             (2.0 as f64).mul_add(3.0 as f64, 1.0 as f64),
// REWRITES-NEXT:             unsafe { hypot(3.0 as f64, 4.0 as f64) },
// REWRITES-NEXT:             unsafe { fdim(5.0 as f64, 2.0 as f64) },
// REWRITES-NEXT:             unsafe { fdim(2.0 as f64, 5.0 as f64) },
// REWRITES-NEXT:             unsafe { cbrt(27.0 as f64) },
// REWRITES-NEXT:             unsafe { ldexp(1.0 as f64, 4 as i32) },
// REWRITES-NEXT:             unsafe { scalbn(10.0 as f64, 3 as i32) },
// REWRITES-NEXT:             unsafe { logb(10.0 as f64) },
// REWRITES-NEXT:             unsafe { ilogb(10.0 as f64) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
