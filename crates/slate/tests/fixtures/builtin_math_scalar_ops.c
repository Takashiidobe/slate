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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "fma"]
// CHECK-NEXT:     fn __builtin_fma([[V_0:__v[0-9]+]]: f64, [[V_1:__v[0-9]+]]: f64, [[V_2:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "hypot"]
// CHECK-NEXT:     fn __builtin_hypot([[V_3:__v[0-9]+]]: f64, [[V_4:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "fdim"]
// CHECK-NEXT:     fn __builtin_fdim([[V_5:__v[0-9]+]]: f64, [[V_6:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "cbrt"]
// CHECK-NEXT:     fn __builtin_cbrt([[V_7:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "ldexp"]
// CHECK-NEXT:     fn __builtin_ldexp([[V_8:__v[0-9]+]]: f64, [[V_9:__v[0-9]+]]: i32) -> f64;
// CHECK-NEXT:     #[link_name = "scalbn"]
// CHECK-NEXT:     fn __builtin_scalbn([[V_10:__v[0-9]+]]: f64, [[V_11:__v[0-9]+]]: i32) -> f64;
// CHECK-NEXT:     #[link_name = "logb"]
// CHECK-NEXT:     fn __builtin_logb([[V_12:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "ilogb"]
// CHECK-NEXT:     fn __builtin_ilogb([[V_13:__v[0-9]+]]: f64) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fma_v: f64 = unsafe { __builtin_fma(2.0f64, 3.0f64, 1.0f64) };
// CHECK-NEXT:     let hypot_v: f64 = unsafe { __builtin_hypot(3.0f64, 4.0f64) };
// CHECK-NEXT:     let fdim_v: f64 = unsafe { __builtin_fdim(5.0f64, 2.0f64) };
// CHECK-NEXT:     let fdim_neg: f64 = unsafe { __builtin_fdim(2.0f64, 5.0f64) };
// CHECK-NEXT:     let cbrt_v: f64 = unsafe { __builtin_cbrt(27.0f64) };
// CHECK-NEXT:     let ldexp_v: f64 = unsafe { __builtin_ldexp(1.0f64, 4i32) };
// CHECK-NEXT:     let scalbn_v: f64 = unsafe { __builtin_scalbn(10.0f64, 3i32) };
// CHECK-NEXT:     let logb_v: f64 = unsafe { __builtin_logb(10.0f64) };
// CHECK-NEXT:     let ilogb_v: i32 = unsafe { __builtin_ilogb(10.0f64) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n".as_ptr(),
// CHECK-NEXT:             fma_v,
// CHECK-NEXT:             hypot_v,
// CHECK-NEXT:             fdim_v,
// CHECK-NEXT:             fdim_neg,
// CHECK-NEXT:             cbrt_v,
// CHECK-NEXT:             ldexp_v,
// CHECK-NEXT:             scalbn_v,
// CHECK-NEXT:             logb_v,
// CHECK-NEXT:             ilogb_v,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
