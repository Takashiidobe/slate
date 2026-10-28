/* { dg-options "-fcomplex-arithmetic=promoted" } */
#include <stdio.h>
#include <string.h>

static void show(const char *name, double _Complex z) { printf("%s %a %a\n", name, __real__ z, __imag__ z); }
static void showf(const char *name, float _Complex z) { printf("%s %a %a\n", name, (double)__real__ z, (double)__imag__ z); }

static unsigned long long state = 88172645463325252ull;
static double next(void) {
  state ^= state << 13;
  state ^= state >> 7;
  state ^= state << 17;
  return (double)(long long)(state >> 11) / 0x1p40 - 4096.0;
}

int main(void) {
  unsigned long long hash = 0, hashf = 0;
  for (int i = 0; i < 4000; i++) {
    double _Complex q = (next() + next() * 1.0i) / (next() + next() * 1.0i);
    double parts[2] = {__real__ q, __imag__ q};
    unsigned long long bits[2];
    memcpy(bits, parts, sizeof bits);
    hash = hash * 31 + (bits[0] ^ bits[1] * 7);
    float _Complex qf = ((float)next() + (float)next() * 1.0fi) / ((float)next() + (float)next() * 1.0fi);
    float partsf[2] = {__real__ qf, __imag__ qf};
    unsigned int bitsf[2];
    memcpy(bitsf, partsf, sizeof bitsf);
    hashf = hashf * 31 + (bitsf[0] ^ bitsf[1] * 7);
  }
  printf("hash %llx %llx\n", hash, hashf);
  volatile double big = 1e300, one = 1.0, zero = 0.0, inf = __builtin_inf();
  double _Complex z = big + big * 1.0i;
  show("big", z / z);
  show("small", (one + 2.0i) / (3.0 + 4.0i));
  show("tilted", (one + 2.0i) / (4.0 + 3.0i));
  show("equal", (one + 2.0i) / (3.0 - 3.0i));
  show("zero", (one + one * 1.0i) / (zero + zero * 1.0i));
  show("inf", (inf + one * 1.0i) / (one + one * 1.0i));
  show("real", one / (3.0 + 4.0i));
  show("imaginary", (one + 2.0i) / (3.0i));
  show("mul", (inf + inf * 1.0i) * (one + zero * 1.0i));
  float _Complex f = (float)one + 2.0fi;
  showf("float", f / (3.0f + 7.0fi));
  long double _Complex l = (long double)one + 2.0Li;
  l /= 3.0L + 7.0Li;
  printf("long %.6Lf %.6Lf\n", __real__ l, __imag__ l);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fcomplex-arithmetic=promoted" }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: mod __slate_libgcc {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     #[link(name = "gcc", kind = "static")]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     unsafe extern "C" {}
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u32) -> *mut ();
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_add(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_add(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_sub(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_sub(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_neg(
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_neg(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *const i8, _1: LongDouble, _2: LongDouble) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *const i8, _1: LongDouble, _2: LongDouble) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// I686-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[derive(Clone, Copy)]
// I686-UNKNOWN-LINUX-GNU-NEXT: #[derive(Clone, Copy)]
// X86_64-UNKNOWN-LINUX-GNU-NEXT: struct LongDouble([u8; 10]);
// I686-UNKNOWN-LINUX-GNU-NEXT: struct LongDouble([u8; 10]);
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Add for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Add for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_add(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_add(self, __o)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Sub for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Sub for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_sub(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_sub(self, __o)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Mul for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Mul for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_mul(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_mul(self, __o)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Div for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Div for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_div(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_div(self, __o)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Neg for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Neg for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn neg(self) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn neg(self) -> LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_neg(self)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_neg(self)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// I686-UNKNOWN-LINUX-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:         } else {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// I686-UNKNOWN-LINUX-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:             } else {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// I686-UNKNOWN-LINUX-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 } else {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     None
// I686-UNKNOWN-LINUX-GNU-NEXT:                     None
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             }
// I686-UNKNOWN-LINUX-GNU-NEXT:             }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// X86_64-UNKNOWN-LINUX-GNU-NEXT: struct __SlateAlign16<T>(T);
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// I686-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const u8, z: num_complex::Complex<f64>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re, z.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn showf(name: *const i8, z: num_complex::Complex<f32>) {
// I686-UNKNOWN-LINUX-GNU-NEXT: fn showf(name: *const i8, z: num_complex::Complex<f32>) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn showf(name: *const u8, z: num_complex::Complex<f32>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re as f64, z.im as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut state: u64 = 88172645463325252;
// CHECK-EMPTY:
// CHECK-NEXT: fn next() -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         state = state ^ state << 13i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         state = state ^ state >> 7i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         state = state ^ state << 17i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((((unsafe { state }) >> 11i32) as i64) as f64) / 1099511627776.0f64 - 4096.0f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut hash: u64 = 0;
// CHECK-NEXT:     let mut hashf: u64 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..4000i32 {
// CHECK-NEXT:         let q: num_complex::Complex<f64> = {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_0:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:             let [[T_0:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_1:__t[0-9]+]] = next()
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_1:__t[0-9]+]] = next()
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         + next()
// I686-UNKNOWN-LINUX-GNU-NEXT:                         + next()
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                                 re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                                 re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                                 im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                                 im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             };
// I686-UNKNOWN-LINUX-GNU-NEXT:                             };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_1]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_1]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_1]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_1]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// I686-UNKNOWN-LINUX-GNU-NEXT:                 },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_0:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_1:__t[0-9]+]] = next()
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     + next()
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_1]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_1]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             .c_div(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_2:__t[0-9]+]] = next()
// CHECK-NEXT:                         + next()
// CHECK-NEXT:                             * num_complex::Complex {
// CHECK-NEXT:                                 re: 0.0f64,
// CHECK-NEXT:                                 im: 1.0f64,
// CHECK-NEXT:                             };
// CHECK-NEXT:                     num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_2]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_2]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_2]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_2]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_2]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_2]].im as f128,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 true,
// CHECK-NEXT:             );
// CHECK-NEXT:             num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 re: __slate_f80_to_f64([[T_0]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 re: __slate_f80_to_f64([[T_0]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 im: __slate_f80_to_f64([[T_0]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 im: __slate_f80_to_f64([[T_0]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_0]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_0]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let mut parts: __SlateAlign16<[f64; 2]> = __SlateAlign16([q.re, q.im]);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let mut bits: __SlateAlign16<[u64; 2]> = __SlateAlign16([0u64; 2]);
// I686-UNKNOWN-LINUX-GNU-NEXT:         let mut parts: [f64; 2] = [q.re, q.im];
// I686-UNKNOWN-LINUX-GNU-NEXT:         let mut bits: [u64; 2] = [0u64; 2];
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let mut parts: [f64; 2] = [q.re, q.im];
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let mut bits: [u64; 2] = [0u64; 2];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::copy_nonoverlapping(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 parts.0.as_mut_ptr() as *const u8,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 bits.0.as_mut_ptr() as *mut u8,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 parts.as_mut_ptr() as *const u8,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 bits.as_mut_ptr() as *mut u8,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 parts.as_mut_ptr() as *const u8,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 bits.as_mut_ptr() as *mut u8,
// CHECK-NEXT:                 16 as usize,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         hash = hash
// CHECK-NEXT:             .wrapping_mul(31)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             .wrapping_add(bits.0[0] ^ bits.0[1].wrapping_mul(7));
// I686-UNKNOWN-LINUX-GNU-NEXT:             .wrapping_add(bits[0] ^ bits[1].wrapping_mul(7));
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             .wrapping_add(bits[0] ^ bits[1].wrapping_mul(7));
// CHECK-NEXT:         let qf: num_complex::Complex<f32> = {
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = {
// CHECK-NEXT:                 let [[T_4:__t[0-9]+]] = (next() as f32)
// CHECK-NEXT:                     + (next() as f32)
// CHECK-NEXT:                         * num_complex::Complex {
// CHECK-NEXT:                             re: 0.0f32,
// CHECK-NEXT:                             im: 1.0f32,
// CHECK-NEXT:                         };
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: [[T_4]].re as f64,
// CHECK-NEXT:                     im: [[T_4]].im as f64,
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             .c_div(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_5:__t[0-9]+]] = (next() as f32)
// CHECK-NEXT:                         + (next() as f32)
// CHECK-NEXT:                             * num_complex::Complex {
// CHECK-NEXT:                                 re: 0.0f32,
// CHECK-NEXT:                                 im: 1.0f32,
// CHECK-NEXT:                             };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_5]].re as f64,
// CHECK-NEXT:                         im: [[T_5]].im as f64,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 true,
// CHECK-NEXT:             );
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_3]].re as f32,
// CHECK-NEXT:                 im: [[T_3]].im as f32,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut partsf: [f32; 2] = [qf.re, qf.im];
// CHECK-NEXT:         let mut bitsf: [u32; 2] = [0u32; 2];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                 partsf.as_mut_ptr() as *const u8,
// CHECK-NEXT:                 bitsf.as_mut_ptr() as *mut u8,
// CHECK-NEXT:                 8 as usize,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         hashf = hashf
// CHECK-NEXT:             .wrapping_mul(31)
// CHECK-NEXT:             .wrapping_add((bitsf[0] ^ bitsf[1].wrapping_mul(7)) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"hash %llx %llx\n".as_ptr(), hash, hashf) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let big: f64 = 1e300f64;
// CHECK-NEXT:     let one: f64 = 1.0f64;
// CHECK-NEXT:     let zero: f64 = 0.0f64;
// CHECK-NEXT:     let inf: f64 = f64::INFINITY;
// CHECK-NEXT:     let z: num_complex::Complex<f64> =
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 };
// CHECK-NEXT:     show(c"big".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_6:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_6:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_7:__t[0-9]+]] = z;
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_7:__t[0-9]+]] = z;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_7]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_7]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_7]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_7]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_6:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_7:__t[0-9]+]] = z;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_7]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_7]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_8:__t[0-9]+]] = z;
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_8]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_8]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_8]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_8]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_8]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_8]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_6]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_6]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_6]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_6]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_6]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_6]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"small".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_9:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_9:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_10:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_10:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_10]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_10]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_10]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_10]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_9:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_10:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_10]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_10]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_11:__t[0-9]+]] = 3.0f64
// CHECK-NEXT:                     + num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 4.0f64,
// CHECK-NEXT:                     };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_11]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_11]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_11]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_11]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_11]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_11]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_9]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_9]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_9]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_9]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_9]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_9]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"tilted".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_12:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_12:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_13:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_13:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_13]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_13]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_13]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_13]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_12:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_13:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_13]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_13]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_14:__t[0-9]+]] = 4.0f64
// CHECK-NEXT:                     + num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 3.0f64,
// CHECK-NEXT:                     };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_14]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_14]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_14]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_14]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_14]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_14]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_12]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_12]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_12]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_12]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_12]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_12]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"equal".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_15:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_15:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_16:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_16:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_16]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_16]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_16]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_16]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_15:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_16:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_16]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_16]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_17:__t[0-9]+]] = 3.0f64
// CHECK-NEXT:                     - num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 3.0f64,
// CHECK-NEXT:                     };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_17]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_17]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_17]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_17]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_17]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_17]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_15]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_15]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_15]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_15]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_15]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_15]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"zero".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_18:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_18:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_19:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_19:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_19]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_19]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_19]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_19]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_18:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_19:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_19]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_19]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_20:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                         * num_complex::Complex {
// CHECK-NEXT:                             re: 0.0f64,
// CHECK-NEXT:                             im: 1.0f64,
// CHECK-NEXT:                         };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_20]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_20]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_20]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_20]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_20]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_20]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_18]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_18]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_18]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_18]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_18]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_18]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"inf".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_21:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_21:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_22:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_22:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_22]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_22]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_22]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_22]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_21:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_22:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_22]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_22]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_23:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                     + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                         * num_complex::Complex {
// CHECK-NEXT:                             re: 0.0f64,
// CHECK-NEXT:                             im: 1.0f64,
// CHECK-NEXT:                         };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_23]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_23]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_23]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_23]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_23]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_23]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_21]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_21]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_21]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_21]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_21]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_21]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"real".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_24:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_24:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 re: __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 re: __slate_f80_from_f64(unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_24:__t[0-9]+]] = num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }) as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: 0.0,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_25:__t[0-9]+]] = 3.0f64
// CHECK-NEXT:                     + num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 4.0f64,
// CHECK-NEXT:                     };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_25]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_25]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_25]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_25]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_25]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_25]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_24]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_24]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_24]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_24]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_24]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_24]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(c"imaginary".as_ptr(), {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_26:__t[0-9]+]] = __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_26:__t[0-9]+]] = __slate_cf80_div_basic(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_27:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_27:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_27]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_27]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_27]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_27]].im),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_26:__t[0-9]+]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             let [[T_27:__t[0-9]+]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_27]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_27]].im as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_28:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 3.0f64,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_28]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_28]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_28]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_28]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_28]].re as f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_28]].im as f128,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_26]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_26]].re),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_26]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_26]].im),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_26]].re as f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_26]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"mul".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 })
// CHECK-NEXT:         .c_mul(
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// CHECK-NEXT:                     },
// CHECK-NEXT:             true,
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     let f: num_complex::Complex<f32> =
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }) as f32)
// CHECK-NEXT:             + num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f32,
// CHECK-NEXT:                 im: 2.0f32,
// CHECK-NEXT:             };
// CHECK-NEXT:     showf(c"float".as_ptr(), {
// CHECK-NEXT:         let [[T_29:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_30:__t[0-9]+]] = f;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_30]].re as f64,
// CHECK-NEXT:                 im: [[T_30]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         .c_div(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_31:__t[0-9]+]] = 3.0f32
// CHECK-NEXT:                     + num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f32,
// CHECK-NEXT:                         im: 7.0f32,
// CHECK-NEXT:                     };
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: [[T_31]].re as f64,
// CHECK-NEXT:                     im: [[T_31]].im as f64,
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             true,
// CHECK-NEXT:         );
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_29]].re as f32,
// CHECK-NEXT:             im: [[T_29]].im as f32,
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut l: num_complex::Complex<LongDouble> = {
// I686-UNKNOWN-LINUX-GNU-NEXT:     let mut l: num_complex::Complex<LongDouble> = {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_32:__t[0-9]+]] =
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_32:__t[0-9]+]] =
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) });
// I686-UNKNOWN-LINUX-GNU-NEXT:             __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) });
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_33:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_33:__t[0-9]+]] = num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         };
// I686-UNKNOWN-LINUX-GNU-NEXT:         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_32]] + [[T_33]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_32]] + [[T_33]].re,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_33]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_33]].im,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     };
// I686-UNKNOWN-LINUX-GNU-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     l = __slate_cf80_div_improved(l, {
// I686-UNKNOWN-LINUX-GNU-NEXT:     l = __slate_cf80_div_improved(l, {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_34:__t[0-9]+]] = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_34:__t[0-9]+]] = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let [[T_35:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_35:__t[0-9]+]] = num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 1, 64]),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 1, 64]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         };
// I686-UNKNOWN-LINUX-GNU-NEXT:         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_34]] + [[T_35]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: [[T_34]] + [[T_35]].re,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_35]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: [[T_35]].im,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let mut l: num_complex::Complex<f128> =
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }) as f128)
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_0:__v[0-9]+]]: num_complex::Complex<f128> = l;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     l = [[V_0]].c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         3f128
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: 7f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { __slate_printf__ri32_pi8_f80_f80(c"long %.6Lf %.6Lf\n".as_ptr(), l.re, l.im) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { __slate_printf__ri32_pi8_f80_f80(c"long %.6Lf %.6Lf\n".as_ptr(), l.re, l.im) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"long %.6Lf %.6Lf\n".as_ptr(), l.re, l.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
