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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let mut c: f64 = 0.0;
// LOWERING-NEXT:     let mut d: f64 = 0.0;
// LOWERING-NEXT:     let mut e: f64 = 0.0;
// LOWERING-NEXT:     let mut f: f64 = 0.0;
// LOWERING-NEXT:     let mut trig: f64 = 0.0;
// LOWERING-NEXT:     let mut logs: f64 = 0.0;
// LOWERING-NEXT:     let mut powers: f64 = 0.0;
// LOWERING-NEXT:     let mut rem: f64 = 0.0;
// LOWERING-NEXT:     let mut rounded: i64 = 0;
// LOWERING-NEXT:     let mut rounded_ll: i64 = 0;
// LOWERING-NEXT:     let mut exp10_val: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 8.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 5.75;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { sin({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { cos({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { tan({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     trig = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { log({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 100.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { log10({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { log2({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     logs = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { pow({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { sqrt({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { exp({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { exp2({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     powers = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { fmod({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     rem = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { lround({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     rounded = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { llround({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     rounded_ll = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { __slate_builtin_exp10({{_v[0-9]+}}) };
// LOWERING-NEXT:     exp10_val = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = trig;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = logs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = powers;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = rem;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = rounded;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = rounded_ll;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = exp10_val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: f64 = 0.0;
// REWRITES-NEXT: let mut b: f64 = 0.0;
// REWRITES-NEXT: let mut c: f64 = 0.0;
// REWRITES-NEXT: let mut d: f64 = 0.0;
// REWRITES-NEXT: let mut e: f64 = 0.0;
// REWRITES-NEXT: let mut f: f64 = 0.0;
// REWRITES-NEXT: let mut trig: f64 = 0.0;
// REWRITES-NEXT: let mut logs: f64 = 0.0;
// REWRITES-NEXT: let mut powers: f64 = 0.0;
// REWRITES-NEXT: let mut rem: f64 = 0.0;
// REWRITES-NEXT: let mut rounded: i64 = 0;
// REWRITES-NEXT: let mut rounded_ll: i64 = 0;
// REWRITES-NEXT: let mut exp10_val: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.5;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 2.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 8.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 5.75;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 2.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 2.5;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { sin({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { cos({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { tan({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: trig = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { log({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 100.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { log10({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { log2({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: logs = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 3.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { pow({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { sqrt({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 1.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { exp({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 3.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { exp2({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: powers = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// REWRITES-NEXT: rem = unsafe { fmod({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT: rounded = unsafe { lround({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT: rounded_ll = unsafe { llround({{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 2.0;
// REWRITES-NEXT: exp10_val = unsafe { __slate_builtin_exp10({{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, trig, logs, powers, rem, rounded, rounded_ll, exp10_val) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
