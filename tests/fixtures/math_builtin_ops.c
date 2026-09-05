#include <stdio.h>

int main(void) {
  volatile double a    = 0.5;
  volatile double b    = 2.0;
  volatile double c    = 8.0;
  volatile double d    = 5.75;
  volatile double e    = 2.0;
  volatile double f    = 2.5;
  double          trig = __builtin_sin(a) + __builtin_cos(a) + __builtin_tan(a);
  double logs   = __builtin_log(c) + __builtin_log10(100.0) + __builtin_log2(c);
  double powers = __builtin_pow(b, 3.0) + __builtin_sqrt(c) +
                  __builtin_exp(1.0) + __builtin_exp2(3.0);
  double rem    = __builtin_fmod(d, e);
  long   rounded       = __builtin_lround(f);
  long long rounded_ll = __builtin_llround(f);
#if __has_builtin(__builtin_elementwise_exp10)
  double exp10_val = __builtin_elementwise_exp10(2.0);
#else
  double exp10_val = __builtin_pow(10.0, 2.0);
#endif
  printf("%.3f %.3f %.3f %.3f %ld %lld %.3f\n", trig, logs, powers, rem,
         rounded, rounded_ll, exp10_val);
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
// COMMON-LOWERING-NEXT:     #[link_name = "exp10"]
// COMMON-LOWERING-NEXT:     fn __slate_builtin_exp10(_0: f64) -> f64;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn sin(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn cos(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn tan(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn log(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn log10(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn log2(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn pow(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn sqrt(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn exp(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn exp2(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn fmod(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn lround(_0: f64) -> i64;
// COMMON-LOWERING-NEXT:     fn llround(_0: f64) -> i64;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut a: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut b: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut c: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut d: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut e: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut f: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 8.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.75;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.5;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { tan({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 100.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log10({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log2({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { pow({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sqrt({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp2({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fmod({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { lround({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { llround({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { __slate_builtin_exp10({{__v[0-9]+}}) };
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
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     #[link_name = "exp10"]
// COMMON-REWRITES-NEXT:     fn __slate_builtin_exp10(_0: f64) -> f64;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn sin(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn cos(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn tan(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn log(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn log10(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn log2(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn pow(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn sqrt(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn exp(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn exp2(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn fmod(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn lround(_0: f64) -> i64;
// COMMON-REWRITES-NEXT:     fn llround(_0: f64) -> i64;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut a: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut b: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut c: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut d: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut e: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut f: f64 = 0.0;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), 0.5 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), 2.0 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), 8.0 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), 5.75 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), 2.0 as f64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), 2.5 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { tan({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log10(100.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log2({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { pow({{__v[0-9]+}} as f64, 3.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sqrt({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp2(3.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fmod({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { lround({{__v[0-9]+}} as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%.3f %.3f %.3f %.3f %ld %lld %.3f\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             unsafe { llround({{__v[0-9]+}} as f64) },
// COMMON-REWRITES-NEXT:             unsafe { __slate_builtin_exp10(2.0 as f64) },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
