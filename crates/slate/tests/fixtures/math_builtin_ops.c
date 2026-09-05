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
// LOWERING-NEXT:     #[link_name = "exp10"]
// LOWERING-NEXT:     fn __slate_builtin_exp10(_0: f64) -> f64;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn sin(_0: f64) -> f64;
// LOWERING-NEXT:     fn cos(_0: f64) -> f64;
// LOWERING-NEXT:     fn tan(_0: f64) -> f64;
// LOWERING-NEXT:     fn log(_0: f64) -> f64;
// LOWERING-NEXT:     fn log10(_0: f64) -> f64;
// LOWERING-NEXT:     fn log2(_0: f64) -> f64;
// LOWERING-NEXT:     fn pow(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn sqrt(_0: f64) -> f64;
// LOWERING-NEXT:     fn exp(_0: f64) -> f64;
// LOWERING-NEXT:     fn exp2(_0: f64) -> f64;
// LOWERING-NEXT:     fn fmod(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn lround(_0: f64) -> i64;
// LOWERING-NEXT:     fn llround(_0: f64) -> i64;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let mut c: f64 = 0.0;
// LOWERING-NEXT:     let mut d: f64 = 0.0;
// LOWERING-NEXT:     let mut e: f64 = 0.0;
// LOWERING-NEXT:     let mut f: f64 = 0.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 8.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.75;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { tan({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 100.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log10({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log2({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { pow({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sqrt({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp2({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fmod({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { lround({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { llround({{__v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { __slate_builtin_exp10({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut u8;
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
// REWRITES-NEXT:     #[link_name = "exp10"]
// REWRITES-NEXT:     fn __slate_builtin_exp10(_0: f64) -> f64;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn sin(_0: f64) -> f64;
// REWRITES-NEXT:     fn cos(_0: f64) -> f64;
// REWRITES-NEXT:     fn tan(_0: f64) -> f64;
// REWRITES-NEXT:     fn log(_0: f64) -> f64;
// REWRITES-NEXT:     fn log10(_0: f64) -> f64;
// REWRITES-NEXT:     fn log2(_0: f64) -> f64;
// REWRITES-NEXT:     fn pow(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn sqrt(_0: f64) -> f64;
// REWRITES-NEXT:     fn exp(_0: f64) -> f64;
// REWRITES-NEXT:     fn exp2(_0: f64) -> f64;
// REWRITES-NEXT:     fn fmod(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn lround(_0: f64) -> i64;
// REWRITES-NEXT:     fn llround(_0: f64) -> i64;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut a: f64 = 0.0;
// REWRITES-NEXT:     let mut b: f64 = 0.0;
// REWRITES-NEXT:     let mut c: f64 = 0.0;
// REWRITES-NEXT:     let mut d: f64 = 0.0;
// REWRITES-NEXT:     let mut e: f64 = 0.0;
// REWRITES-NEXT:     let mut f: f64 = 0.0;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), 0.5 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), 2.0 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), 8.0 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), 5.75 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), 2.0 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), 2.5 as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { tan({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log10(100.0 as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { log2({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { pow({{__v[0-9]+}} as f64, 3.0 as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sqrt({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp(1.0 as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp2(3.0 as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { fmod({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { lround({{__v[0-9]+}} as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%.3f %.3f %.3f %.3f %ld %lld %.3f\n".as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             unsafe { llround({{__v[0-9]+}} as f64) },
// REWRITES-NEXT:             unsafe { __slate_builtin_exp10(2.0 as f64) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
