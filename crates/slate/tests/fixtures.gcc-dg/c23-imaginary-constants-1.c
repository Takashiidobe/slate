/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */

_Complex float a =
    1.if; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float b =
    2.Fj; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float c =
    3.fI; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float d =
    4.JF; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double e =
    1.i; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double f =
    2.j; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double g =
    3.I; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double h =
    4.J; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double i =
    1.il; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double j =
    2.Lj; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double k =
    3.lI; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double l =
    4.JL; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex float       m = 1.if;
__extension__ _Complex float       n = 2.Fj;
__extension__ _Complex float       o = 3.fI;
__extension__ _Complex float       p = 4.JF;
__extension__ _Complex double      q = 1.i;
__extension__ _Complex double      r = 2.j;
__extension__ _Complex double      s = 3.I;
__extension__ _Complex double      t = 4.J;
__extension__ _Complex long double u = 1.il;
__extension__ _Complex long double v = 2.Lj;
__extension__ _Complex long double w = 3.lI;
__extension__ _Complex long double x = 4.JL;

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int main() {
  if (a * a != -1.f || b * b != -4.f || c * c != -9.f || d * d != -16.f ||
      e * e != -1. || f * f != -4. || g * g != -9. || h * h != -16. ||
      i * i != -1.L || j * j != -4.L || k * k != -9.L || l * l != -16.L ||
      m * m != -1.f || n * n != -4.f || o * o != -9.f || p * p != -16.f ||
      q * q != -1. || r * r != -4. || s * s != -9. || t * t != -16. ||
      u * u != -1.L || v * v != -4.L || w * w != -9.L || x * x != -16.L)
    __builtin_abort();
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -1.0, im: 0.0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -4.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -9.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { d };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { d };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -16.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { e };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { e };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -1.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { f };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { f };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -4.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { g };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { g };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -9.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { h };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { h };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -16.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { i };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { i };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { i };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { i };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -1.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { j };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { j };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { j };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { j };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -4.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { k };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { k };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { k };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { k };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -9.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { l };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { l };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 3, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { l };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { l };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -1.600000e+01f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { m };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { m };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -1.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { n };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { n };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -4.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { o };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { o };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -9.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: -16.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { q };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { q };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -1.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { r };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { r };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -4.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { s };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { s };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -9.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { t };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { t };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: -16.0, im: 0.0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { u };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { u };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { u };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { u };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -1.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { v };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { v };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { v };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { v };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -4.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { w };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { w };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { w };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { w };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -9.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { x };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { x };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:                 __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                     num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                         re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                         im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:                     },
// LOWERING-X86_64-GNU-DAG:                 )
// LOWERING-X86_64-GNU-DAG:             };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 3, 192]),
// LOWERING-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { x };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { x };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:                 unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                 re: -1.600000e+01f128,
// LOWERING-AARCH64-GNU-DAG:                 im: 0.000000e+00f128,
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -1.0, im: 0.0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -4.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { c };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { c };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -9.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { d };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { d };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -16.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { e };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { e };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -1.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { f };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { f };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -4.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { g };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { g };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -9.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { h };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { h };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -16.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { i };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { i };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { i };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { i };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -1.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { j };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { j };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { j };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { j };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -4.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { k };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { k };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { k };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { k };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -9.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { l };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { l };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { l };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { l };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -1.600000e+01f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 3, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { m };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { m };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -1.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { n };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { n };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -4.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { o };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { o };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -9.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { __mulsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -16.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { q };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { q };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -1.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { r };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { r };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -4.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { s };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { s };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -9.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { t };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { t };
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { __muldc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != num_complex::Complex { re: -16.0, im: 0.0 };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { u };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { u };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { u };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { u };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -1.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { v };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { v };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { v };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { v };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -4.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { w };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { w };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { w };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { w };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -9.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { x };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { x };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-DAG:             __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:                 num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                     re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                     im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:                 },
// REWRITES-X86_64-GNU-DAG:             )
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { x };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { x };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-DAG:             unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:             re: -1.600000e+01f128,
// REWRITES-AARCH64-GNU-DAG:             im: 0.000000e+00f128,
// REWRITES-DAG:         };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:             != num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 3, 192]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
