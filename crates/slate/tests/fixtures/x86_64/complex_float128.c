#include <stdio.h>

typedef _Complex __float128 cq;

static cq make(__float128 re, __float128 im) {
  cq z = re;
  __imag__ z = im;
  return z;
}

static cq mul(cq a, cq b) { return a * b; }
static cq div(cq a, cq b) { return a / b; }

static void show(const char *name, cq z) {
  printf("%s %a %a\n", name, (double)__real__ z, (double)__imag__ z);
}

int main(void) {
  cq a = make(1.5Q, -2.25Q);
  cq b = make(0.5Q, 4.0Q);
  show("add", a + b);
  show("sub", a - b);
  show("mul", mul(a, b));
  show("div", div(a, b));
  show("neg", -a);
  show("conj", ~a);
  show("scale", a * 2.0Q);
  show("rdiv", 3.0Q / b);
  __float128 inf = __builtin_inf();
  show("infmul", mul(make(inf, __builtin_nan("")), make(1.0Q, 1.0Q)));
  show("divzero", div(make(1.0Q, 1.0Q), make(0.0Q, 0.0Q)));
  cq tiny = make(0x1p-16000Q, 0x1p-16000Q);
  show("tinydiv", div(tiny, tiny));
  __float128 precise = __real__ mul(make(1.0Q + 0x1p-100Q, 0), make(1.0Q - 0x1p-100Q, 0));
  printf("%d %d %d\n", a == a, a == 1.5Q, precise == 1.0Q);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn make(re: f128, im: f128) -> num_complex::Complex<f128> {
// CHECK-NEXT:     let mut z: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: re,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     z.im = im;
// CHECK-NEXT:     z
// CHECK-NEXT: }
// CHECK-NEXT: fn mul(
// CHECK-NEXT:     mut a: num_complex::Complex<f128>,
// CHECK-NEXT:     b: num_complex::Complex<f128>,
// CHECK-NEXT: ) -> num_complex::Complex<f128> {
// CHECK-NEXT:     a.c_mul(b, false)
// CHECK-NEXT: }
// CHECK-NEXT: fn div(
// CHECK-NEXT:     mut a: num_complex::Complex<f128>,
// CHECK-NEXT:     b: num_complex::Complex<f128>,
// CHECK-NEXT: ) -> num_complex::Complex<f128> {
// CHECK-NEXT:     a.c_div(b, false)
// CHECK-NEXT: }
// CHECK-NEXT: fn show(name: *const i8, z: num_complex::Complex<f128>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re as f64, z.im as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: num_complex::Complex<f128> = make(1.5f128, -2.25f128);
// CHECK-NEXT:     let b: num_complex::Complex<f128> = make(0.5f128, 4f128);
// CHECK-NEXT:     show(c"add".as_ptr(), a + b);
// CHECK-NEXT:     show(c"sub".as_ptr(), a - b);
// CHECK-NEXT:     show(c"mul".as_ptr(), mul(a, b));
// CHECK-NEXT:     show(c"div".as_ptr(), div(a, b));
// CHECK-NEXT:     show(c"neg".as_ptr(), -a);
// CHECK-NEXT:     show(c"conj".as_ptr(), a.conj());
// CHECK-NEXT:     show(c"scale".as_ptr(), a * 2f128);
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"rdiv".as_ptr(),
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: 3f128,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         }
// CHECK-NEXT:             .c_div(b, false),
// CHECK-NEXT:     );
// CHECK-NEXT:     let inf: f128 = f64::INFINITY as f128;
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"infmul".as_ptr(),
// CHECK-NEXT:         mul(make(inf, f64::from_bits(0x7ff8000000000000) as f128), make(1f128, 1f128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(c"divzero".as_ptr(), div(make(1f128, 1f128), make(0f128, 0f128)));
// CHECK-NEXT:     let tiny: num_complex::Complex<f128> = make(
// CHECK-NEXT:         3.31184022194550157139472849083578569E-4817f128,
// CHECK-NEXT:         3.31184022194550157139472849083578569E-4817f128,
// CHECK-NEXT:     );
// CHECK-NEXT:     show(c"tinydiv".as_ptr(), div(tiny, tiny));
// CHECK-NEXT:     let precise: f128 = mul(
// CHECK-NEXT:             make(1f128 + 7.88860905221011805411728565282786229E-31f128, 0i32 as f128),
// CHECK-NEXT:             make(1f128 - 7.88860905221011805411728565282786229E-31f128, 0i32 as f128),
// CHECK-NEXT:         )
// CHECK-NEXT:         .re;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (a == a) as i32, (a == num_complex::Complex { re : 1.5f128, im : 0.0
// CHECK-NEXT:         }) as i32, (precise == 1f128) as i32
// CHECK-NEXT:     );
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
