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
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 0.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), _v1) };
// LOWERING-NEXT:     let _v2: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), _v2) };
// LOWERING-NEXT:     let _v3: f64 = 8.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), _v3) };
// LOWERING-NEXT:     let _v4: f64 = 5.75;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), _v4) };
// LOWERING-NEXT:     let _v5: f64 = 2.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), _v5) };
// LOWERING-NEXT:     let _v6: f64 = 2.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), _v6) };
// LOWERING-NEXT:     let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let _v8: f64 = unsafe { sin(_v7 as f64) };
// LOWERING-NEXT:     let _v9: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let _v10: f64 = unsafe { cos(_v9 as f64) };
// LOWERING-NEXT:     let _v11: f64 = _v8 + _v10;
// LOWERING-NEXT:     let _v12: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-NEXT:     let _v13: f64 = unsafe { tan(_v12 as f64) };
// LOWERING-NEXT:     let _v14: f64 = _v11 + _v13;
// LOWERING-NEXT:     trig = _v14;
// LOWERING-NEXT:     let _v15: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let _v16: f64 = unsafe { log(_v15 as f64) };
// LOWERING-NEXT:     let _v17: f64 = 100.0;
// LOWERING-NEXT:     let _v18: f64 = unsafe { log10(_v17 as f64) };
// LOWERING-NEXT:     let _v19: f64 = _v16 + _v18;
// LOWERING-NEXT:     let _v20: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let _v21: f64 = unsafe { log2(_v20 as f64) };
// LOWERING-NEXT:     let _v22: f64 = _v19 + _v21;
// LOWERING-NEXT:     logs = _v22;
// LOWERING-NEXT:     let _v23: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-NEXT:     let _v24: f64 = 3.0;
// LOWERING-NEXT:     let _v25: f64 = unsafe { pow(_v23 as f64, _v24 as f64) };
// LOWERING-NEXT:     let _v26: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-NEXT:     let _v27: f64 = unsafe { sqrt(_v26 as f64) };
// LOWERING-NEXT:     let _v28: f64 = _v25 + _v27;
// LOWERING-NEXT:     let _v29: f64 = 1.0;
// LOWERING-NEXT:     let _v30: f64 = unsafe { exp(_v29 as f64) };
// LOWERING-NEXT:     let _v31: f64 = _v28 + _v30;
// LOWERING-NEXT:     let _v32: f64 = 3.0;
// LOWERING-NEXT:     let _v33: f64 = unsafe { exp2(_v32 as f64) };
// LOWERING-NEXT:     let _v34: f64 = _v31 + _v33;
// LOWERING-NEXT:     powers = _v34;
// LOWERING-NEXT:     let _v35: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-NEXT:     let _v36: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// LOWERING-NEXT:     let _v37: f64 = unsafe { fmod(_v35 as f64, _v36 as f64) };
// LOWERING-NEXT:     rem = _v37;
// LOWERING-NEXT:     let _v38: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let _v39: i64 = unsafe { lround(_v38 as f64) };
// LOWERING-NEXT:     rounded = _v39;
// LOWERING-NEXT:     let _v40: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-NEXT:     let _v41: i64 = unsafe { llround(_v40 as f64) };
// LOWERING-NEXT:     rounded_ll = _v41;
// LOWERING-NEXT:     let _v42: f64 = 2.0;
// LOWERING-NEXT:     let _v43: f64 = unsafe { __slate_builtin_exp10(_v42) };
// LOWERING-NEXT:     exp10_val = _v43;
// LOWERING-NEXT:     let _v44: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v45: f64 = trig;
// LOWERING-NEXT:     let _v46: f64 = logs;
// LOWERING-NEXT:     let _v47: f64 = powers;
// LOWERING-NEXT:     let _v48: f64 = rem;
// LOWERING-NEXT:     let _v49: i64 = rounded;
// LOWERING-NEXT:     let _v50: i64 = rounded_ll;
// LOWERING-NEXT:     let _v51: f64 = exp10_val;
// LOWERING-NEXT:     let _v52: i32 = unsafe { printf(_v44 as *const i8, _v45, _v46, _v47, _v48, _v49, _v50, _v51) };
// LOWERING-NEXT:     let _v53: i32 = 0;
// LOWERING-NEXT:     __retval = _v53;
// LOWERING-NEXT:     let _v54: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v54 as i32);
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
// REWRITES-NEXT: let _v1: f64 = 0.5;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), _v1) };
// REWRITES-NEXT: let _v2: f64 = 2.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), _v2) };
// REWRITES-NEXT: let _v3: f64 = 8.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), _v3) };
// REWRITES-NEXT: let _v4: f64 = 5.75;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), _v4) };
// REWRITES-NEXT: let _v5: f64 = 2.0;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(e), _v5) };
// REWRITES-NEXT: let _v6: f64 = 2.5;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), _v6) };
// REWRITES-NEXT: let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let _v8: f64 = unsafe { sin(_v7 as f64) };
// REWRITES-NEXT: let _v9: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let _v10: f64 = unsafe { cos(_v9 as f64) };
// REWRITES-NEXT: let _v11: f64 = _v8 + _v10;
// REWRITES-NEXT: let _v12: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-NEXT: let _v13: f64 = unsafe { tan(_v12 as f64) };
// REWRITES-NEXT: trig = _v11 + _v13;
// REWRITES-NEXT: let _v15: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let _v16: f64 = unsafe { log(_v15 as f64) };
// REWRITES-NEXT: let _v17: f64 = 100.0;
// REWRITES-NEXT: let _v18: f64 = unsafe { log10(_v17 as f64) };
// REWRITES-NEXT: let _v19: f64 = _v16 + _v18;
// REWRITES-NEXT: let _v20: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let _v21: f64 = unsafe { log2(_v20 as f64) };
// REWRITES-NEXT: logs = _v19 + _v21;
// REWRITES-NEXT: let _v23: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-NEXT: let _v24: f64 = 3.0;
// REWRITES-NEXT: let _v25: f64 = unsafe { pow(_v23 as f64, _v24 as f64) };
// REWRITES-NEXT: let _v26: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-NEXT: let _v27: f64 = unsafe { sqrt(_v26 as f64) };
// REWRITES-NEXT: let _v28: f64 = _v25 + _v27;
// REWRITES-NEXT: let _v29: f64 = 1.0;
// REWRITES-NEXT: let _v30: f64 = unsafe { exp(_v29 as f64) };
// REWRITES-NEXT: let _v31: f64 = _v28 + _v30;
// REWRITES-NEXT: let _v32: f64 = 3.0;
// REWRITES-NEXT: let _v33: f64 = unsafe { exp2(_v32 as f64) };
// REWRITES-NEXT: powers = _v31 + _v33;
// REWRITES-NEXT: let _v35: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-NEXT: let _v36: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) };
// REWRITES-NEXT: rem = unsafe { fmod(_v35 as f64, _v36 as f64) };
// REWRITES-NEXT: let _v38: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT: rounded = unsafe { lround(_v38 as f64) };
// REWRITES-NEXT: let _v40: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-NEXT: rounded_ll = unsafe { llround(_v40 as f64) };
// REWRITES-NEXT: let _v42: f64 = 2.0;
// REWRITES-NEXT: exp10_val = unsafe { __slate_builtin_exp10(_v42) };
// REWRITES-NEXT: let _v44: *mut i8 = b"%.3f %.3f %.3f %.3f %ld %lld %.3f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v52: i32 = unsafe { printf(_v44 as *const i8, trig, logs, powers, rem, rounded, rounded_ll, exp10_val) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
